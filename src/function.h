// ----------------------------------- function.h ------------------------------------- //


//====================================================================================//
//									FUNCTION										  //
//====================================================================================//
// Function to reset array
void reset_arr(char* arr);
// Function to reset the pointer
void reset_ptr(const char* cli_buffer);
// Compare string to string
int comp_str(const char* s1, const char* s2);
// Function return absolute int value
int abs(int x);

//====================================================================================//
//								    DISPLAY MENU							    	  //
//====================================================================================//
// Function to display commands menu
void displayMenu();


//====================================================================================//
//								   IMAGE VIDEO FONT    								  //
//====================================================================================//
// Function to display the screen background image
void displayGameBackground(int x, int y);
// Function to clear emulator screen
void clearscreen(int x, int y);


//====================================================================================//
//									WELCOME SCREEN									  //
//====================================================================================//
//Welcome text
void welcome_text();



