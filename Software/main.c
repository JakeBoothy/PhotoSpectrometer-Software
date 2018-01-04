/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define osObjectsPublic                     // define objects in main module
#include "cmsis_os.h" 

#include "board.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "gpio.h"
#include "uart.h"
#include "fsl_uart.h"
#include "adc.h"
#include "timers.h"


#include <string.h> 
#include <stdio.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {           // Add HAL_GetTick function for STM32Cube HAL
  return os_time; 
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
char test[50];
enum states
{
    null,
    Startup,
    Imaging
};
enum states state;

uint32_t button_input;
uint8_t uart_input;

osThreadId state_logic_id;
osThreadId main_id;
/*******************************************************************************
 * Code
 ******************************************************************************/

void state_logic(void const *argument)
{
    state = Startup;
    while (1)
    {
        switch (state)
        {
            case NULL:
                break;
            case Startup:
                osDelay(100);
                button_input = GPIO_ReadPinInput(GPIOB, BUTTON0);
                uart_input = uart_read_byte();
                if (!button_input || uart_input == 'i')
                {
                    GPIO_SetPinsOutput(GPIOB, 1u << LED0);
                    GPIO_SetPinsOutput(GPIOB, 1u << LED1);
                    osDelay(100);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED0);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED1);
                    osDelay(100);
                    GPIO_SetPinsOutput(GPIOB, 1u << LED0);
                    GPIO_SetPinsOutput(GPIOB, 1u << LED1);
                    osDelay(100);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED0);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED1);
                    osDelay(100);
                    GPIO_SetPinsOutput(GPIOB, 1u << LED0);
                    GPIO_SetPinsOutput(GPIOB, 1u << LED1);
                    osDelay(100);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED0);
                    GPIO_ClearPinsOutput(GPIOB, 1u << LED1);
                    state = Imaging;
                }
                else
                {
                }
                break;
            case Imaging:
                
                //reading = adc_read();
                //sprintf(test,"ADC Reading: %f\r\n",reading);
                uart_write((uint8_t*)test,sizeof(test));              
                osDelay(100);
            
                break;
        }
            
        //GPIO_TogglePinsOutput(GPIOB, 1u << LED0);
        //GPIO_TogglePinsOutput(GPIOB, 1u << LED1);
        //sprintf(test,"Output: %i\r\n",x);
        //uart_write((uint8_t*)test,sizeof(test));
        
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    uart_init();
    gpio_init();
    adc_init();
    Clocks_init();

    //osKernelInitialize ();               // Initialize RTOS Kernel for setup    
    //osKernelStart (); 
    //main_id = osThreadGetId(); //Get the main thread ID, its always useful
    
    //osThreadDef(state_logic,osPriorityNormal,1,0);
    //state_logic_id = osThreadCreate(osThread(state_logic),NULL);
    
    start_read();
    
    while(1)
    {
        button_input = GPIO_ReadPinInput(GPIOB, BUTTON0);
        //uart_input = uart_read_byte();
        if (!button_input)// ||  uart_input == 'i')
        {
            //*(volatile uint32_t *)(UART1_BASE+0x4) = 0;
            //UART_ClearStatusFlags(UART0,kUART_
            takeImage();
        }
    }
    
    //osThreadTerminate(main_id);    
}
