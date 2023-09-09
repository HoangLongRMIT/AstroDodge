#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "game_background.h"

void displayGameBackground(int x, int y){
  for (int h = 0; h < background_height; h++)
  {
    for (int w = 0; w < background_width; w++)
    {
      drawPixelARGB32(x + w, y + h, background_image[h * background_width + w]);
    }
  }
}
void clearscreen(int x, int y){
  for (int h = 0; h < background_height; h++)
  {
    for (int w = 0; w < background_width; w++)
    {
      drawPixelARGB32(x + w, y + h, 0x00000000);
    }
  }
}
