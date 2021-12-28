objs = start.o led.o uart.o init.o nand_flash.o main.o hyhsnake.o wechat.o interrupt.o timer.o my_printf.o string_utils.o lib1funcs.o

objs += lcd/font.o
objs += lcd/framebuffer.o
objs += lcd/geometry.o
objs += lcd/lcd.o
objs += lcd/lcd_4.3.o
objs += lcd/lcd_controller.o
objs += lcd/lcd_test.o
objs += lcd/s3c2440_lcd_controller.o
objs += lcd/font_8x16.o
objs += lcd/image.o

all: $(objs)
	#arm-linux-ld -Ttext 0 -Tdata 0x30000000  start.o led.o uart.o init.o main.o -o Hyhgame.elf
	arm-linux-ld -T sdram.lds $^ -o Hyhgame.elf
	arm-linux-objcopy -O binary -S Hyhgame.elf Hyhgame.bin
	arm-linux-objdump -D Hyhgame.elf > Hyhgame.dis
clean:
	rm *.bin *.o *.elf *.dis
	
%.o : %.c
	arm-linux-gcc -march=armv4 -c -o $@ $<

%.o : %.S
	arm-linux-gcc -march=armv4 -c -o $@ $<
	