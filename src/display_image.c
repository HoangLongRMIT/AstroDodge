#include "image1.h"
#include "image2.h"
#include "image3.h"
#include "image4.h"
#include "framebf.h"
#include "function.h"
#include "uart.h"

#define MAX_DOWN_HEIGHT 635
#define BUFFER_STEP 40

// Function to display first image
void display_image_1(int x, int y)
{
  for (int h = 0; h < image1_height; h++)
    for (int w = 0; w < image1_width; w++)
      drawPixelARGB32(x + w, y + h, image1[h * image1_width + w]);
}

// Function to display second image to scroll
void display_image_2(int row)
{
  for (int h = 0; h < image2_height; h++)
    for (int w = 0; w < image2_width; w++)
      drawPixelARGB32(w, h, image2[(h + row) * image2_width + w]);
}

// Function to display third image
void display_image_3(int x, int y)
{
  for (int h = 0; h < image3_height; h++)
    for (int w = 0; w < image3_width; w++)
      drawPixelARGB32(x + w, y + h, image3[h * image3_width + w]);
}

// Function to display fourth image
void display_image_4(int x, int y)
{
  for (int h = 0; h < image4_height; h++)
    for (int w = 0; w < image4_width; w++)
      drawPixelARGB32(x + w, y + h, image4[h * image4_width + w]);
}

// Display selected images (image 1, image 3, image 4) as commanded with count starts at 1 in kernel.c
void display_certain_image(int count, int x, int y)
{
  if (count == 1)
    display_image_1(x,y);
  else if (count == 2)
    display_image_4(x,y);
  else if (count == 3)
    display_image_3(x,y);
}

// Function to display scrollable large image
void controlScrollableImage()
{

	framebf_init();
	uart_puts("\nPress 's' to scroll down \n");
	uart_puts("Press 'w' to scroll up \n");
	uart_puts("Press x to exit image! \n\n");
	
  int row = 0;
  display_image_2(row);

	while (1)
	{
    /*
        NOTE:
          In order to scroll down without a problem, during debug this picture needs to press s for around 15 time with BUFFER_STEP to move down for 40 rows of image 2 height (1235).
          So BUFFER_STEP (40) * 15 times = 600 rows to reach at the end of the image without going further.
          Hence image 2 height (1235) - 600 = 635 which is the number for MAX_DOWN_HEIGHT as defined.
          BUFFER_STEP can be any number but 40 in personal opinion is suitable in this situation.
    */

    // Declare character variable to get user input
    char character = uart_getc();
    
    // Scroll down the image withing the range of row to scroll as defined
    if (character == 's' && (row <= image2_height - MAX_DOWN_HEIGHT - BUFFER_STEP))
		{
      row = row + BUFFER_STEP;
			display_image_2(row);
		}
    // Scroll up the image withing the range of row to scroll as defined
    else if (character == 'w' && (row >= BUFFER_STEP))
		{
			row = row - BUFFER_STEP;
			display_image_2(row);
		}
		else if (character == 'x')
    {
      // Prompt message for user
      uart_puts("\n\nSuccessfully out!\n");
      return;
    }
	}
}

// Display slideshow of small images
void controlSlideshowImage(int x, int y, int count)
{
  // Prompt the user how to use
  uart_puts("Press a to switch left: \n");
  uart_puts("Press d to switch right: \n");
  uart_puts("Press x to out: \n");

  display_certain_image(count, x, y);
  while (1)
  {
    // Declare character variable to get user input
    char character = uart_getc();
    // Backward the option of selected image in the range
    if (character == 'a')
    {
      //Reset image to make no trace of previous one
      clearscreen(0,0);
      // 
      count--;
      if(count == 0)
        count = 3;
      display_certain_image(count, x, y);
    } 
    // Forward the option of selected image in the range
    else if (character == 'd') 
    {
      //Reset image to make no trace of previous one
      clearscreen(0,0);
      count++;
      if(count == 4)
        count = 1;
      display_certain_image(count, x, y);
    }
    // if character = x, out -> stop move screen
    else if (character == 'x')
    {
      // Prompt message for user
      uart_puts("\n\nSuccessfully out!\n");
      return;
    }
  }
}