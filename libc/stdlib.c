#include "libc/stddef.h"
#include "libc/stdlib.h"
#include "libc/function.h"
#include "libc/memory.h"
#include "libc/math.h"
#include "drivers/screen.h"

const char _itoa_upper_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char _itoa_lower_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
const bool _itoa_upper_case;

char* itoa(int value, char str[], int base)
{
	if (base > 36 || base < 2)
	{
		kerr("only bases up to 36 supported");
	}

	char digits[36];
	if (_itoa_upper_case)	memcpy(digits, _itoa_upper_digits, 36);
	else					memcpy(digits, _itoa_lower_digits, 36);

	int len = 0;
	{
		int val = abs(value);
		do {
			val = (val - (val % base)) / base;
			len++;
		} while(val);
		if (value < 0) len++;
	}

	char* bufflim = str + len;

	switch(base)
	{
		case 10:
			{
				// TODO: Make optimized algorithms for base 10
			}
			// break; // Uncomment when optimized algorithm is implemented

		case 16:
			{
				// TODO: Make optimized algorithms for base 16
			}
			// break; // Uncomment when optimized algorithm is implemented

		case 2:
			{
				// TODO: Make optimized algorithms for base 2
			}
			// break; // Uncomment when optimized algorithm is implemented

		case 8:
			{
				// TODO: Make optimized algorithms for base 8
			}
			// break; // Uncomment when optimized algorithm is implemented

		default:
			{
				int val = abs(value);
				do {
					*--bufflim = digits[val % base];
				}
				while((val /= base) != 0);
				if (value < 0) *(--bufflim) = '-';
			}
	}
	return str;
}

char* ftoa(float value, char str[], int afterpoint)
{
	bool negative = false;
	if (value < 0) negative = true;

	int ipart = (int)absf(value);
	int ipartlen = 0;
	{
		int val = ipart;
		do {
			val = ((val - val % 10) / 10);
			ipartlen++;
		} while(val);
		if (negative) ipartlen++;
	}

	float decpart = absf(value) - (float)ipart;
	do {
		decpart *= 10;
	} while ((--afterpoint) != 0);

	str += (intptr_t)(int)(negative);
	itoa(ipart, str, 10);
	if (negative) *(--str) = '-';
	*(str + ipartlen) = '.';
	itoa(decpart, (str + ipartlen + 1), 10);

	return str;
}

