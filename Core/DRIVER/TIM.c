#include "TIM.h"
#include "EXTI.h"


void TIM2_init_IT(void){
	TIM2_PSC = 7999;
	TIM2_ARR = 999;
	TIM2_DIER |= (1 << 0);
	TIM2_CR1 |= (1 << 0);
	NVIC_ISER0 |= (1 << 28);
}

void TIM2_IRQHandler(void){
	if(TIM2_SR & 0x01){
		//doing sth
		TIM2_SR &= ~(uint32_t)(1 << 0);
	}
}

void delay_ms(uint32_t ms){
	TIM2_PSC = 7999;
	TIM2_ARR = ms;
	TIM2_CNT = 0;
	TIM2_CR1 |= (1 << 0);
	while(TIM2_CNT < ms){}
	TIM2_CR1 &= ~(uint32_t)(1 << 0);
	TIM2_CNT = 0;
}
