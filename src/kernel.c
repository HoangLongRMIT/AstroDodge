// -----------------------------------main.c -------------------------------------
#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "function.h"
#include "display_image.h"
#include "display_video.h"

void displayMenu()
{
    uart_puts(
        "\n\n\tEnter a number to choose command:\n"
        "\t1.\tDisplay text on screen\n"
        "\t2.\tDisplay a slideshow of images\n"
        "\t3.\tDisplay a scrollable large image\n"
        "\t4.\tDisplay a video\n"
        "\t5.\tPlay game\n"
        "\t0.\tClear the screen\n");
}
void main()
{
    // declaring variables
    int x_coordinate = 100;
    int y_coordinate = 300;
    // set up serial console
    uart_init();
    // Initialize frame buffer
    framebf_init();
    while (1)
    {
        unsigned int *physize = 0; //pointer to response data
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &physize, 8, 8, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);

        int count = 1;
        displayMenu();
        uart_puts("\n");
        // read each char
        uart_puts("\n\n");

        uart_puts("YplOS>>> ");

        char command = uart_getc();

        uart_sendc(command);
        uart_sendc('\n');

        if (command == '2')
        {
            clearscreen(0,0);
            control_slideshow_image(x_coordinate, y_coordinate, count);
        }
        else if (command == '3')
        {
            clearscreen(0,0);
            control_scrollable_image(x_coordinate, y_coordinate);
        } else if (command == '4')
        {
            clearscreen(0,0);
            display_Video(x_coordinate, y_coordinate);
        }
        else if (command == '5') {
            clearscreen(0,0);
            displayGameBackground(0,0);
        }
        else if (command == '0') {
            clearscreen(0,0);
            uart_puts("\033[2J"  // clear screen
					    "\033[H");  // scroll to top
        }
    }
}