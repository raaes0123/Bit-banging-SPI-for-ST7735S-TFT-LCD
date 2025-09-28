#pragma once

#define PIPE_WIDTH 20
#define PIPE_HEIGHT 100
#define HEADER_HEIGHT 40
#define PIPE_GAP 40

#define COLOR_PIPE 0x03E0
#define COLOR_SKY_BLUE 0x05FF
#define COLOR_DEEP_SKY_BLUE 0x867D

void setPin(uint8_t pin);
void clrPin(uint8_t pin);
void spiWrite(uint8_t byte);
void spiWrite12bit(uint16_t byte);

void ST7735_Reset();
void ST7735_Command(uint8_t com);
void ST7735_Data(uint8_t data);
void ST7735S_Init(void);
void ST7735S_SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void ST7735S_FillScreen(uint16_t color);