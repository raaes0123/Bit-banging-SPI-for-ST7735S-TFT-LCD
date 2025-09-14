/*
A software-based SPI (bit-banging) implementation to drive an ST7735S TFT LCD using GPIO pins 
on an STM32 microcontroller
*/
#include <stdint.h>
#include "main.h"

void delay_us(uint32_t us){
    SYST_RVR = 8*us-1;
    SYST_CVR = 0;
    SYST_CSR |= 1 << 0;                 // Enable Systick
    while(!(SYST_CSR & (1<<CNTFLAG)));
    SYST_CSR &= ~(1 << 0);             // Disable Systick
}

void delay_ms(uint32_t us){
    SYST_RVR = 8000*us-1;
    SYST_CVR = 0;
    SYST_CSR |= 1 << 0;                 // Enable Systick
    while(!(SYST_CSR & (1<<CNTFLAG)));
    SYST_CSR &= ~(1 << 0);             // Disable Systick
}

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
        delay_us(10);
        setPin(SCK);
        delay_us(10);
    }
}

void GPIOB_config(){
    RCC_APB2ENR |= 0x01;            // Enable AFIO clock (Needed to use AFIO_MAPR)      
    AFIO_MAPR &= ~(7<<24);          // 010: JTAG-DP Disabled and SW-DP Enabled
    AFIO_MAPR |= 2<<24;             // This frees up PB4 (SCK)
    // Enable GPIOB clock
    RCC_APB2ENR |= (1 << 3);            // Bit 3 = GPIOBEN
    
    // Disable AFIO clock
    RCC_APB2ENR |= (1 << 3);            // Bit 3 = GPIOBEN

    // SCK as Output
    GPIOB_CRL &= ~(3 << 4*SCK);       // Clear MODE2 (output 10MHz)
    GPIOB_CRL |=  (1 << 4*SCK);
    GPIOB_CRL &= ~(3 << (4*SCK+2));   // Clear CNF2 (00) GPIO Push-pull
    // MOSI as Output
    GPIOB_CRL &= ~(3 << 4*MOSI);       // Clear MODE2 (output 10MHz)
    GPIOB_CRL |=  (1 << 4*MOSI);
    GPIOB_CRL &= ~(3 << (4*MOSI+2));   // Clear CNF2 (00) GPIO Push-pull
    // RST as Output
    GPIOB_CRL &= ~(3 << 4*RST);       // Clear MODE2 (output 10MHz)
    GPIOB_CRL |=  (1 << 4*RST);
    GPIOB_CRL &= ~(3 << (4*RST+2));   // Clear CNF2 (00) GPIO Push-pull
    // DCX as Output
    GPIOB_CRL &= ~(3 << 4*DCX);       // Clear MODE2 (output 10MHz)
    GPIOB_CRL |=  (1 << 4*DCX);
    GPIOB_CRL &= ~(3 << (4*DCX+2));   // Clear CNF2 (00) GPIO Push-pull

    // CS as Output
    GPIOB_CRH &= ~(3 << 4*(CS-8));       // Clear MODE2 (output 10MHz)
    GPIOB_CRH |=  (1 << 4*(CS-8));
    GPIOB_CRH &= ~(3 << (4*(CS-8)+2));   // Clear CNF2 (00) GPIO Push-pull
    // BLK as Output
    GPIOB_CRH &= ~(3 << 4*(BLK-8));       // Clear MODE2 (output 10MHz)
    GPIOB_CRH |=  (1 << 4*(BLK-8));
    GPIOB_CRH &= ~(3 << (4*(BLK-8)+2));   // Clear CNF2 (00) GPIO Push-pull

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

void Software_Reset(){
    ST7735_Command(0x01);
}

void ST7735S_Init(void) {
    ST7735_Reset();         // Toggle RST pin
    delay_ms(150);
    Software_Reset();
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
    ST7735_Data(0xC8);      // MX, MY, RGaB (0xC8) or BGR (0xC0) ???

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
    ST7735S_SetAddrWindow(0, 0, 127, 127); // 128x128

    clrPin(CS);
    setPin(DCX); // Data mode

    for (uint16_t i = 0; i < 128 * 128; i++) {
        //uint16_t color = (((uint16_t)red) << 8) | (green << 4) | blue;
        spiWrite12bit(color);
    }

    setPin(CS);
}


int main(void) {

    SYST_CSR |= 1 << 2;             // Use System Clock for Systick
    GPIOB_config();                 // Configure GPIOB for LCD
    setPin(BLK);
    ST7735S_Init();
    ST7735S_FillScreen(0x02A6);     // 
    while(1){
    }   
}
