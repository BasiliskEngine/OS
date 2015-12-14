#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);

static void move_cursor()
{
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14); // Tell VGA board, we are setting high cursor byte
	outb(0x3D5, cursorLocation >> 8); // Shift the cursor location byte, to the next byte location
	outb(0x3D4, 15); // Tell VGA board, we are setting low cursor byte
	outb(0x3D5, cursorLocation); // Send last cursorLocation
}

static void scroll()
{
	u8int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);
	
	if (cursor_y >= 25)
	{
		int i;
		for (i = 0 * 80; i < 24 * 80; i++)
		{
			video_memory[i] = video_memory[i + 80];
		}

		for (i = 24 * 80; i < 25 * 80; i++)
		{
			video_memory[i] = blank;
		}

		cursor_y = 24;
	}
}
 
#endif
