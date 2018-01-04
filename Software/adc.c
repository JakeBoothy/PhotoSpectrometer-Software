#include "adc.h"
#include "fsl_adc16.h"
#include "fsl_edma.h"

#define ADC_TYPE ADC0

#define CHANNEL_NUMBER 7u
#define CHANNEL_GROUP 0u 
#define conversion_factor 1;


void adc_init()
{
    adc16_config_t adc16ConfigStruct;
	ADC16_GetDefaultConfig(&adc16ConfigStruct);
    
	adc16ConfigStruct.clockSource = kADC16_ClockSourceAlt0;
    adc16ConfigStruct.enableHighSpeed = true;
    adc16ConfigStruct.resolution = kADC16_ResolutionSE8Bit;
    adc16ConfigStruct.clockDivider = kADC16_ClockDivider2;
    adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
    adc16ConfigStruct.enableContinuousConversion = true;
	
	//Auto Calibration	
    
    adc16_channel_config_t adc16ChannelConfigStruct;
    adc16ChannelConfigStruct.channelNumber = CHANNEL_NUMBER; //Pin E19, ADC0_SE7
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; // Enable the interrupt.
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
    
    ADC16_Init(ADC_TYPE,&adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC_TYPE, false);
    ADC16_DoAutoCalibration(ADC_TYPE);

    ADC16_SetChannelConfig(ADC_TYPE,CHANNEL_GROUP,&adc16ChannelConfigStruct); // This sets the config but also starts a conversion?? This might not be necesary right now
    while (kADC16_ChannelConversionDoneFlag != ADC16_GetChannelStatusFlags(ADC0,CHANNEL_GROUP))
	{
	}
}


