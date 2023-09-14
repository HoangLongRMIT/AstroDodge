#include "image1.h"
#include "image2.h"
#include "image3.h"
#include "image4.h"
#include "framebf.h"
#include "function.h"
#include "uart.h"

// Function to display first image
void display_image_1(int x, int y)
{
  for (int h = 0; h < image1_height; h++)
    for (int w = 0; w < image1_width; w++)
      drawPixelARGB32(x + w, y + h, image1[h * image1_width + w]);
}

// Function to display second image
void display_image_2(int x, int y)
{
  for (int h = 0; h < image2_height; h++)
    for (int w = 0; w < image2_width; w++)
      drawPixelARGB32(x + w, y + h, image2[h * image2_width + w]);
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

// Display selected images
void display_certain_image(int count, int x, int y)
{
  if (count == 1)
    display_image_1(x,y);
  else if (count == 2)
    display_image_4(x,y);
  else if (count == 3)
    display_image_3(x,y);
}

// Function to move vertical
void moveVerticalScreen(int width, int height, int x, int y)
{
  for (int h = 0; h < height; h++)
    for (int w = 0; w < width; w++)
      drawPixel(x + w, y + h, 0);
}

// Function to control oversize image
void control_scrollable_image(int x, int y)
{
  // Prompt the user how to use
  uart_puts("Press w to scroll up: \n");
  uart_puts("Press s to scroll down: \n");
  uart_puts("Press x to out: \n");
  // Assign value for screen width, height and value for each time scroll up or down
  int screen_width = 1200;
  int screen_height = 800;
  int scroll_vertical_value = 20;
  display_image_2(x, y);
  while (1)
  {
    // Declare character variable to get user input
    char character = uart_getc();
    // if character = w, scroll up -> screen up
    if (character == 'w')
    {
      // Move the screen
      moveVerticalScreen(screen_width, scroll_vertical_value, x, y + screen_height - scroll_vertical_value);
      // Calculate new y coordinate value
      y = y - scroll_vertical_value;
      //Reset image to make no trace of previous one
      clearscreen(0,0);
      // Display image
      display_image_2(x, y);
    }
    // if character = s, scroll down -> screen down
    else if (character == 's')
    {
      // Move the screen
      moveVerticalScreen(screen_width, scroll_vertical_value, x, y);
      // Calculate new y coordinate value
      y = y + scroll_vertical_value;
      //Reset image to make no trace of previous one
      clearscreen(0,0);
      // Display image
      display_image_2(x, y);
    }
    // if character = o, out -> stop move screen
    else if (character == 'x')
    {
      moveVerticalScreen(screen_width, scroll_vertical_value, x, y + screen_height - scroll_vertical_value);
      // Prompt message for user
      uart_puts("\n\nSuccessfully out!\n");
      return;
    }
  }
}

// Display slideshow of images
void control_slideshow_image(int x, int y, int count)
{
  // Prompt the user how to use
  uart_puts("Press a to switch left: \n");
  uart_puts("Press d to switch right: \n");
  uart_puts("Press x to out: \n");
  // Assign value for screen width, height and value for each time scroll up or down
  int screen_width = 1200;
  int screen_height = 800;
  display_certain_image(count, x, y);
  while (1)
  {
    // Declare character variable to get user input
    char character = uart_getc();
    if (character == 'a')
    {
      //Reset image to make no trace of previous one
      clearscreen(0,0);
      count--;
      if(count == 0)
        count = 3;
      display_certain_image(count, x, y);
    } 
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