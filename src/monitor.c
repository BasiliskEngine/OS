#include "monitor.h"
#include "common.h"

u16int *video_memory = (u16int *)0xB8000;
u8int cursor_x = 0;
u8int cursor_y = 0;

static void move_cursor()
{
        u16int cursorLocation = cursor_y * 80 + cursor_x;
        outb(0x3D4, 14); // Tell VGA board, we are setting high cursor byte
        outb(0x3D5, cursorLocation >> 8); // Shift the cursor location byte, to the next byte location
        outb(0x3D4, 15); // Tell VGA board, we are setting low cursor byte
        outb(0x3D5, cursorLocation); // Send last cursorLocation
}

static void rewind_cursor() // moves cursor back one space
{
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation << 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
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

void monitor_put(char c)
{
	u8int backColor = 0;
	u8int foreColor = 15;

	u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);
	u16int attribute = attributeByte << 8;
	u16int *location;

	// Backspace
	if (c == 0x08 && cursor_x)
	{
		cursor_x--;
	}
	else if (c == 0x09) // Tab (move cursor to location divisible by 8)
	{
		cursor_x = (cursor_x+ 8) & ~(8-1);
	}
	else if (c == '\r') // Carriage return
	{
		cursor_x = 0;
	}
	else if (c == '\n') // Return (escape line)
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if (c >= ' ') // Any other character (space code and above in ASCII values)
	{
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	if (cursor_x >= 80) // If we are at end of buffer (last line)
	{
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	move_cursor();
}

void monitor_clear()
{
	u8int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	int i;
	for (i = 0; i < 80 * 25; i++)
	{
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void monitor_write(char *c)
{
	int i = 0;
	while (c[i])
	{
		monitor_put(c[i++]);
	}
}
