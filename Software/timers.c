
#include "fsl_pit.h"
#include "cmsis_os.h" 
#include "gpio.h"
#include "pin_mux.h"
#include "fsl_adc16.h"
#include "fsl_ftm.h"
#include "adc.h"
#include "uart.h"

#include <string.h> 
#include <stdio.h>

#define FM_FREQ 100000 // FM = 100kHz
#define FM_PER (10^6/FM_FREQ/2) //uS

#define INTEGRATION_TIME 50//uS must be larger than 10uS
#define SH_TIME (INTEGRATION_TIME/2)  //5//uS,
#define ICG_DELAY (6 + SH_TIME)//uS ideal ~From datasheet

#define MAX_PIX_COUNT 3694  //Pixel elements

/* 
Every INTEGRATION_TIME, PIT1 fires
After SH_TIME, PIT2 fires
After ICG_TIME, PIT3 fires
*/
 
unsigned char *gpiob_pdo = (unsigned char*)(GPIOB_BASE + GPIO_PDOR_PDO_SHIFT);
//uint8_t read_count = 0;
uint32_t pix_count = 0;
bool output = false;
bool icgNeeded = false;

uint8_t frame[MAX_PIX_COUNT];

char buff[50];
char value[6];

adc16_channel_config_t adc16ChannelConfigStruct;
   

void start_read()
{
    pix_count = 0;
    
    adc16ChannelConfigStruct.channelNumber = CHANNEL_NUMBER; //Pin E19, ADC0_SE7
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; // Enable the interrupt.
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
    
    PIT_StartTimer(PIT, kPIT_Chnl_0);//Start SH cycle
    PIT_StartTimer(PIT,kPIT_Chnl_2);//Start ICG cycle
    FTM_StartTimer(FTM0,kFTM_SystemClock);//Start main freq

}
 
void takeImage()
{
    output = true;
}

void Clocks_init()
{
    pit_config_t pitConfig;
    ftm_config_t ftmConfig;
    
    PIT_GetDefaultConfig(&pitConfig);
    FTM_GetDefaultConfig(&ftmConfig);
    
    FTM_Init(FTM0,&ftmConfig);    
    FTM_Init(FTM1,&ftmConfig);    
    
    ftm_chnl_pwm_signal_param_t ftmParam;
    ftm_pwm_level_select_t pwmLevel = kFTM_LowTrue;

    /* Configure ftm params with frequency 24kHZ */
    ftmParam.chnlNumber = kFTM_Chnl_3;
    ftmParam.level = pwmLevel;
    ftmParam.dutyCyclePercent = 50u;
    ftmParam.firstEdgeDelayPercent = 0U;
    
    FTM_SetupPwm(FTM0,&ftmParam,1u,kFTM_CenterAlignedPwm,800000,CLOCK_GetFreq(kCLOCK_BusClk));
        
    pitConfig.enableRunInDebug = true;

    PIT_Init(PIT, &pitConfig);
    
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, 100);//SH Pulse Width 9 = 200Khz, 224 = 80 Khz
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 22000);//SH Period
    //SH Period must be a factor of ICG period!!
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, 600);//ICG Pulse Width
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, 664130);//ICG Period   Uses bus clock at 35.99Mhz
    
    
    FTM_SetTimerPeriod(FTM1,175);    
    
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0,kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1,kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2,kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_3,kPIT_TimerInterruptEnable);
    
    FTM_EnableInterrupts(FTM1,kFTM_TimeOverflowInterruptEnable);

    
    /* Enable at the NVIC */
    EnableIRQ(PIT0_IRQn);
    EnableIRQ(PIT1_IRQn);
    EnableIRQ(PIT2_IRQn);
    EnableIRQ(PIT3_IRQn);
    
    EnableIRQ(FTM1_IRQn);
}

void outputData()
{
    sprintf(buff,"Starting frame\r\n");
    uart_write((uint8_t*)buff,sizeof(buff));
    for (uint32_t i = 0; i < sizeof(frame); i++)
    {
        sprintf(value,"%i\r\n",frame[i]);
        uart_write((uint8_t*)value,sizeof(value));
    }
    sprintf(buff,"Frame finished\r\n");
    uart_write((uint8_t*)buff,sizeof(buff));
}

//SH Pulse Low
void PIT0_IRQHandler(void)
{
    PIT_ClearStatusFlags(PIT,kPIT_Chnl_0,1u); 
    GPIO_SetPinsOutput(GPIOB,1u<<SH_PIN_IDX);
    PIT_StartTimer(PIT,kPIT_Chnl_1);
    PIT_StopTimer(PIT,kPIT_Chnl_0);
}

//SH Pulse High
void PIT1_IRQHandler(void)
{
    PIT_ClearStatusFlags(PIT,kPIT_Chnl_1,1u); 
    GPIO_ClearPinsOutput(GPIOB,1u<<SH_PIN_IDX);
    if (icgNeeded)
    {
        icgNeeded = false;
        GPIO_SetPinsOutput(GPIOB,1u<<ICG_PIN_IDX);
        PIT_StartTimer(PIT,kPIT_Chnl_2);
    }
    PIT_StartTimer(PIT,kPIT_Chnl_0);
    PIT_StopTimer(PIT,kPIT_Chnl_1);
}

//ICG Pulse High 
void PIT2_IRQHandler(void)
{
    PIT_ClearStatusFlags(PIT,kPIT_Chnl_2,1u); 
    GPIO_ClearPinsOutput(GPIOB,1u<<ICG_PIN_IDX);
    PIT_StartTimer(PIT,kPIT_Chnl_3);
    PIT_StopTimer(PIT,kPIT_Chnl_2);//Stop this timer after its done
    if (output == true)
    {
        //If we want to start a read, start the reading timer
        output = false;
        FTM_StartTimer(FTM1,kFTM_SystemClock);
    }
}

//ICG Pulse Low
void PIT3_IRQHandler(void)
{
    PIT_ClearStatusFlags(PIT,kPIT_Chnl_3,1u); 
    icgNeeded = true;
}


void FTM1_IRQHandler(void)
{
    FTM_ClearStatusFlags(FTM1,kFTM_TimeOverflowFlag);
    frame[pix_count] = ADC16_GetChannelConversionValue(ADC_TYPE,CHANNEL_GROUP);
    pix_count++;
    if (pix_count >= MAX_PIX_COUNT)
    {
        pix_count = 0;
        //Stop Reads
        FTM_StopTimer(FTM1);
        
        //Stop ICG and SH pulses
        PIT_StopTimer(PIT,kPIT_Chnl_0);
        PIT_StopTimer(PIT,kPIT_Chnl_1);
        PIT_StopTimer(PIT,kPIT_Chnl_2);
        PIT_StopTimer(PIT,kPIT_Chnl_3);
        
        //Even stop the fM pulses
        FTM_StopTimer(FTM0);
        
        outputData();
        
        Clocks_init();
        start_read();
    }
}

void ADC0_IRQHandler(void)
{    // Read the conversion result to clear the conversion completed flag.
    //frame[pix_count] = ADC16_GetChannelConversionValue(ADC_TYPE, CHANNEL_GROUP);
  //  pix_count++;
}
