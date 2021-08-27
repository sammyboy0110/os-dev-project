#include "drivers/serial.h"
#include "kernel/util.h"
#include "libc/function.h"
#include "cpu/ports.h"
#include "cpu/isr.h"
#include "drivers/screen.h"

int serial_initialized = 0;

static void serial_in_callback(registers_t *regs)
{
	char serial_in = port_byte_in(COM1);

	kprint("char recived from serial in: ");
	char byte_str[] = { serial_in, '\0' };
	kprint(byte_str);
	kprint("\n");

	UNUSED(regs);
}

int init_serial()
{
	if (serial_initialized > 0) return 2;

	int fail = 1;

	port_byte_out(COM1 + 1, (unsigned char)0x00);
	port_byte_out(COM1 + 3, (unsigned char)0x80);
	port_byte_out(COM1 + 0, (unsigned char)0x03);
	port_byte_out(COM1 + 1, (unsigned char)0x00);
	port_byte_out(COM1 + 3, (unsigned char)0x03);
	port_byte_out(COM1 + 2, (unsigned char)0xC7);
	port_byte_out(COM1 + 4, (unsigned char)0x0B);
	port_byte_out(COM1 + 4, (unsigned char)0x1E);
	port_byte_out(COM1 + 0, (unsigned char)'S');

	unsigned char loop_byte = port_byte_in(COM1);
	if (loop_byte != (unsigned char)'S')
	{
		return fail;
	}
	else
	{
		fail = 0;
		serial_initialized = 1;
	}

	register_interrupt_handler(IRQ4, serial_in_callback);
	port_byte_out(COM1 + 4, 0x0F);

	return fail;
}

int serial_is_initialized()
{
	return serial_initialized;
}

