#include "ADC.h"
#include "TIM.h"

void ADC01_CH0_Init(void){
	ADC01_SMPR2 &= ~(uint32_t)(0x7 << 0);
	ADC01_SMPR2 |= (1 << 1);
	ADC01_SQR3 = 0;
	ADC01_CR2 |= (1 << 1);
	ADC01_CR2 |= (1 << 0); //ADON
	delay_ms(1);
	ADC01_CR2 |= (1 << 0);
	
	ADC01_CR2 |= (1 << 3);
	while(ADC01_CR2 & (1 << 3)){}
	
	ADC01_CR2 |= (1 << 2);
	while(ADC01_CR2 & (1 << 2)){}
		
	ADC01_CR2 |= (1 << 22);
}

uint16_t ADC01_CH0_Read(void){
	while(!(ADC01_SR & (1 << 1))){
	}
	return (uint16_t)(ADC01_DR & 0xFFFF);
}



