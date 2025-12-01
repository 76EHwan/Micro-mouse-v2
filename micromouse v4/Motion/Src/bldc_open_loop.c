/*
 * bldc_open_loop.c
 *
 * Description: Open Loop SVPWM Control for DRV8316C (3x PWM Mode)
 * Created on: Nov 10, 2025
 * Author: (Modified for Open Loop)
 */

#include "main.h"
#include "math.h"
#include "tim.h"
#include "drive.h" // Speed_t 정의 포함
#include "bldc_open_loop.h"

/* --- 설정 상수 (사용자 환경에 맞게 튜닝 필요) --- */
#define POLE_PAIRS      1.0f       // 모터 극쌍 수 (예: 14극 모터라면 7)
#define VBUS_VOLTAGE    7.4f      // 배터리 전압 (V) - ADC로 읽어오면 더 좋음
#define PWM_PERIOD      9999.f    // 타이머 Period (tim.c 설정값)
#define MAX_VOLTAGE     (VBUS_VOLTAGE * 0.8f) // 최대 출력 전압 제한
#define RAMP_RATE       1000.0f    // 속도 변경 가속도 (RPM/s)
#define MIN_START_V     1.0f       // 기동 시 최소 전압 (마찰 극복용)
#define V_PER_RPM       0.002f     // 속도에 따른 전압 증가량 (V/f 곡선 기울기)
#define DT              0.001f     // 제어 루프 주기 (예: 1kHz = 0.001s)

#define SQRT3_INV       0.57735026919f
#define TWO_PI          6.28318530718f

/* --- 모터 상태 구조체 --- */
typedef struct {
    TIM_HandleTypeDef *htim;      // 사용할 타이머 핸들
    float target_rpm;             // 목표 RPM
    float current_rpm;            // 현재 램프된 RPM
    float theta_e;                // 현재 전기각 (Electrical Angle)
    float voltage_mag;            // 출력 전압 크기
} BLDC_Motor_t;

/* 왼쪽, 오른쪽 모터 인스턴스 초기화 */
BLDC_Motor_t Motor_L = { .htim = &htim1, .theta_e = 0.0f };
BLDC_Motor_t Motor_R = { .htim = &htim8, .theta_e = 0.0f };

/* --- SVPWM 생성 함수 (3x PWM 모드 호환) --- */
// 입력: 전압 벡터 Valpha, Vbeta (Volts)
static void SVPWM_Generate(TIM_HandleTypeDef *htim, float Valpha, float Vbeta) {
    float Ta, Tb, Tc;
    float Vref = (2.0f / VBUS_VOLTAGE) * PWM_PERIOD; // 전압을 PWM 듀티로 변환하는 계수

    // 역 Clarke 변환 및 SVPWM 시간 계산
//    float X = Vbeta * Vref;
//    float Y = (Valpha * 0.5f + Vbeta * 0.8660254f) * Vref; // 0.866 = sqrt(3)/2
//    float Z = (Valpha * 0.5f - Vbeta * 0.8660254f) * Vref; // 계산 단순화

    // Sector 판단 없이 단순화된 SVPWM (Saddle Profile)
    // Common Mode Voltage 주입 방식 사용
    float Va = Valpha * Vref;
    float Vb = (-0.5f * Valpha + 0.8660254f * Vbeta) * Vref;
    float Vc = (-0.5f * Valpha - 0.8660254f * Vbeta) * Vref;

    float V_max = Va;
    if (Vb > V_max) V_max = Vb;
    if (Vc > V_max) V_max = Vc;

    float V_min = Va;
    if (Vb < V_min) V_min = Vb;
    if (Vc < V_min) V_min = Vc;

    float V_com = (V_max + V_min) * 0.5f;

    // 실제 PWM 듀티 계산 (Center Aligned 기준)
    Ta = (Va - V_com) + (PWM_PERIOD / 2.0f);
    Tb = (Vb - V_com) + (PWM_PERIOD / 2.0f);
    Tc = (Vc - V_com) + (PWM_PERIOD / 2.0f);

    // PWM 값 제한
    if (Ta > PWM_PERIOD) Ta = PWM_PERIOD; if (Ta < 0) Ta = 0;
    if (Tb > PWM_PERIOD) Tb = PWM_PERIOD; if (Tb < 0) Tb = 0;
    if (Tc > PWM_PERIOD) Tc = PWM_PERIOD; if (Tc < 0) Tc = 0;

    // 타이머 레지스터 설정
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (uint32_t)Ta);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (uint32_t)Tb);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (uint32_t)Tc);
}

/* --- 모터 제어 루프 (타이머 인터럽트에서 호출) --- */
static void BLDC_Update_Loop(BLDC_Motor_t *motor, float target_rpm) {
    float Valpha, Vbeta;

    // 1. 속도 램프 (가감속 제한)
    float rpm_diff = target_rpm - motor->current_rpm;
    float max_step = RAMP_RATE * DT;

    if (rpm_diff > max_step) motor->current_rpm += max_step;
    else if (rpm_diff < -max_step) motor->current_rpm -= max_step;
    else motor->current_rpm = target_rpm;

    // 2. 전기각 업데이트 (적분)
    // RPM to Hz = RPM / 60. Electrical Hz = Mechanical Hz * PolePairs
    float freq_e = (motor->current_rpm / 60.0f) * POLE_PAIRS;
    motor->theta_e += freq_e * TWO_PI * DT;

    // 각도 정규화 (0 ~ 2PI)
    if (motor->theta_e > TWO_PI) motor->theta_e -= TWO_PI;
    else if (motor->theta_e < 0.0f) motor->theta_e += TWO_PI;

    // 3. V/f 제어: 속도에 비례하여 전압 인가
    if (fabs(motor->current_rpm) < 10.0f) {
        motor->voltage_mag = 0.0f; // 정지
    } else {
        // 기본 전압 + 속도 비례 전압
        motor->voltage_mag = MIN_START_V + (fabs(motor->current_rpm) * V_PER_RPM);
        if (motor->voltage_mag > MAX_VOLTAGE) motor->voltage_mag = MAX_VOLTAGE;
    }

    // 4. 전압 벡터 생성 (회전)
    // 방향(Sign) 처리: RPM이 음수면 각도를 반대로 돌리는 대신 theta는 계속 증가시키고 위상을 반전
    // 하지만 위 코드에선 theta_e가 음수 주파수를 반영하므로 그대로 sin/cos 하면 됨.
    Valpha = motor->voltage_mag * cosf(motor->theta_e);
    Vbeta  = motor->voltage_mag * sinf(motor->theta_e);

    // 5. PWM 출력
    SVPWM_Generate(motor->htim, Valpha, Vbeta);
}

/* --- 메인에서 호출할 함수 --- */
void Motor_Control_Loop(void) {
    // drive.h의 mps 구조체 값을 목표로 사용
    // mps.ref.left, mps.ref.right가 목표 RPM이라고 가정

    BLDC_Update_Loop(&Motor_L, mps.ref.left);
    BLDC_Update_Loop(&Motor_R, mps.ref.right);
}

void Motor_Start(void) {
	HAL_TIM_Base_Start_IT(&htim7);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}
