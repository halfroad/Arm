#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

#include "AnalogDigitalConvertor.h"

void init_adc_port(uint32_t pin)
{
	/* Enable Clock. https://blog.csdn.net/chen244798611/article/details/50154909 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialize the GPIOA.1 port. */
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = pin;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AN;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}

void init_adc_input(void)
{
	/* Enable ADC1 clock. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* Configurations of ADC generic controls. */
	ADC_CommonInitTypeDef adc_common_init_struct;
	
	adc_common_init_struct.ADC_Mode = ADC_Mode_Independent;
	/* Delay 5 cycles between 2 samples. */
	adc_common_init_struct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	/* No DMA. */
	adc_common_init_struct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	/* 4 divided, ADCCLK (ADC CLOCK) = PCLK2 / 4 = 84MHz / 4 = 21MHz, ADC CLOCK should not exceed 36MHz. */
	adc_common_init_struct.ADC_Prescaler = ADC_Prescaler_Div4;
	
	ADC_CommonInit(&adc_common_init_struct);
	
	/* Configure ADC1 channel. */
	ADC_InitTypeDef adc_init_struct;
	
	adc_init_struct.ADC_Resolution = ADC_Resolution_12b;
	adc_init_struct.ADC_ScanConvMode = DISABLE;
	adc_init_struct.ADC_ContinuousConvMode = DISABLE;
	/* Disallow the external trigger detection, use software trigger instead. */
	adc_init_struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	/* Right alignment. */
	adc_init_struct.ADC_DataAlign = ADC_DataAlign_Right;
	
	/* 1 conversion in the regular sequence, that is only the regular sequence 1 would be converted. */
	adc_init_struct.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &adc_init_struct);
	
	/* Set the channel of ADC regular group, 1 sequence and sampling time. */
	/* ADC1, ADC channel, 480 cycles, the longer the cycles time, the better accuracy. */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
}

uint16_t acquire_adc_result(void)
{
	/* Enable the software conversion on ADC1. */
	ADC_SoftwareStartConv(ADC1);
	
	/* Wait for the conversion completion. */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;
	
	/* Return the latest result of  ADC1 regular conversion group. */
	return ADC_GetConversionValue(ADC1);
}