#include "helper.h"
#include "framebf.h"

// Function to clear the emulator screen before showing new thing
void clear_emulator_screen(int display_width, int display_height) {
  // Clear both width and height of the screen
  for (int h = 0; h < display_height; h++) {
    for (int w = 0; w < display_width; w++) {
      drawPixel(w, h, 0);
    }
  }
}

// Generate a function to find the length of the string
int string_length(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    i++;
  }
  return i;
}

// Generate a function convert integer to string and return string
void integer_to_string(int n, char *str)
{
  int i = 0;
  while (n != 0)
  {
    int rem = n % 10;
    str[i] = rem + '0';
    n = n / 10;
    i++;
  }
  str[i] = '\0';
  // reverse the string
  int j = 0;
  int k = string_length(str) - 1;
  while (j < k)
  {
    char temp = str[j];
    str[j] = str[k];
    str[k] = temp;
    j++;
    k--;
  }
}



