#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/acpi.h"
#include "kernel/util.h"
#include "cpu/isr.h"
#include "cpu/idt.h"
#include "cpu/timer.h"
#include "cpu/ports.h"
#include "libc/string.h"
#include "libc/stddef.h"
#include "libc/stdlib.h"
#include "libc/math.h"
#include "libc/memory.h"

static bool should_quit = false;

void _start()
{
	kprint_at("Loaded Kernel\n", 0, 1);
	init_acpi();
	isr_install();
	irq_install();

	kprint("\n> ");

	while (!should_quit);
}

void user_input(char* input)
{
	if (strcmp(input, "shutdown") == 0)
	{
		kprint("Stopping the CPU.\nBye!\n");
		should_quit = true;
	}
	else if (strcmp(input, "clear") == 0)
	{
		clear_screen();
		kprint("> ");
	}
	else {
		kprint("You said: ");
		kprint(input);
		kprint("\n> ");
	}
}

