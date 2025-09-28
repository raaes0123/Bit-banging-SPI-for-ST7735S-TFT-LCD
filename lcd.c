#include <stdint.h>
#include "lcd.h"
#include "main.h"


void setPin(uint8_t pin){
    GPIOB_ODR |= 1 << pin;
}

void clrPin(uint8_t pin){
    GPIOB_ODR &= ~(1 << pin);
}

void spiWrite(uint8_t byte){
    for(uint8_t i = 0 ; i < 8 ;i++){
        clrPin(SCK);
        (byte & 0x80) ? setPin(MOSI) : clrPin(MOSI);
        byte <<= 1;
        delay_us(10);
        setPin(SCK);
        delay_us(10);
    }
}

void spiWrite12bit(uint16_t byte){
    for(uint8_t i = 0 ; i < 12 ;i++){
        clrPin(SCK);
        (byte & 0x800) ? setPin(MOSI) : clrPin(MOSI);
        byte <<= 1;
        delay_us(1);
        setPin(SCK);
        delay_us(1);
    }
}

void ST7735_Reset(){
    clrPin(RST);
    delay_ms(50);
    setPin(RST);
    delay_ms(50);
}

void ST7735_Command(uint8_t com){
    clrPin(CS);         // CS
    clrPin(DCX);        // Command Mode On
    spiWrite(com);      // SPI_Write
    setPin(CS);         // CS off
}

void ST7735_Data(uint8_t data){
    clrPin(CS);         // CS
    setPin(DCX);        // Command Mode On
    spiWrite(data);      // SPI_Write
    setPin(CS);         // CS off
}

void ST7735S_Init(void) {
    ST7735_Reset();         // Toggle RST pin
    delay_ms(150);

    ST7735_Command(0x11);   // Sleep out
    delay_ms(120);

    ST7735_Command(0xB1);   // Frame rate control (normal mode)
    ST7735_Data(0x05);
    ST7735_Data(0x3A);
    ST7735_Data(0x3A);

    ST7735_Command(0xB2);   // Frame rate control (idle)
    ST7735_Data(0x05);
    ST7735_Data(0x3A);
    ST7735_Data(0x3A);

    ST7735_Command(0xB3);   // Frame rate control (partial)
    ST7735_Data(0x05);
    ST7735_Data(0x3A);
    ST7735_Data(0x3A);
    ST7735_Data(0x05);
    ST7735_Data(0x3A);
    ST7735_Data(0x3A);

    ST7735_Command(0xB4);   // Display inversion control
    ST7735_Data(0x03);

    ST7735_Command(0xC0);   // Power control 1
    ST7735_Data(0x62);
    ST7735_Data(0x02);
    ST7735_Data(0x04);

    ST7735_Command(0xC1);   // Power control 2
    ST7735_Data(0xC0);

    ST7735_Command(0xC2);   // Power control 3
    ST7735_Data(0x0D);
    ST7735_Data(0x00);

    ST7735_Command(0xC3);   // Power control 4
    ST7735_Data(0x8D);
    ST7735_Data(0x6A);

    ST7735_Command(0xC4);   // Power control 5
    ST7735_Data(0x8D);
    ST7735_Data(0xEE);

    ST7735_Command(0xC5);   // VCOM control
    ST7735_Data(0x0E);

    ST7735_Command(0x36);   // Memory Access Control
    ST7735_Data(0xC8);      // MX, MY, RGB (0xC8) or BGR (0xC0) ???

    ST7735_Command(0x3A);   // Color Mode
    ST7735_Data(0x03);      // 12-bit/pixel

    ST7735_Command(0xE0);   // Gamma Correction (positive)
    ST7735_Data(0x10);
    ST7735_Data(0x0E);
    ST7735_Data(0x02);
    ST7735_Data(0x03);
    ST7735_Data(0x0E);
    ST7735_Data(0x07);
    ST7735_Data(0x02);
    ST7735_Data(0x07);
    ST7735_Data(0x0A);
    ST7735_Data(0x12);
    ST7735_Data(0x27);
    ST7735_Data(0x37);
    ST7735_Data(0x00);
    ST7735_Data(0x0D);
    ST7735_Data(0x0E);
    ST7735_Data(0x10);

    ST7735_Command(0xE1);   // Gamma Correction (negative)
    ST7735_Data(0x10);
    ST7735_Data(0x0E);
    ST7735_Data(0x03);
    ST7735_Data(0x03);
    ST7735_Data(0x0F);
    ST7735_Data(0x06);
    ST7735_Data(0x02);
    ST7735_Data(0x08);
    ST7735_Data(0x0A);
    ST7735_Data(0x13);
    ST7735_Data(0x26);
    ST7735_Data(0x36);
    ST7735_Data(0x00);
    ST7735_Data(0x0D);
    ST7735_Data(0x0E);
    ST7735_Data(0x10);

    ST7735_Command(0x29);   // Display ON
    delay_ms(100);

    ST7735_Command(0x13);   // Normal display mode
    delay_ms(10);
}

void ST7735S_SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    ST7735_Command(0x2A); // Column addr set
    ST7735_Data(0x00);
    ST7735_Data(x0);
    ST7735_Data(0x00);
    ST7735_Data(x1);

    ST7735_Command(0x2B); // Row addr set
    ST7735_Data(0x00);
    ST7735_Data(y0);
    ST7735_Data(0x00);
    ST7735_Data(y1);

    ST7735_Command(0x2C); // Write to RAM
}

void ST7735S_FillScreen(uint16_t color) {
    ST7735S_SetAddrWindow(0, 0, 132, 132); // 132x132
    uint16_t pixel_count = 132*132;
    clrPin(CS);
    setPin(DCX); // Data mode

    for (uint16_t i = 0; i < pixel_count; i++) {
        //uint16_t color = (((uint16_t)red) << 8) | (green << 4) | blue;
        spiWrite12bit(color);
    }
    setPin(CS);
}

