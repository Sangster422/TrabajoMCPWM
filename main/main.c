#include <stdio.h>
#include <stdint.h>

#include "servo.h"
#include "motor.h"
#include "potenciometro.h"
#include "button.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define DEBOUNCE_TIME_MS 300

volatile bool boton_onoff_presionado = false;
volatile int boton_angulo_presionado = 0;

static volatile uint32_t last_debounce_time_onoff = 0;
static volatile uint32_t last_debounce_time_angulo = 0;

static portMUX_TYPE button_count_access = portMUX_INITIALIZER_UNLOCKED;

void servo_task(void *pvParameter)
{

    bool maquina_encendida = true;
    int angulos[] = {30, 60, 90};
    int indice_angulo = 0;
    int indice_angulo_anterior = 0;

    for (;;)
    {   
        taskENTER_CRITICAL(&button_count_access);
        maquina_encendida = boton_onoff_presionado;
        indice_angulo = boton_angulo_presionado;
        taskEXIT_CRITICAL(&button_count_access);

        if (maquina_encendida && indice_angulo != indice_angulo_anterior)
        {
            int angulo = angulos[indice_angulo];
            servo_move_to_angle(angulo);
            printf("Servo movido a %d grados\n", angulo);
            indice_angulo_anterior = indice_angulo;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


void motor_task(void *pvParameter)
{

    bool maquina_encendida = true;

    for(;;) 
    {   
        taskENTER_CRITICAL(&button_count_access);
        maquina_encendida = boton_onoff_presionado;
        taskEXIT_CRITICAL(&button_count_access);

        if (maquina_encendida)
        {
        float valor = potenciometro_get_valor();;
        motor_forward(valor); 
        printf("Motor: %.2f%%\n", valor);
        vTaskDelay(pdMS_TO_TICKS(100));
        }else{
           motor_forward(0); 
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void IRAM_ATTR gpio_isr_handler1(void* arg)
{
    uint32_t current_time = xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;

    if (current_time - last_debounce_time_onoff > DEBOUNCE_TIME_MS) 
    {
        portENTER_CRITICAL_ISR(&button_count_access);
        boton_onoff_presionado = !boton_onoff_presionado;
        if (!boton_onoff_presionado){
            boton_angulo_presionado = 0;
        }
        portEXIT_CRITICAL_ISR(&button_count_access);

        last_debounce_time_onoff = current_time;
    }
}

static void IRAM_ATTR gpio_isr_handler2(void* arg)
{
    uint32_t current_time = xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;

    if (current_time - last_debounce_time_angulo > DEBOUNCE_TIME_MS)
    {
        portENTER_CRITICAL_ISR(&button_count_access);
        if (boton_onoff_presionado){
            boton_angulo_presionado = (boton_angulo_presionado + 1) % 3;
        }
        portEXIT_CRITICAL_ISR(&button_count_access);

        last_debounce_time_angulo = current_time;
    }
}

void app_main(void)
{
    
    servo_initialize();
    motor_initialize();
    potenciometro_init();
    button_init();
    gpio_isr_handler_add(GPIO_BUTTON_1, gpio_isr_handler1, (void*) GPIO_BUTTON_1);
    gpio_isr_handler_add(GPIO_BUTTON_2, gpio_isr_handler2, (void*) GPIO_BUTTON_2);

    xTaskCreate(servo_task, "servo_task", 2048, NULL, 5, NULL);
    xTaskCreate(motor_task, "motor_task", 2048, NULL, 5, NULL);

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
