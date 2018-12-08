#include "printf.h"
#include "gpio.h"
#include "timer.h"
#include "sensors.h"
#include "uart.h"
#include "gl.h"
#include "malloc.h"
#include "strings.h"

// #define MOTION_PIN GPIO_PIN26

// void setup_motion_sensor(void)
// {
//     gpio_set_input(MOTION_PIN);
// }

// void check_for_motion(void)
// {
//     timer_delay(2);
//     printf("Warmed up! Checking for motion...\n");
//     int count = 0;
//     while (1) {
//     	if (gpio_read(MOTION_PIN)) {
//             printf("Motion detected %d!\n", count + 1);
//             timer_delay(1);
//             count++;
//     	}
//     }
// }
 

/*
* Draws the string in color c to (x,y).
* Assumes string is properly null-terminated. 
*/ 

// unsigned int gl_get_char_height(void) {
//     return (unsigned int)font_get_height();
// }

// unsigned int gl_get_char_width(void) {
//     return (unsigned int)font_get_width(); 
// }

// void gl_draw_string(int x, int y, char* str, color_t c){
//     int width = gl_get_char_width();
//     int len = strlen(str);

//     for (int i = 0; i < len; i++) {
//         gl_draw_char(x + (width * i), y, str[i], c);
//     }
// }




 int read_date_time();

void main(void)
{
    uart_init();
    gpio_init();
    timer_init();
    gl_init(1024, 768, GL_DOUBLEBUFFER);
    printf("init complete. Switch to esp-32\n");

    while(1) {
       gl_clear(GL_BLACK);
       read_date_time();
       gl_swap_buffer(); 
    }    

}

