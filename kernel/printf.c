
#include <freewpc.h>
#include <m6809_math.h>

char sprintf_buffer[PRINTF_BUFFER_SIZE];

uint8_t sprintf_width;

bool sprintf_leading_zeroes;


char digit2char (uint8_t digit)
{
	if (digit <= 9)
		return digit + '0';
	else
		return digit - 10 + 'A';
}


char *do_sprintf_decimal (char *buf, uint16_t w)
{
	uint8_t quot;
	uint8_t rem;

	DIV10 (w & 0xFF, quot, rem);

	*buf++ = quot + '0';
	*buf++ = rem + '0';
	return buf;
}


char *do_sprintf_hex_byte (char *buf, uint8_t b)
{
	*buf++ = digit2char (b >> 4);
	*buf++ = digit2char (b & 0x0F);
	return buf;
}


char *do_sprintf_hex (char *buf, uint16_t w)
{
	buf = do_sprintf_hex_byte (buf, w >> 8);
	buf = do_sprintf_hex_byte (buf, w & 0xFF);
	return buf;
}


void sprintf (const char *format, ...)
{
	va_list va;
	uint8_t *buf = sprintf_buffer;

	va_start (va, format);
	while (*format)
	{
		if (*format == '%')
		{
do_format_chars:
			format++;
			
			sprintf_width = 0;
			sprintf_leading_zeroes = FALSE;

			switch (*format)
			{
				case '0':
					sprintf_leading_zeroes = TRUE;
					goto do_format_chars;

				case '1':
				case '2':
				case '3':
				case '4':
					sprintf_width = *format - '0';
					goto do_format_chars;

				case 'd':
				case 'i':
				{
					uint16_t w = va_arg (va, uint16_t);
					buf = do_sprintf_decimal (buf, w);
					break;
				}

				case 'x':
				{
					register uint16_t w  = va_arg (va, uint16_t);
					if (sprintf_width <= 2)
						buf = do_sprintf_hex_byte (buf, w & 0xFF);
					else
						buf = do_sprintf_hex (buf, w);
					break;
				}

				case 'b':
				{
					register bcd_t *bcd  = va_arg (va, bcd_t *);
					db_putp (bcd);
					while (sprintf_width-- != 0)
					{
						db_put2x (*bcd);
						buf = do_sprintf_hex_byte (buf, *bcd++);
					}
					break;
				}

				case 's':
				{
					const char *s = va_arg (va, const char *);
					while (*s)
						*buf++ = *s++;
					break;
				}

				case 'p':
				{
					void *p = va_arg (va, void *);
					p = p;
					break;
				}
			}
		}
		else
		{
			*buf++ = *format;
		}
		format++;
	}
	va_end (va);

	*buf = '\0';
#if 0
	db_puts ("sprintf: ");
	db_puts (sprintf_buffer);
	db_putc ('\n');
#endif
}


