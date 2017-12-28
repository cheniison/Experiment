
#include "myuart.h"
#include "uart.h"

uint_8 readByte()
{
	uint_t fp = 1, res;
	while(1)
	{
		res = uart_re1(UART_2, &fp);
		if (fp == 0)
			return res;
	}
}
