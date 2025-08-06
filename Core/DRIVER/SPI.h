#ifndef __SPI_H
#define __SPI_H
#include <stdint.h>
#include "GPIO.h"

#define PORT_SCK       GPIOA
#define PORT_MISO      GPIOA
#define PORT_MOSI      GPIOA
#define PORT_CS				 GPIOB

#define PIN_SCK        GPIO_PIN_5
#define PIN_MISO       GPIO_PIN_6
#define PIN_MOSI       GPIO_PIN_7
#define PIN_CS		     GPIO_PIN_0
 

#define SPI1_BASE_ADDR     0x40013000


#define SPI_CR1_OFFSET		  0x00
#define SPI_CR2_OFFSET		  0x04
#define SPI_SR_OFFSET		    0x08
#define SPI_DR_OFFSET		    0x0C
#define SPI_CRCPR_OFFSET		0x10
#define SPI_RXCRCR_OFFSET		0x14
#define SPI_TXCRCR_OFFSET		0x18
#define SPI_I2SCFGR_OFFSET  0x1C
#define SPI_I2SPR_OFFSET		0x20

#define SPI1_CR1    *((volatile uint32_t* )(SPI1_BASE_ADDR + SPI_CR1_OFFSET))
#define SPI1_CR2    *((volatile uint32_t* )(SPI1_BASE_ADDR + SPI_CR2_OFFSET))
#define SPI1_SR     *((volatile uint32_t* )(SPI1_BASE_ADDR + SPI_SR_OFFSET))
#define SPI1_DR     *((volatile uint32_t* )(SPI1_BASE_ADDR + SPI_DR_OFFSET))



void SPI1_Init_Master(void);
void SPI1_Send(uint8_t data);


#endif
