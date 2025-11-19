#include <stdio.h>
#include "esp_log.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "motor.h"
#include "sdkconfig.h"

#define GPIO_MOTOR1_PWM 15  // Motor 1 en GPIO 15 (MCPWM0A)
#define GPIO_MOTOR2_PWM 16  // Motor 2 en GPIO 16 (MCPWM0B)

static const char *TAG = __FILE__;

void motor_initialize(void)
{
    ESP_LOGI(TAG, "Inicializando motor");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_MOTOR1_PWM);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_MOTOR2_PWM);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    //frecuencia = 1000Hz --> 1ms
    pwm_config.cmpr_a = 0;    //ciclo de trabajo de PWMxA = 0
    pwm_config.cmpr_b = 0;    //ciclo de trabajo PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;     //Comienza en 0, asciende a 1
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;       //Funcionamiento a nivel alto
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configuración de PWM0A y PWM0B
}

void motor_forward(float duty_cycle)
{
    //ESP_LOGI(TAG, "Moviendo motor");
    
    // Motor 1 adelante
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);

    // Motor 2 adelante
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}

void motor_stop()
{
    //ESP_LOGI(TAG, "Motor parado");
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);     //Ponemos a 0 outputA
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);     //Ponemos a 0 outputB
}