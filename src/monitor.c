#include "monitor.h"

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