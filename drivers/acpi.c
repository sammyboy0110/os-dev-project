#include "drivers/acpi.h"
#include "drivers/screen.h"
#include "libc/function.h"
#include "libc/memory.h"
#include "libc/string.h"
#include "libc/stddef.h"
#include "libc/stdlib.h"

bool rsdpFound = false;
bool rsdtFound = false;

struct RSDPDescriptor* prsdpd = 0x00000000;
struct ACPISDTHeader* pacpisdth = 0x00000000;

void init_acpi()
{
	char* ptr = (char*)0x000E0000;
	char sigptr[9];
	memset(sigptr, 0, sizeof(sigptr));
	do
	{
		memcpy(sigptr, ptr, 8);
		if (strcmp("RSD PTR ", sigptr) == 0)
		{
			rsdpFound = true;
			break;
		}
	}
	while((int)(ptr += 16) <= 0x000FFFFF);

	if (!rsdpFound) {
		char* ebdaptr = (char*)(0x40E);
		ptr = ebdaptr;

		do
		{
			memcpy(sigptr, ptr, 8);
			if (strcmp("RSD PTR ", sigptr) == 0)
			{
				rsdpFound = true;
				break;
			}
		}
		while ((int)(ptr += 16) <= (int)ebdaptr + 1024);
	}

	if (!rsdpFound)
	{
		kcrit("RSDP NOT FOUND!\n");
		return;
	}
	if (strcmp(".RSDP NOT FOUND", (ptr + 8)) == 0)
	{
		kprint("ACPI not supported\n");
		return;
	}

	struct RSDPDescriptor* rsdpd = (struct RSDPDescriptor*)(ptr);
	UNUSED(rsdpd);
	// Checksum validation.
	{
		uint32_t checksumResult = 0;
		for (uint32_t i = 0; i <= sizeof(struct RSDPDescriptor); i++)
		{
			checksumResult += *(ptr + i);
		}
		if ((checksumResult & 0xFF) != 0)
		{
			kcrit("RSDP checksum failed.\n");
			rsdpFound = false;
		}
	}
	if (rsdpd->Revision == 0)
	{
		kprint("ACPI version 1.0\n");
	}
	else if (rsdpd->Revision == 2) {
		kprint("ACPI version >= 2.0.\tAttempting to detect exact value.\n");
	}
	else
	{
		kprint("Could not detect ACPI version.\n");
	}

	struct ACPISDTHeader* prsdt = (struct ACPISDTHeader*)rsdpd->RsdtAddress;
	rsdtFound = true;

	// Checksum validation
	{
		uint32_t checksumResult = 0;
		for (uint32_t i = 0; i < prsdt->Length; i++)
		{
			checksumResult += (int)*((char*)prsdt + i);
		}
		if ((checksumResult & 0xFF) != 0)
		{
			kcrit("RSDT checksum failed.\n");
			rsdtFound = false;
		}
	}

	prsdpd = rsdpd;
	pacpisdth = prsdt;
}


struct RSDPDescriptor getRsdp()
{
	return *prsdpd;
}

struct ACPISDTHeader* getAcpisdt()
{
	return pacpisdth;
}

