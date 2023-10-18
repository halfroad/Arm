#include "gpio.h"
#include "PhotoResistor.h"

void init_photo_resistor_input(void)
{
	init_gpio();
	
	/* Enable the ADC1 clock on APB2. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* ADC Common settings. */
	ADC_CommonInitTypeDef adc_common_init_type;
	
	adc_common_init_type.ADC_Mode = ADC_Mode_Independent;
	/* 4 Divisions, ADCCLK = PCLK2 / 4 = 84 / 4 = 21MHz. ADC clock had better not greater than 36MHz. */
	adc_common_init_type.ADC_Prescaler = ADC_Prescaler_Div4;
	/* Disable the DMA. */
	adc_common_init_type.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	/* 5 cycles delay between 2 sampling. */
	adc_common_init_type.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	ADC_CommonInit(&adc_common_init_type);
	
	/* Initialize the ADC. */
	
	ADC_InitTypeDef adc_init_type;
	
	adc_init_type.ADC_Resolution = ADC_Resolution_12b;
	/* No Scan Conversion mode. */
	adc_init_type.ADC_ScanConvMode = DISABLE;
	/* No Continuous Conversion mode. */
	adc_init_type.ADC_ContinuousConvMode = DISABLE;
	/* Disallow the trigger detection, will be triggered by software. */
	adc_init_type.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_type.ADC_DataAlign = ADC_DataAlign_Right;
	/* 1 conversion in the regular sequence. */
	adc_init_type.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &adc_init_type);
	
	/* Set the regular channel for the specific ADC, one sequence, sampling time.
	ADC1, ADC channel, 480 cycles. Accurate the sampling by prelonging the sampling time. */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
	
	/* Wait for the completion of calibration, but not available on STM32F4xx.
	ADC_StartCalibration(ADC1);
	
	while(ADC_GetCalibrationStatus(ADC1))
		;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	*/
}

uint16_t get_analog_digital_convertor_result(void)
{
	/* Enable the sofeware conversion on ADC1. */
	ADC_SoftwareStartConv(ADC1);
	
	/* Wait for the completion of conversion. */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	
	/* Return the latest conversion value for the given regular group.*/
	return ADC_GetConversionValue(ADC1);
}