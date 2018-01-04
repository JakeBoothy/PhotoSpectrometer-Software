
#include "pin_mux.h"   
#include "gpio.h"

/* Define the init structure for the output LED pin*/
gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 0,
};

gpio_pin_config_t  high_config = {
    kGPIO_DigitalOutput, 1,
};

gpio_pin_config_t input_config = {
    kGPIO_DigitalInput, 0,
};

void gpio_init()
{
    GPIO_PinInit(LED_GPIO_PORT, LED0, &led_config);
    GPIO_PinInit(LED_GPIO_PORT, LED1, &led_config);
    GPIO_PinInit(LED_GPIO_PORT, BUTTON0, &input_config);
    
    GPIO_PinInit(LED_GPIO_PORT, fM_PIN_IDX, &high_config);
    GPIO_PinInit(LED_GPIO_PORT, SH_PIN_IDX, &high_config);
    
    GPIO_PinInit(LED_GPIO_PORT, ICG_PIN_IDX, &led_config);
}
