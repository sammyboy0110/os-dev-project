C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS := $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

OBJ := ${C_SOURCES:.c=.o cpu/interrupt.o}

CC := i386-elf-gcc
GDB := i386-elf-gdb

TARGET := i386

CFLAGS := -g -m32 -fno-exceptions -Isysdeps/${TARGET} -I${CURDIR} -Wall -Wextra -Werror

bin/os-image.bin: boot/boot.bin kernel.bin
	cat $^ > $@
	chmod +x $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary
	chmod -x $@

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

.PHONY: run
run: bin/os-image.bin
	qemu-system-i386 -chardev vc,id=serial0,logfile=serial.log,\
		-serial chardev:serial0\
		-drive file=bin/os-image.bin,if=floppy,format=raw

.PHONY: debug
debug: bin/os-image.bin kernel.elf
	qemu-system-i386 -s -S -chardev vc,id=serial0,logfile=serial.log,signal=off \
		-serial chardev:serial0 \
		-drive file=bin/os-image.bin,if=floppy,format=raw & \
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

.PHONY: run-vnc
run-vnc: bin/os-image.bin
	qemu-system-i386 -vnc :0 -chardev stdio,id=serial0,logfile=serial.log,signal=off\
		-serial chardev:serial0\
		-drive file=bin/os-image.bin,if=floppy,format=raw

.PHONY: debug-vnc
debug-vnc: bin/os-image.bin
	qemu-system-i386 -vnc :0 -s -chardev stdio,id=serial0,logfile=serial.log,signal=off \
		-serial chardev:serial0 \
		-drive file=bin/os-image.bin,if=floppy,format=raw & \
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -felf -o $@

%.bin: %.asm
	nasm $< -fbin -o $@

.PHONY: clean
clean:
	rm -f bin/* *.o *.elf *.bin cpu/*.o cpu/*.bin
	rm -f kernel/*.o kernel/*.bin drivers/*.o boot/*.bin boot/*.o libc/*.o

