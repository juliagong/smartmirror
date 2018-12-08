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
                if (buf[i * char_width + j] == (unsigned char) FOREGROUND_PIXEL) {
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
    int char_width = gl_get_char_width();
    for (int i = 0; i < len; i++) {
        gl_draw_char_with_size(i * char_width * scale + x, y, str[i], c, scale);
    }
}

void gl_draw_empty_rect(int x, int y, int w, int h, color_t c, unsigned int lineWidth){
    if (w < 0 || h < 0) {
        return;
    }

    unsigned int maxX = x + w >= gl_get_width() ? gl_get_width(): x + w;
    unsigned int maxY = y + h >= gl_get_height() ? gl_get_height(): y + h;
    unsigned int depth = fb_get_depth() / 8;
    unsigned int pitch = fb_get_pitch();
    unsigned char* fb = fb_get_draw_buffer();

    unsigned int baseVal;
    unsigned int index;

    for (int j = y; j < maxY; j++){
        baseVal = j * pitch;

        // top and bottom boundary
        if (j < y + lineWidth || j >= maxY - lineWidth){
            for (int i = x; i < maxX; i++){
                index = baseVal + i * depth;
                *(unsigned int*)(fb + index) = c;
            }
        } else {
            for (int i = x; i < x + lineWidth; i++){
                index = baseVal + i * depth;
                *(unsigned int*)(fb + index) = c;
            }

            for (int i = maxX - lineWidth; i < maxX; i++){
                index = baseVal + i * depth;
                *(unsigned int*)(fb + index) = c;
            }
        }
    }
}
