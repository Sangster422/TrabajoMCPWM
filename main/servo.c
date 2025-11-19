#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define SERVO_MIN_PULSEWIDTH 350  // Ancho de pulso ángulo mínimo
#define SERVO_MAX_PULSEWIDTH 1230   // Ancho de pulso ángulo máximo
#define SERVO_MAX_DEGREE 90       // Ángulo máximo
#define SERVO_MIN_DEGREE 0        // Ángulo mínimo

#define GPIO_SERVO_OUTPUT 0

static const char *TAG = __FILE__;

void servo_initialize(void) {
    ESP_LOGI(TAG, "Inicializando servomotor");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_SERVO_OUTPUT);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;           // frecuencia = 50Hz --> 20ms
    pwm_config.cmpr_a = 0;               // ciclo de trabajo de PWMxA = 0
    pwm_config.cmpr_b = 0;               // ciclo de trabajo de PWMxB = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;  // comienza en 0, asciende a 1
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;    // funcionamiento a nivel alto

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config); // configuración PWM
}
// Función para mover servo a ángulo fijo (30, 60, 80)
void servo_move_to_angle(int32_t angle) {
    if (angle < SERVO_MIN_DEGREE) angle = SERVO_MIN_DEGREE;
    if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;

    int32_t pulsewidth = SERVO_MIN_PULSEWIDTH + ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) / SERVO_MAX_DEGREE;

    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, pulsewidth);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

