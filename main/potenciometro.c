#include "potenciometro.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define ADC_WIDTH       ADC_WIDTH_BIT_12   // Resolución de 12 bits
#define MAX_ADC_VALUE   4095
#define VREF            1100               // Referencia en mV, si se usa calibración

// Configuración fija del canal
#define ADC_CHANNEL_USED   ADC_CHANNEL_4   // GPIO5 en ESP32-S3

void potenciometro_init(void)
{
    // Configura el ancho del ADC
    adc1_config_width(ADC_WIDTH);

    // Configura el canal con atenuación para permitir 0-3.3V
    adc1_config_channel_atten(ADC_CHANNEL_USED, ADC_ATTEN_DB_11);
}

float potenciometro_get_valor(void)
{
    int raw = adc1_get_raw(ADC_CHANNEL_USED);
    
    // Normaliza de 0.0 a 100.0
    float valor = ((float)raw * 100.0f) / MAX_ADC_VALUE;

    if (valor > 100.0f) valor = 100.0f;
    if (valor < 0.0f) valor = 0.0f;

    return valor;
}
