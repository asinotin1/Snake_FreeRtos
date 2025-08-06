#include "TFT.h"


void writeCMDTFT(uint8_t cmd){
	GPIO_Write_Pin(PORT_CS, PIN_CS, 0); // CS = 0
	GPIO_Write_Pin(PORT_A0, PIN_A0, 0); // A0 = 0 -> CMD
	SPI1_Send(cmd);
	GPIO_Write_Pin(PORT_CS, PIN_CS, 1); // CS = 1
}

void writeDataTFT(uint8_t data){
	GPIO_Write_Pin(PORT_CS, PIN_CS, 0);
	GPIO_Write_Pin(PORT_A0, PIN_A0, 1); // A0 = 1 -> DATA
	SPI1_Send(data);
	GPIO_Write_Pin(PORT_CS, PIN_CS, 1);
}

void sendCMDList(const uint8_t* cmdList){
	uint8_t index = 0;
	uint8_t cmd = 0;
	uint8_t num = 0;
	
	while(1){
		cmd = *cmdList++;
		num = *cmdList++;
		if(cmd == LCD_CMD_END){
			break;
		}else{
			writeCMDTFT(cmd);
			for(index = 0; index < num ; index++){
				writeDataTFT(*cmdList++);
			}
		}
	}
}

void setPos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	writeCMDTFT(0x2A);
	writeDataTFT(0x00);
	writeDataTFT(x1);
	writeDataTFT(0x00);
	writeDataTFT(x2);
	writeCMDTFT(0x2B);
	writeDataTFT(0x00);
	writeDataTFT(y1);
	writeDataTFT(0x00);
	writeDataTFT(y2);
}
//RGB 16bit 565
void fullDisplay(uint16_t color){
	writeCMDTFT(0x2C);
	int i;
	for(i = 0 ; i < 128*160;i++){
		writeDataTFT(color & 0xFF);
		writeDataTFT(color >> 8);
	}
}


void drawPixel(uint8_t x, uint8_t y, uint16_t color){
	if(x >= 128 || y >= 160){
		return ;
	}
	setPos(x,y,x+1,y+1);
	writeCMDTFT(0x2C);
	writeDataTFT(color & 0xFF);
	writeDataTFT(color >> 8);
}

void initTFT(void){
	GPIO_Config(PORT_A0, PIN_A0, GPIO_MODE_OUTPUT_PP);
	GPIO_Config(PORT_CS, PIN_CS, GPIO_MODE_OUTPUT_PP);
	GPIO_Config(PORT_RESET, PIN_RESET, GPIO_MODE_OUTPUT_PP);
	// HW RESET
	GPIO_Write_Pin(PORT_RESET, PIN_RESET,0);
	delay_ms(20);
	GPIO_Write_Pin(PORT_RESET, PIN_RESET,1);
	delay_ms(150);
	//SW RESET
	writeCMDTFT(0x01);
	delay_ms(150);
	// Sleep out
	writeCMDTFT(0x11);
	delay_ms(255);
	
	sendCMDList(u8InitCmdList);
	
	writeCMDTFT(0x36); // Memory Data Access Control 
	writeDataTFT(0x08);
	
	writeCMDTFT(0x3A); // Interface Pixel Format
	writeDataTFT(0x05);
	
	writeCMDTFT(0x20); // Display inversion off 
	
	setPos(0,0,128,160);
	
	// EN DISPLAY
	writeCMDTFT(0x29);
	delay_ms(100);

}


void drawChar(uint8_t x, uint8_t y, char ch, FontDef font, uint16_t color, uint16_t bg){
	uint8_t i,j;
	uint16_t pixelData;
	for( i = 0; i < font.height ;i++ ){
		pixelData = font.data[(ch - 32)*font.height + i];
		for(j = 0 ; j < font.width ;j++){
			if( (pixelData << j) & 0x8000 ){
				drawPixel(x+j,y+i,color);
			}else{
				drawPixel(x+j,y+i,bg);
			}
		}
	}
}


void drawString(uint8_t x, uint8_t y, char *str, FontDef font, uint16_t color, uint16_t bg){
	while(*str){
		drawChar(x,y,*str,font,color,bg);
		x+= font.width;
		str++;
	}
}

