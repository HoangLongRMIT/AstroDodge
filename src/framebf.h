// ----------------------------------- framebf.h -------------------------------------


void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLineARGB32(int x1, int y1, int x2, int y2, unsigned int attr);
void drawCircleARGB32(int centerX, int centerY, int radius, unsigned int attr);


//==================================================================================================//
//                                      FONT CONVERSION                                             //
//==================================================================================================//
// Function to draw pixel
void drawPixel(int x, int y, unsigned char attr);
// Function print string with font
void drawString(int x, int y, char *s, char* color);
// Function print content with font
void font();