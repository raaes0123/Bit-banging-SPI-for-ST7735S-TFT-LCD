CPU ?= cortex-m4

build:
	arm-none-eabi-gcc -mcpu=$(CPU) -mthumb -ggdb -c startup.s -o startup.o
	arm-none-eabi-gcc -mcpu=$(CPU) -mthumb -ggdb -c main.c -o main.o
	arm-none-eabi-gcc -mcpu=$(CPU) -mthumb -ggdb -c c_startup.c -o c_startup.o
	arm-none-eabi-ld -T link.ld startup.o main.o c_startup.o -o main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin
	stm32flash -w main.bin -v -g 0x0 /dev/ttyUSB0

clean:
	rm -rf *.out *.elf *.debug *.o *.bin

