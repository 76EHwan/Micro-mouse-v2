/*
 * DRV8316C.c
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#include "DRV8316C.h"

DRV8316C_Handle_t DRV8316C_L;
DRV8316C_Handle_t DRV8316C_R;

/* Macros for manual nCS pin control */
#define DRV8316C_CS_LOW(hdrv)     HAL_GPIO_WritePin((hdrv)->nCS_Port, (hdrv)->nCS_Pin, GPIO_PIN_RESET)
#define DRV8316C_CS_HIGH(hdrv)    HAL_GPIO_WritePin((hdrv)->nCS_Port, (hdrv)->nCS_Pin, GPIO_PIN_SET)

/*=======================================================================*/
/* Internal Helper Functions                                             */
/*=======================================================================*/

/**
 * @brief  Calculates the even parity bit by counting the number of '1's.
 * @note   Datasheet 8.5.1.1: "Parity bit is set such that the SDI input data word has even number of 1s"
 * @param  data: 16-bit SPI frame with parity bit (B8) cleared to 0.
 * @return 1 (if 1s count is odd), 0 (if 1s count is even)
 */
static uint8_t DRV8316C_CalculateEvenParity(uint16_t data)
{
    uint8_t one_count = 0;
    // Clear the parity bit (B8) just in case, to count the other 15 bits
    data &= ~DRV_SPI_PARITY_BIT;

    for (int i = 0; i < 16; i++)
    {
        if ((data >> i) & 0x01)
        {
            one_count++;
        }
    }

    // If the count of 1s is odd, return 1 to make the total count even.
    return (one_count % 2);
}

/**
 * @brief  Internal helper function to perform SPI transmit/receive
 */
static HAL_StatusTypeDef DRV8316C_SPI_TxRx(DRV8316C_Handle_t* hdrv, uint16_t* pTxData, uint16_t* pRxData)
{
    HAL_StatusTypeDef status;

    DRV8316C_CS_LOW(hdrv); // Activate Chip Select (LOW)

    // Transmit 1 frame of 16 bits (size=1) and receive 1 frame simultaneously
    status = HAL_SPI_TransmitReceive(hdrv->hspi, (uint8_t*)pTxData, (uint8_t*)pRxData, 1, HAL_MAX_DELAY);

    DRV8316C_CS_HIGH(hdrv); // Deactivate Chip Select (HIGH)

    return status;
}

/*=======================================================================*/
/* Public Function Implementations                                       */
/*=======================================================================*/

/**
 * @brief  Initializes the DRV8316C handle.
 */
void DRV8316C_Init(DRV8316C_Handle_t* hdrv, SPI_HandleTypeDef* hspi,
                   GPIO_TypeDef* nCS_Port, uint16_t nCS_Pin)
{
    hdrv->hspi = hspi;
    hdrv->nCS_Port = nCS_Port;
    hdrv->nCS_Pin = nCS_Pin;

    // Set initial pin states
    DRV8316C_CS_HIGH(hdrv);  // nCS starts inactive (HIGH)
}

/**
 * @brief  Writes 8 bits of data to a specific DRV8316C register.
 */
HAL_StatusTypeDef DRV8316C_WriteRegister(DRV8316C_Handle_t* hdrv, uint8_t regAddr, uint8_t data)
{
    uint16_t tx_frame = 0;
    uint16_t rx_frame = 0; // SDO returns the *previous* value of the register being written

    // 1. Construct the basic frame with R/W=0, address, and data
    tx_frame = DRV_SPI_WRITE_MASK |
               ((regAddr << DRV_SPI_ADDR_SHIFT) & DRV_SPI_ADDR_MASK) |
               (data & DRV_SPI_DATA_MASK);

    // 2. Calculate and set the even parity bit
    if (DRV8316C_CalculateEvenParity(tx_frame))
    {
        tx_frame |= DRV_SPI_PARITY_BIT;
    }

    // 3. Transmit the frame
    return DRV8316C_SPI_TxRx(hdrv, &tx_frame, &rx_frame);
}

/**
 * @brief  Reads 8 bits of data from a specific DRV8316C register.
 */
HAL_StatusTypeDef DRV8316C_ReadRegister(DRV8316C_Handle_t* hdrv, uint8_t regAddr, uint8_t* pData)
{
    uint16_t tx_frame = 0;
    uint16_t rx_frame = 0;
    HAL_StatusTypeDef status;

    // 1. Construct the read frame with R/W=1 and address (data field is 0)
    tx_frame = DRV_SPI_READ_MASK |
               ((regAddr << DRV_SPI_ADDR_SHIFT) & DRV_SPI_ADDR_MASK);

    // 2. Calculate and set the even parity bit
    if (DRV8316C_CalculateEvenParity(tx_frame))
    {
        tx_frame |= DRV_SPI_PARITY_BIT;
    }

    // 3. Transmit/Receive
    //    For a read command, SDO immediately returns the requested register data
    status = DRV8316C_SPI_TxRx(hdrv, &tx_frame, &rx_frame);

    if (status == HAL_OK)
    {
        // The lower 8 bits of the received frame contain the data
        *pData = (rx_frame & DRV_SPI_DATA_MASK);
    }

    return status;
}

/**
 * @brief  Unlock DRV8316C register.
 */

HAL_StatusTypeDef DRV8316C_UnlockRegister(DRV8316C_Handle_t* hdrv)
{
	HAL_StatusTypeDef status;
    status = DRV8316C_WriteRegister(hdrv, 0x3, 0x3);
    return status;
}

/**
 * @brief  Lock DRV8316C register.
 */

HAL_StatusTypeDef DRV8316C_LockRegister(DRV8316C_Handle_t* hdrv)
{
	HAL_StatusTypeDef status;
    status = DRV8316C_WriteRegister(hdrv, 0x3, 0x6);
    return status;
}

/**
 * @brief  Applies a common default configuration to the DRV8316C.
 * (For FOC: 6x PWM, 0.6V/V Gain, OCP Auto-Retry, 125V/us Slew)
 */
HAL_StatusTypeDef DRV8316C_ApplyDefaultConfig(DRV8316C_Handle_t* hdrv)
{
    HAL_StatusTypeDef status;
    uint8_t reg_val;

    // --- CTRL_2 (0x04): 3x PWM Mode, Slew Rate 125V/us, SDO Push-Pull ---
    reg_val = DRV_CTRL2_SDO_MODE_PP |
              DRV_CTRL2_SLEW_125V_us |
              DRV_CTRL2_PWM_MODE_3X;
    status = DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_2, reg_val);
    if (status != HAL_OK) return status;

    // --- CTRL_3 (0x05): Enable OVP, Enable OTW reporting on nFAULT ---
    reg_val = (1 << 2) | // OVP_EN = 1
              (1 << 0);  // OTW_REP = 1
    status = DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_3, reg_val);
    if (status != HAL_OK) return status;

    // --- CTRL_4 (0x06): OCP Mode = Auto-Retry, OCP Level = 16A ---
    reg_val = DRV_CTRL4_OCP_MODE_RETRY |
              DRV_CTRL4_OCP_LVL_16A |
              (1 << 4); // OCP_DEG 0.6us (default 1h)
    status = DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_4, reg_val);
    if (status != HAL_OK) return status;

    // --- CTRL_5 (0x07): CSA Gain = 0.6 V/A (good for FOC), Enable Active Demag ---
    reg_val = DRV_CTRL5_CSA_GAIN_0_6 |
              DRV_CTRL5_EN_ASR_BIT |
              DRV_CTRL5_EN_AAR_BIT;
    status = DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_5, reg_val);
    if (status != HAL_OK) return status;

    // --- CTRL_6 (0x08): Buck converter disable, 3.3V output ---
    reg_val = 1; // BUCK_DIS = 	1b
    status = DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_6, reg_val);
    if (status != HAL_OK) return status;

    return HAL_OK;
}

/**
 * @brief  Clears all fault flags.
 */
HAL_StatusTypeDef DRV8316C_ClearFaults(DRV8316C_Handle_t* hdrv)
{
    // To clear latched faults, we write 1 to the CLR_FLT bit.
    // This bit is self-clearing, so we must also write the other
    // bits in the register to their desired values.

    // Example: Clear faults while maintaining 6x PWM, 125V/us Slew, Push-Pull SDO
    uint8_t reg_val = DRV_CTRL2_SDO_MODE_PP |
                      DRV_CTRL2_SLEW_125V_us |
                      DRV_CTRL2_PWM_MODE_3X |
                      DRV_CTRL2_CLR_FLT_BIT;

    return DRV8316C_WriteRegister(hdrv, DRV_REG_CTRL_2, reg_val);
}
