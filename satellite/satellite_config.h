// Tamariw configuration file
// 2024-04-13

#ifndef _SATELLITE_CONFIG_H_
#define _SATELLITE_CONFIG_H_

// ToF sensor
#define TOF_I2C_HAL_IDX I2C_IDX1
#define TOF_I2C_HAL_GPIO_SCL GPIO_022
#define TOF_I2C_HAL_GPIO_SDA GPIO_023
#define TOF_I2C_ADDRESS 0x29
#define TOF_I2C_MUX_ADDRESS 0x70
#define TOF_CALIBRATE_TARGET_MM 100
#define TOF_CALIBRATE_SAMPLES 20

// Electromagnet enable pin
#define EM_ENABLE_PIN GPIO_056

// PWMs for electromagnets
#define EM_PWM_FREQUENCY 15000
#define EM_PWM_INCREMENTS 1000
#define EM1_PWM_IDX PWM_IDX04
#define EM2_PWM_IDX PWM_IDX05
#define EM3_PWM_IDX PWM_IDX06
#define EM4_PWM_IDX PWM_IDX07

// Direction pins for electromagnets
#define EM1_PIN_IN1 GPIO_063
#define EM1_PIN_IN2 GPIO_062
#define EM2_PIN_IN1 GPIO_061
#define EM2_PIN_IN2 GPIO_060
#define EM3_PIN_IN1 GPIO_076
#define EM3_PIN_IN2 GPIO_075
#define EM4_PIN_IN1 GPIO_074
#define EM4_PIN_IN2 GPIO_073

// ADC channels for electromagnets
#define EM1_ADC_CH ADC_CH_010
#define EM2_ADC_CH ADC_CH_011
#define EM3_ADC_CH ADC_CH_012
#define EM4_ADC_CH ADC_CH_013

#endif // satellite_config.h
