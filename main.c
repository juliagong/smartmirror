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



/*
 Returns the number of chars written to buf
*/
static int uart_getline(char *buf, int bufsize) {
    char c = uart_getchar(); 
    int index = 0; 
    
    while (c != '\0' && index < bufsize-1) {
        buf[index] = c; 
        index++;
        c = uart_getchar(); 
    }
    buf[index] = '\0';
 
    return index; 
}


/** From Lab 4
 * Allocate space on the heap for n chars plus null-terminator.
 * Copy n bytes fro
 */
static char *strndup(const char *src, int n) {
    char *str = malloc(n+1);
    for (int i = 0; i < n; i++) {
        str[i] = src[i];
    }
    str[n] = '\0';
    return str; 
}

/** From Lab 4
* Returns 1 if 'ch' is whitespace and 0 otherwise. 
*/
static int isspace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

/** From Lab 4
* Parses 'line' into tokens separated by spaces and stores them into array of char*
* pointing to strings on the heap. 
*/
static int tokenize(const char *line, char *arr[], int max) {
    int ntokens = 0; 

    while (*line != '\0' && ntokens < max) {
        while (isspace(*line)) line++;  // skip past spaces
        if (*line == '\0') break; // no more non-white chars
        const char *start = line;
        while (*line != '\0' && !isspace(*line)) line++; // advance to next space/null
        if (*start == '*') {
            int nchars = line - start - 1;      
            arr[ntokens] = strndup(start + 1, nchars);   // make heap-copy, add to array 
            ntokens++; 
        }
    }
    return ntokens;
}


/*
date_time[] array holds 7 pieces of information:
    Day of the Week
    Month Name
    Day Number
    Year Number
    HH
    MM
    SS
*/
int read_date_time_simple() {
// Send request to esp-32
    uart_putchar('t');
    timer_delay_ms(200);

    char *line = (char *)malloc (200);
  
    int len = uart_getline(line, 100);
    if (len == 0) return 0;
    // gl_draw_string(0,0, line, GL_BLUE);

    // Tokenize
    char *date_time[8]; 
    int ntokens = tokenize(line, date_time, len); 

    // Print to display for testing purposes
    int height = gl_get_char_height();  
    for (int i = 0; i < ntokens; i++) {
        gl_draw_string(5, 15 + height * i, date_time[i], GL_GREEN);
    }

    // Free array
    for(int i = 0; i < ntokens; i++) {
        free((char *)date_time[i]);
    }

    return ntokens; 
}


int read_weather_simple() {
// Send request to esp-32
    uart_putchar('w');
    timer_delay_ms(100);

    char *line = (char *)malloc (100);
  
    int len = uart_getline(line, 100);
    if (len == 0) return 0;
    // gl_draw_string(0,0, line, GL_BLUE);

    // Tokenize
    char *weather[6]; 
    int ntokens = tokenize(line, weather, len); 

    // Print to display for testing purposes
    int height = gl_get_char_height();  
    for (int i = 0; i < ntokens; i++) {
        gl_draw_string(5, 15 + height * i, weather[i], GL_GREEN);
    }

    // Free array
    for(int i = 0; i < ntokens; i++) {
        free((char *)weather[i]);
    }

    return ntokens; 
}

int split_lines(const char *buf, char *arr[], int max, int maxLineLength) {
    int ntokens = 0; 
    while (*buf != '\0' && ntokens < max) {    
        while (*buf != '\0' && *buf != '*') buf++; 

        if (*buf == '\0') return 0;

        char *start = (char*)buf; // Start points to * 

        while (*buf !='\0' && *buf != '^') buf++;   // buf points to ^ or end

        int nchars = buf - start - 1; 
        int nCharsToRead = nchars < maxLineLength - 3 ? nchars : maxLineLength - 3;

        arr[ntokens] = strndup(start + 1, nCharsToRead);
        if (nchars > maxLineLength - 3) {
            memcpy(arr[ntokens] + maxLineLength - 3, "...",3);
        }
        arr[ntokens][maxLineLength] = '\0';
       
        gl_draw_string(50, 15 + 30 * ntokens, arr[ntokens], GL_RED);

        ntokens++;
    }


    return ntokens;
}      

int read_headlines_simple() {
// Send request to esp-32
    uart_putchar('h');
    timer_delay_ms(100);

    char *buf = (char *)malloc (1024);
  
    int len = uart_getline(buf, 1024);
    if (len == 0) return 0;
    // gl_draw_string(0,0, buf+50, GL_BLUE);

    // Tokenize
    char *headlines[10]; 
    int ntokens = split_lines(buf, headlines, len, 30); 
    // int ntokens = tokenize(buf, headlines, len); 

    // Print to display for testing purposes
    int height = gl_get_char_height();  
    for (int i = 0; i < ntokens; i++) {
        gl_draw_string(5, 15 + height * i, headlines[i], GL_GREEN);
    }

    // Free array
    for(int i = 0; i < ntokens; i++) {
        free((char *)headlines[i]);
    }

    return ntokens; 
}

void main(void)
{
    uart_init();
    gpio_init();
    timer_init();
    gl_init(1024, 768, GL_DOUBLEBUFFER);
    printf("init complete. Switch to esp-32\n");
    timer_delay(2); 

    while(1) {
       gl_clear(GL_BLACK);
       read_date_time_simple();
       gl_swap_buffer(); 
       timer_delay(1);

       gl_clear(GL_BLACK);
       read_weather_simple();
       gl_swap_buffer(); 
       timer_delay(1);

       gl_clear(GL_BLACK);
       read_headlines_simple();
       gl_swap_buffer(); 
       timer_delay(1);
    }    

}

