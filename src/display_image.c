#include "image1.h"
#include "image2.h"
#include "image3.h"
#include "image4.h"
#include "framebf.h"
#include "function.h"
#include "uart.h"
#include "game_over.h"
#include "game_win.h"
#include "game_image.h"
#include "life.h"
#include "spaceship.h"
#include "mbox.h"

// Function to display first image
//--------------------------------------------------------------------------
void display_image_1(int x, int y)
{
  for (int h = 0; h < image1_height; h++)
    for (int w = 0; w < image1_width; w++)
      drawPixelARGB32(x + w, y + h, image1[h * image1_width + w]);
}

// Function to display second image
//--------------------------------------------------------------------------
void display_image_2(int x, int y)
{
  for (int h = 0; h < image2_height; h++)
    for (int w = 0; w < image2_width; w++)
      drawPixelARGB32(x + w, y + h, image2[h * image2_width + w]);
}

// Function to display third image
//--------------------------------------------------------------------------
void display_image_3(int x, int y)
{
  for (int h = 0; h < image3_height; h++)
    for (int w = 0; w < image3_width; w++)
      drawPixelARGB32(x + w, y + h, image3[h * image3_width + w]);
}

// Function to display fourth image
//--------------------------------------------------------------------------
void display_image_4(int x, int y)
{
  for (int h = 0; h < image4_height; h++)
    for (int w = 0; w < image4_width; w++)
      drawPixelARGB32(x + w, y + h, image4[h * image4_width + w]);
}

// Display selected images
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
void moveVerticalScreen(int width, int height, int x, int y)
{
  for (int h = 0; h < height; h++)
    for (int w = 0; w < width; w++)
      drawPixel(x + w, y + h, 0);
}

// Function to control oversize image
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
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

//=======================================================================================//
//                            FUNCTION DISPLAY NUM SCORE                                 //
//=======================================================================================//
// Function to display word "score"
//--------------------------------------------------------------------------
void displayScore(int x, int y)
{
  for (int h = 0; h < word_score_height; h++)
  {
    for (int w = 0; w < word_score_width; w++)
    {
      // drawPixelARGB32(x + w, y + h, word_score_image[h * word_score_width + w]);

      // unsigned char mask = 1 << j;
      unsigned long mask = 0xffff;

      unsigned char *glyph = (unsigned char *)&word_score_image[h * word_score_width + w];
            
      // Get the coordinate to assign color - green
      unsigned char col = (*glyph & mask) ? (4 & 0xff) : 1;

      // Move to next column until reach the end of bitmap epd_bitmap_allArray
      if (col != 1) {
        // Fill pixel with current column while neglecting the background pixel
        if ((unsigned char *)&word_score_image[h * word_score_width + w] != 0x0000){
          drawPixel(x + w, y + h, col);
        }
      }
    }
  }
}


//=======================================================================================//
//                          FUNCTION DISPLAY EXPLOSION EFFECT                            //
//=======================================================================================//
// Function to display the player explosion
//--------------------------------------------------------------------------
void displayExplosion(int x, int y)
{
  for (int h = 0; h < explosion_height; h++)
  {
    for (int w = 0; w < explosion_width; w++)
    {
      drawPixelARGB32noBackground(x + w, y + h, explosion_image[h * explosion_width + w]);
    }
  }
}
// Function to display the asteroid explosion
//--------------------------------------------------------------------------
void displayExplosion2(int x, int y)
{
  for (int h = 0; h < explosion2_height; h++)
  {
    for (int w = 0; w < explosion2_width; w++)
    {
      if (explosion2_image[h * explosion2_width + w]!=0x00000000)
      drawPixelARGB32noBackground(x + w, y + h, explosion2_image[h * explosion2_width + w]);
    }
  }
}


//=======================================================================================//
//                                FUNCTION TO CLEAR IMAGE                                //
//=======================================================================================//
// Function to clear score
//--------------------------------------------------------------------------
void clearScore(int num, int x, int y)
{
  for (int h = 0; h < 50; h++)
  {
    for (int w = 0; w < 50; w++)
    {
      drawPixelARGB32(x + w, y + h, 0);
    }
  }
}
// Function to clear lives
//--------------------------------------------------------------------------
void clearPlayerLife(int x, int y)
{
  for (int h = 0; h < life_height; h++)
  {
    for (int w = 0; w < life_width; w++)
    {
      drawPixelARGB32(x + w, y + h, 0);
    }
  }
}


//=======================================================================================//
//                           FUNCTION TO DISPLAY/CLEAR LIVES                             //
//=======================================================================================//
// Function to display word "lives"
//--------------------------------------------------------------------------
void displayWordPlayerLife(int x, int y)
{
  for (int h = 0; h < word_lives_height; h++)
  {
    for (int w = 0; w < word_lives_width; w++)
    {
      // drawPixelARGB32(x + w, y + h, word_lives_image[h * word_lives_width + w]);

      // unsigned char mask = 1 << j;
      unsigned long mask = 0xffff;

      unsigned char *glyph = (unsigned char *)&word_lives_image[h * word_lives_width + w];
            
      // Get the coordinate to assign color - bright magenta
      unsigned char col = (*glyph & mask) ? (15 & 0xff) : 1;

      // Move to next column until reach the end of bitmap epd_bitmap_allArray
      if (col != 1) {
        // Fill pixel with current column while neglecting the background pixel
        if ((unsigned char *)&word_lives_image[h * word_lives_width + w] != 0x0000){
          drawPixel(x + w, y + h, col);
        }
      }
    }
  }
}

// Function to display player live symbol (3 in total)
//--------------------------------------------------------------------------
void displayPlayerLife(int x, int y)
{
  for (int h = 0; h < life_height; h++)
  {
    for (int w = 0; w < life_width; w++)
    {
      drawPixelARGB32(x + w, y + h, life_image[h * life_width + w]);
    }
  }
}


//=======================================================================================//
//                           FUNCTION TO DISPLAY ENDGAME                                 //
//=======================================================================================//
// Function to display game win image
//--------------------------------------------------------------------------
void displayGameWinImage(int x, int y)
{
  for (int h = 0; h < game_win_height_image; h++)
  {
    for (int w = 0; w < game_win_width_image; w++)
    {
      drawPixelARGB32(x + w, y + h, game_win_image[h * game_win_width_image + w]);
    }
  }
}

// Function to display game over image
//--------------------------------------------------------------------------
void displayGameOverImage(int x, int y)
{
  for (int h = 0; h < game_over_height_image; h++)
  {
    for (int w = 0; w < game_over_width_image; w++)
    {
      drawPixelARGB32(x + w, y + h, game_over_image[h * game_over_width_image + w]);
    }
  }
}


//=======================================================================================//
//                           FUNCTION TO DISPLAY ENDGAME                                 //
//=======================================================================================//
// Function to display space ship image without drawing green pixels
//--------------------------------------------------------------------------
void displaySpaceShipImage(int x, int y)
{
  for (int h = 0; h < spaceship_height; h++)
  {
    for (int w = 0; w < spaceship_width; w++)
    {
      // Get the pixel color
      uint32_t pixelColor = spaceship_image[h * spaceship_width + w];
      
      // Check if it's not black (assuming green is 0x00FF00FF in ARGB32)
      if (pixelColor != 0x00000000)
      {
        // Draw the pixel if it's not black
        drawPixelARGB32(x + w, y + h, pixelColor);
      }
    }
  }
}