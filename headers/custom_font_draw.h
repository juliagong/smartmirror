#ifndef FONTDRAW_H
#define FONTDRAW_H

/* Draw single character at location x,y in color c with specified font size,
 * where `scale` is the number of times larger than the default size.
 */
void gl_draw_char_with_size(int x, int y, int ch, color_t c, int scale);

/*
 * Draw a string at location x,y in color c with specified font size,
 * where `scale` is the number of times larger than the default size.
 */
void gl_draw_string_with_size(int x, int y, char* str, color_t c, int scale);

/*
 * Draw a rectangle of size w * h at location x, y with boundary of width lineWidth and color c.
 */
void gl_draw_empty_rect(int x, int y, int w, int h, color_t c, unsigned int lineWidth);

#endif
