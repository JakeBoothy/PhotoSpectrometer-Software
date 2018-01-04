/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK22FN512xxx12'
- !!package 'MK22FN512VLH12'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.1'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
  - {pin_num: '23', peripheral: GPIOA, signal: 'GPIO, 1', pin_signal: PTA1/UART0_RX/FTM0_CH6/JTAG_TDI/EZP_DI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/


 
void BOARD_InitPins(void) {
    
    CLOCK_EnableClock(kCLOCK_PortC);                           /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);                           /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);                           /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_Pit0);
    CLOCK_EnableClock(kCLOCK_Ftm0);
    
    PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAsGpio);          /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTB, BUTTON_IDX, kPORT_MuxAsGpio);        /* PORTA1 (pin 23) is configured as PTA1 */
    
    PORT_SetPinMux(PORTC, 4u, kPORT_MuxAlt4);
    PORT_SetPinMux(PORTB, fM_PIN_IDX, kPORT_PinDisabledOrAnalog);        /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTB, SH_PIN_IDX, kPORT_MuxAsGpio);        /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTB, ICG_PIN_IDX, kPORT_MuxAsGpio);       /* PORTA1 (pin 23) is configured as PTA1 */

    PORT_SetPinMux(PORTE, PIN_RX, kPORT_MuxAlt3);          /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTE, PIN_TX, kPORT_MuxAlt3);          /* PORTA1 (pin 23) is configured as PTA1 */
    PORT_SetPinMux(PORTE, SPEC_IN_IDX, kPORT_PinDisabledOrAnalog);          /* PORTA1 (pin 23) is configured as PTA1 */

}

/*******************************************************************************
 * EOF
 ******************************************************************************/
