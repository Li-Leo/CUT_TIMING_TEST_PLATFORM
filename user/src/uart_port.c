
#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "stdint.h"

#pragma import(__use_no_semihosting)
// __asm(".global __use_no_semihosting\n\t");   //AC6

void _sys_exit(int x)
{

    x = x;
}

void _ttywrch(int ch)
{
    ch = ch;
}

struct __FILE
{

    int handle;
};


FILE __stdout;

int fputc(int ch, FILE *f)
{
    uint8_t temp[1]={ch};
    HAL_UART_Transmit(&huart1, temp, 1, 10);
    
    return 0;
}
