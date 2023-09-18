// ----------------------------------- function.c ------------------------------------- //

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "game_background.h"
#include "game_universe_background.h"


//====================================================================================//
//									                  FUNCTIONS										                    //
//====================================================================================//
// Declare reset_arr to reset the cli buffer
//--------------------------------------------------------------------------------
void reset_arr(char* arr){
	int i = 0;
	while(arr[i] != '\0') {
		arr[i] = '\0';
		i++;
	 }
}
// Function to reset buffer
//--------------------------------------------------------------------------------
void reset_ptr(const char* cli_buffer){
	while(*cli_buffer != '\n'){
		cli_buffer++;
	}
}
// Function to compare input cli
//--------------------------------------------------------------------------------
int comp_str(const char* s1,const char* s2){	
	while (*s1 != '\0' && *s2 != '\0') {
		// Check if s1 no equal to s2
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        }
        s1++;
        s2++;
    }

    // At this point, one or both strings have ended
    if (*s1 == '\0' && *s2 == '\0') {
        return 0; 	// Both strings are equal
    } else if (*s1 == '\0') {
        return -1; 	// s1 is Null
    } else {
        return 1; 	// s2 is either Null or no equal s1
    }
}

// Function return absolute int value
//--------------------------------------------------------------------------------
int abs(int x)
{
    return (x < 0) ? -x : x;
}

//====================================================================================//
//								                IMAGE VIDEO FONT    							              	  //
//====================================================================================//
// Function to display the screen background image
//--------------------------------------------------------------------------------
void displayGameBackground(int x, int y){
  for (int h = 0; h < background_height; h++)
  {
    for (int w = 0; w < background_width; w++)
    {
      drawPixelARGB32(x + w, y + h, background_image[h * background_width + w]);
    }
  }
}
// Function to display the screen background universe image
//--------------------------------------------------------------------------------
void displayGameUniverseBackground(int x, int y){
  for (int h = 0; h < universe_background_height; h++)
  {
    for (int w = 0; w < universe_background_width; w++)
    {
      drawPixelARGB32(x + w, y + h, background_universe_image[h * universe_background_width + w]);
    }
  }
}
// Function to clear emulator screen
//--------------------------------------------------------------------------------
void clearscreen(int x, int y){
  for (int h = 0; h < background_height; h++)
  {
    for (int w = 0; w < background_width; w++)
    {
      drawPixelARGB32(x + w, y + h, 0x00000000);
    }
  }
}

//====================================================================================//
//										                DISPLAY MENU							    		  	          //
//====================================================================================//
// Function to display commands menu
//--------------------------------------------------------------------------------
void displayMenu()
{
    uart_puts("\n----------------------------------------------------------------------------------------------\n");
    uart_puts(
        "\n\tEnter a number to choose command:\n"
        "\t1.\tDisplay text on screen\n"
        "\t2.\tDisplay a small image\n"
        "\t3.\tDisplay a scrollable large image\n"
        "\t4.\tDisplay a video\n"
        "\t5.\tPlay game\n"
        "\t0.\tClear the screen\n");
    uart_puts("\n----------------------------------------------------------------------------------------------\n");
}

//====================================================================================//
//									                  WELCOME SCREEN									                //
//====================================================================================//
void welcome_text(){
	uart_puts("\n----------------------------------------------------------------------------------------------\n\n");
	uart_puts("\x1b[38;5;215md88888b d88888b d88888b d888888b .d888b.   j88D  .d888b.  .d88b.  "  );uart_puts("\n");
				  uart_puts("88'     88'     88'     `~~88~~' VP  `8D  j8~88  88' `8D .8P  88. "  );uart_puts("\n");
				  uart_puts("88ooooo 88ooooo 88ooooo    88       odD' j8' 88  `V8o88' 88  d'88 "  );uart_puts("\n");
				  uart_puts("88~~~~~ 88~~~~~ 88~~~~~    88     .88'   V88888D    d8'  88 d' 88 "  );uart_puts("\n");
				  uart_puts("88.     88.     88.        88    j88.        88    d8'   `88  d8' "  );uart_puts("\n");
				  uart_puts("Y88888P Y88888P Y88888P    YP    888888D     VP   d8'     `Y88P'  "  );uart_puts("\n");
				  uart_puts(" \n\n\n");
				  uart_puts("d8888b.  .d8b.  d8888b. d88888b    .d88b.  .d8888.   ");uart_puts("\n");
				  uart_puts("88  `8D d8' `8b 88  `8D 88'       .8P  Y8. 88'  YP   ");uart_puts("\n");
				  uart_puts("88oooY' 88ooo88 88oobY' 88ooooo   88    88 `8bo.     ");uart_puts("\n");
				  uart_puts("88~~~b. 88~~~88 88`8b   88~~~~~   88    88   `Y8b.   ");uart_puts("\n");
				  uart_puts("88   8D 88   88 88 `88. 88.       `8b  d8' db   8D   ");uart_puts("\n");
				  uart_puts("Y8888P' YP   YP 88   YD Y88888P    `Y88P'  `8888Y'   ");uart_puts("\n");
				  uart_puts("\n\n");
				  uart_puts("     \x1b[33mDeveloped by GROUP 12");
				  uart_puts("\n\n\x1b[37m");
	uart_puts("\n----------------------------------------------------------------------------------------------\n");
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
