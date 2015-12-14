#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

u16int *video_memory = (u16int *) 0xB8000;
u8int cursor_x = 0;
u8int cursor_y = 0;

void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);

#endif
