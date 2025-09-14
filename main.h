#pragma once
// RCC
#define RCC_CR          *((volatile unsigned int*)0x40021000)
#define RCC_CFGR        *((volatile unsigned int*)0x40021004)
#define RCC_APB2ENR     *((volatile unsigned int*)0x40021018)


// GPIOA
#define GPIOA_CRL       *((volatile unsigned int*)0x40010800)
#define GPIOA_CRH       *((volatile unsigned int*)0x40010804)
#define GPIOA_IDR       *((volatile unsigned int*)0x40010808)
#define GPIOA_ODR       *((volatile unsigned int*)0x4001080C)
#define GPIOA_BSRR      *((volatile unsigned int*)0x40010810)

// GPIOB
#define GPIOB_CRL       *((volatile unsigned int*)0x40010C00)
#define GPIOB_CRH       *((volatile unsigned int*)0x40010C04)
#define GPIOB_IDR       *((volatile unsigned int*)0x40010C08)
#define GPIOB_ODR       *((volatile unsigned int*)0x40010C0C)
#define GPIOB_BSRR      *((volatile unsigned int*)0x40010C10)

// LCD PINS
#define SCK         4
#define MOSI        5
#define RST         6
#define DCX         7
#define CS          8
#define BLK         9

// SYSTICK
#define SYST_CSR   (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR   (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR   (*(volatile uint32_t*)0xE000E018)
#define SYST_CALIB (*(volatile uint32_t*)0xE000E01C)

#define CNTFLAG        16

// AFIO
#define AFIO_MAPR  (*(volatile uint32_t*)0x40010004)


