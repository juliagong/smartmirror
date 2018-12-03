#include "gl.h"
#include "fb.h"
#include "font.h"
#include "strings.h"

#define FOREGROUND_PIXEL 0xff

void gl_draw_char_with_size(int x, int y, int ch, color_t c, int scale)
{
    int font_size = font_get_size();
    int char_height = gl_get_char_height();
    int char_width = gl_get_char_width();
    
    unsigned char buf[font_size];
    if (font_get_char(ch, buf, font_size)) {
        for (int i = 0; i < char_height; i++) {
            for (int j = 0; j < char_width; j++) {
                if (buf[i * gl_get_char_width() + j] == (unsigned char) FOREGROUND_PIXEL) {
		    for (int k = 0; k < scale; k++) {
			for (int l = 0; l < scale; l++) {
			    gl_draw_pixel((scale * j) + x + k, (scale * i) + y + l, c);
			}
		    }
                }
            }
        }
    }
}

void gl_draw_string_with_size(int x, int y, char* str, color_t c, int scale)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        gl_draw_char_with_size(i * gl_get_char_width() * scale + x, y, str[i], c, scale);
    }
}