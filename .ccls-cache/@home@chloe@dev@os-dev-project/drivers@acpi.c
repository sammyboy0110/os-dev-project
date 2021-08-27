#include "drivers/acpi.h"
#include "drivers/screen.h"
#include "libc/function.h"
#include "libc/memory.h"
#include "libc/string.h"
#include "libc/stddef.h"
#include "libc/stdlib.h"

void init_acpi()
{
	if (0)
	{
		// Search first KiB of EBDA for RSDP
		bool found = false;
		intptr_t ebda_base_address_location = 0x040E;
		intptr_t ebda_base_address = (uint16_t)((*((char*)ebda_base_address_location))) << 4;
		
		// EBDA size in bytes
		uint8_t ebda_size = ((*((char*)(ebda_base_address))) * 1024);
		UNUSED(ebda_size);

		intptr_t search_depth = ebda_base_address;

		do {
			if (strcmp("RSD PTR ", (char*)search_depth) == 0)
			{
				found = true;
				break;
			}
		} while((search_depth++) < (0x000A0000));

		// Seach from 0x000E0000 to 0x000FFFFF in memory for RSDP if now found in EBDA.
		if (!found) {
			search_depth = 0x000E0000;
			do {
				if (strcmp("RSD PTR ", (char*)search_depth) == 0)
				{
					found = true;
					break;
				}
			} while((search_depth++) < 0x000FFFFF);
		}
		if (!found) kcrit("RSDP NOT FOUND!\n");
	}
	bool found = false;
	char* ptr = 0x00000000;
	do
	{
		if (strcmp("RSD PTR ", ptr) == 0)
		{
			found = true;
			break;
		}
	}
	while((int)(ptr++) < 0x00100000);

	if (!found)
	{
		kcrit("RSDP NOT FOUND!\n");
		return;
	}
	else
	{
		char str[64];
		memset(str, 0, 64);
		kprint("0x");
		kprint(itoa((intptr_t)ptr, str, 16));
		kprint("\n");
	}
}

