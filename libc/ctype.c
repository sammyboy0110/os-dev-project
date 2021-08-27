#include "libc/ctype.h"

int isalpha(int c)
{
	return (unsigned)(c - 0x41) <= (0x5A - 0x41) ? c : 0;
}

