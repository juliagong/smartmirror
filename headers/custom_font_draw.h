#ifndef GL_H
#define GL_H

/* Draw single character at location x,y in color c with specified font size,
 * where `scale` is the number of times larger than the default size.
 */
void gl_draw_char_with_size(int x, int y, int ch, color_t c, int scale);

/*
 * Draw a string at location x,y in color c with specified font size,
 * where `scale` is the number of times larger than the default size.
 */
void gl_draw_string_with_size(int x, int y, char* str, color_t c, int scale);

#endif
