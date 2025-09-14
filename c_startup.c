extern unsigned int _sidata;
extern unsigned int _sdata;
extern unsigned int _edata;
extern unsigned int _sbss;
extern unsigned int _ebss;

extern void main(void);

void c_startup(){
    // Copy initialized data from Flash to SRAM
    unsigned int *src = &_sidata;
    unsigned int *dst = &_sdata;
    while(dst < &_edata){
        *dst++ = *src++;
    }

    // Initialize the bss values with 0
    unsigned int *ptr = &_sbss;
    while(ptr < &_ebss){
        *ptr++ = 0;
    }
    // Call main
    main();
}