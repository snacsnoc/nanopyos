CC=aarch64-elf-gcc
CFLAGS=-nostdlib -nostartfiles -ffreestanding -I./build/Python-3.11.8/Include -I./include -I./build/toolchain/include/
LDFLAGS=-T linker.ld -L./python -lpython3.11

all: kernel.img

kernel.img: start.o main.o
	$(CC) $(LDFLAGS) -o kernel.elf $^
	aarch64-none-elf-objcopy -O binary kernel.elf kernel.img

start.o: start.S
	$(CC) $(CFLAGS) -c start.S -o start.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.elf *.img
