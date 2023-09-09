// -----------------------------------main.c -------------------------------------
#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "function.h"

void displayMenu()
{
    uart_puts(
        "\n\n\tEnter a number to choose command:\n"
        "\t1.\tDisplay text on screen\n"
        "\t2.\tDisplay a small image\n"
        "\t3.\tDisplay a scrollable large image\n"
        "\t4.\tDisplay a video\n"
        "\t5.\tPlay game\n"
        "\t0.\tClear the screen\n");
}
void main()
{
    // set up serial console
    uart_init();
    // say hello
    uart_puts("\n\nHello World!\n");
    // Initialize frame buffer
    framebf_init();
    while (1)
    {
        unsigned int *physize = 0; //pointer to response data
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &physize, 8, 8, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);

        displayMenu();
        uart_puts("\n");
        // read each char
        uart_puts("\n\n");

        uart_puts("YplOS>>> ");

        char command = uart_getc();

        uart_sendc(command);
        uart_sendc('\n');

        if (command == '5') {
            displayGameBackground(0,0);
        }
        if (command == '0') {
            clearscreen(0,0);
        }
    }
}
