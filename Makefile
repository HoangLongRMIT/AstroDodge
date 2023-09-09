#--------------------------------------Makefile-------------------------------------
CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:./src/%.c=./obj/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
all: clean kernel8.img run
# ./obj/uart.o: ./src/uart.c
# 	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/uart.c -o ./obj/uart.o
./obj/boot.o: ./src/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/boot.S -o ./obj/boot.o
./obj/%.o: ./src/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@
kernel8.img: ./obj/boot.o ./obj/uart.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./obj/boot.o $(OFILES) -T ./src/link.ld -o ./obj/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./obj/kernel8.elf ./kernel8.img

clean:
	rm -f ./obj/kernel8.elf ./obj/*.o ./kernel8.img

#raspi3b for mac and raspi3 for window
run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio
