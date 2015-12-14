#include "monitor.h"
#include "common.h"

int main(struct multiboot *mboot_ptr)
{
	monitor_clear();
	monitor_write("Hello, world!");
	return 0xDEADBABA;
}
