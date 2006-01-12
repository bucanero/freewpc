#ifndef _ASM_6809_H
#define _ASM_6809_H

/* Basic properties of the CPU architecture */
#define BITS_PER_BYTE 8
#define BITS_PER_WORD 16

/* Defines for various bits in the condition code register */
#define CC_CARRY 		0x1
#define CC_OVERFLOW 	0x2
#define CC_ZERO 		0x4
#define CC_NEGATIVE 	0x8
#define CC_IRQ 		0x10
#define CC_HALF 		0x20
#define CC_FIRQ 		0x40
#define CC_E 			0x80

extern inline void __lda (uint8_t i)
{
	asm __volatile__ ("lda\t%0" :: "g" (i) : "d");
}

extern inline void __ldb (uint8_t i)
{
	asm __volatile__ ("ldb\t%0" :: "g" (i) : "d");
}

extern inline void __sta (uint8_t *i)
{
	asm __volatile__ ("sta\t%0" : "=m" (*i));
}

extern inline void __stb (uint8_t *i)
{
	asm __volatile__ ("stb\t%0" : "=m" (*i));
}

extern inline void __bytecopy (uint8_t *dst, uint8_t src)
{
	__lda (src);
	__sta (dst);
}

extern inline void __swapbyte (uint8_t *v1, uint8_t *v2)
{
}

extern inline void set_stack_pointer (const uint16_t s)
{
	asm __volatile__ ("lds\t%0" :: "g" (s) : "d");
}


extern inline U16 get_stack_pointer (void)
{
	U16 result;
	asm __volatile__ ("lea%0\t,s" : "=a" (result));
	return result;
}

extern inline void set_direct_page_pointer (const uint8_t dp)
{
	asm __volatile__ ("tfr\tb, dp" :: "q" (dp));
}


extern inline void *memset (void *s, int c, long unsigned int n)
{
	if ((n % 2) == 0)
	{
		register U16 *s1 = (U16 *)s;
		n /= 2;
		while (n > 0)
		{
			*s1++ = ((U16)c << 8) | c;
			n--;
		}
	}
	else
	{
		register char *s1 = (char *)s;
		while (n > 0)
		{
			*s1++ = c & 0xFF;
			n--;
		}
		return (s);
	}
}


extern inline void __blockclear16 (void *s1, long unsigned int n)
{
	register U16 *_s1 = (U16 *)s1;
	do
	{
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		*_s1++ = 0UL;
		n -= 16;
	} while (n > 0);
}


extern inline void *memcpy (void *s1, const void *s2, long unsigned int n)
{
	register char *_s1 = (char *)s1;
	register char *_s2 = (char *)s2;
	while (n > 0)
	{
		*_s1++ = *_s2++;
		n--;
	}
	return (s1);
}


extern inline void __blockcopy16 (void *s1, const void *s2, long unsigned int n)
{
	register U16 *_s1 = (U16 *)s1;
	register U16 *_s2 = (U16 *)s2;
	do
	{
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		*_s1++ = *_s2++;
		n -= 16;
	} while (n > 0);
}



extern inline void *memmove (void *s1, const void *s2, long unsigned int n)
{
	return memcpy (s1, s2, n);
}


extern inline long unsigned int strlen (const char *s)
{
	long unsigned int len = 0;
	while (*s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}



#endif /* _ASM_6809_H */
