/*
 * Copyright 2006 by Brian Dominy <brian@oddchange.com>
 *
 * This file is part of FreeWPC.
 *
 * FreeWPC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * FreeWPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FreeWPC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _SYS_DMD_H
#define _SYS_DMD_H

/** The size of each DMD page */
#define DMD_PAGE_SIZE 0x200UL

/** The number of DMD pages */
#define DMD_PAGE_COUNT 16

/** The width of a DMD page, in pixels */
#define DMD_PIXEL_WIDTH	128

/** The width of a DMD page, in bytes */
#define DMD_BYTE_WIDTH (DMD_PIXEL_WIDTH / 8)

/** The height of a DMD page, in pixel */
#define DMD_PIXEL_HEIGHT 32

/** The type of a page number */
typedef uint8_t dmd_pagenum_t;

/** The type of a page buffer pointer */
typedef uint8_t *dmd_buffer_t;


/** A DMD transition describes all of the properties of a
 * transition, such as the functions that render each
 * successive frame change, timing, etc.
 */
typedef struct
{
	/** Render the part of the transitional image that comes from
	 * the previous image. */
	void (*composite_old) (void);

	/** Render the part of the transitional image that comes from
	 * the new image.   This function is also responsible for
	 * determining when the transition is finished; it should
	 * dmd_in_transition to FALSE when this happens.
	 */
	void (*composite_new) (void);

	/** A parameter that determines how fast the transition occurs.
	 * This is the delay between transitional frames, and is given
	 * as a TIME_xxx define. */
	U8 delay;

	U8 arg;
} dmd_transition_t;


#define wpc_dmd_firq_row		(*(uint8_t *)WPC_DMD_FIRQ_ROW_VALUE)
#define wpc_dmd_visible_page	(*(uint8_t *)WPC_DMD_ACTIVE_PAGE)

#define dmd_low_buffer			((dmd_buffer_t)DMD_LOW_BASE)
#define dmd_high_buffer			((dmd_buffer_t)DMD_HIGH_BASE)

extern U8 *dmd_trans_data_ptr;
extern bool dmd_in_transition;
extern dmd_transition_t *dmd_transition;

extern dmd_transition_t 
	trans_scroll_up,
	trans_scroll_down,
	trans_scroll_left,
	trans_scroll_right
	;

void dmd_init (void);
void dmd_rtt (void);
void dmd_alloc_low (void);
void dmd_alloc_high (void);
void dmd_alloc_low_high (void);
void dmd_show_low (void);
void dmd_show_high (void);
void dmd_show_other (void);
void dmd_flip_low_high (void);
void dmd_show2 (void);
void dmd_clean_page (dmd_buffer_t dbuf);
void dmd_clean_page_low (void);
void dmd_clean_page_high (void);
void dmd_invert_page (dmd_buffer_t dbuf);
void dmd_copy_page (dmd_buffer_t dst, dmd_buffer_t src);
void dmd_copy_low_to_high (void);
void dmd_alloc_low_clean (void);
void dmd_alloc_high_clean (void);
void dmd_draw_border (char *dbuf);
void dmd_draw_horiz_line (U16 *dbuf, U8 y);
void dmd_shift_up (dmd_buffer_t dbuf);
void dmd_shift_down (dmd_buffer_t dbuf);
void dmd_draw_image (dmd_buffer_t image_bits);
void dmd_draw_image2 (dmd_buffer_t image_bits);
void dmd_draw_bitmap (dmd_buffer_t image_bits, 
	U8 x, U8 y, U8 width, U8 height);
void dmd_nop_hook (U8 new_dark_page, U8 new_bright_page);
void dmd_do_transition (void);
void dmd_sched_transition (dmd_transition_t *trans);

#endif /* _SYS_DMD_H */
