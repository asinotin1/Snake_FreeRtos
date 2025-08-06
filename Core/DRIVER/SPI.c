#include "SPI.h"

void SPI1_Init_Master(void){
	GPIO_Config(PORT_SCK, PIN_SCK, GPIO_MODE_AF_PP);
	GPIO_Config(PORT_MOSI, PIN_MOSI, GPIO_MODE_AF_PP);
	GPIO_Config(PORT_MISO, PIN_MISO, GPIO_MODE_INPUT_FLOATING);
	GPIO_Config(PORT_CS, PIN_CS, GPIO_MODE_OUTPUT_PP);
	SPI1_CR1 = 0;
	SPI1_CR1 |= (1 << 2);     //Master
	SPI1_CR1 |= (0x05 << 3);  
	SPI1_CR1 |= (1 << 9);
	SPI1_CR1 |= (1 << 8);
	SPI1_CR1 |= (1 << 6);   //Enable SPI
	GPIO_Write_Pin(PORT_CS, PIN_CS, 1);
}

void SPI1_Send(uint8_t data){
	GPIO_Write_Pin(PORT_CS, PIN_CS, 0);
	while(!(SPI1_SR & (1 << 1))){}
	SPI1_DR = data;
	while(!(SPI1_SR & (1 << 0))){}
	uint32_t checkDataR = SPI1_DR;
	checkDataR++;
	checkDataR--;
	while(SPI1_SR & (1 << 7)){}
  GPIO_Write_Pin(PORT_CS, PIN_CS, 1);
}



