#include <stdio.h>
#include "button.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

void button_init(void)
{
    gpio_config_t io_conf = {};
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;

    // Botón 1
    io_conf.pin_bit_mask = 1ULL << GPIO_BUTTON_1;
    gpio_config(&io_conf);

    // Botón 2
    io_conf.pin_bit_mask = 1ULL << GPIO_BUTTON_2;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
}


button_state_t button_get1(void)
{
        uint8_t pin_level;

        pin_level = gpio_get_level(GPIO_BUTTON_1);
        if (pin_level == 0)
        {
            button_state_t boton = BUTTON_PRESSED;
            return boton ;
        }
        else
        {
            button_state_t boton = BUTTON_RELEASED;
            return boton;
        }
}

button_state_t button_get2(void)
{
        uint8_t pin_level;

        pin_level = gpio_get_level(GPIO_BUTTON_2);
        if (pin_level == 0)
        {
            button_state_t boton = BUTTON_PRESSED;
            return boton ;
        }
        else
        {
            button_state_t boton = BUTTON_RELEASED;
            return boton;
        }
}