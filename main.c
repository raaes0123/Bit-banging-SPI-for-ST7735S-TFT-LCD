/*
A software-based SPI (bit-banging) implementation to drive an ST7735S TFT LCD using GPIO pins 
on an STM32 microcontroller
*/
#include <stdint.h>
#include "main.h"
#include "lcd.h"

void delay_us(uint32_t us){
    SYST_RVR = 8*us-1;
    SYST_CVR = 0;
    SYST_CSR |= 1 << 0;                 // Enable Systick
    while(!(SYST_CSR & (1<<CNTFLAG)));
    SYST_CSR &= ~(1 << 0);             // Disable Systick
}

void delay_ms(uint32_t ms){
    SYST_RVR = 8000*ms-1;
    SYST_CVR = 0;
    SYST_CSR |= 1 << 0;                 // Enable Systick
    while(!(SYST_CSR & (1<<CNTFLAG)));
    SYST_CSR &= ~(1 << 0);             // Disable Systick
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

int main(void) {
    SYST_CSR |= 1 << 2;             // Use System Clock for Systick
    GPIOB_config();                 // Configure GPIOB for LCD
    setPin(BLK);
    ST7735S_Init();
    ST7735S_FillScreen(COLOR_SKY_BLUE);  
}
