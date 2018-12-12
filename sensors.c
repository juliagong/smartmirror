#include "sensors.h"
#include "interrupts.h"
#include "gpio.h"
#include "printf.h"
#include <stdbool.h>
#include "timer.h"
#include "gpioextra.h"
#include "assert.h"
#include "malloc.h"
#include "strings.h"
#include "uart.h"
#include "output_formatter.h"

// TODO - remove this
#include "gl.h"


// Motion Sensor
#define MOTION_PIN GPIO_PIN19

// Temperature & Humidity Sensor
#define MAX_PIN_FLIPS 85 // max number of times the pin switches between 0 or 1
#define TIMEOUT_COUNT 255 // used to exit loop if timeout occurs while waiting for data 
#define TEMP_PIN GPIO_PIN26

// Rotary Encoder
#define ROTARY_CLK GPIO_PIN16
#define ROTARY_DT GPIO_PIN20
#define ROTARY_SW GPIO_PIN21

// Time 
#define TIME_BUF_LEN 71

static volatile unsigned int prev_clk_val;
static volatile int rotation;
static volatile bool rotary_clicked;

// Sensor Initialization
static void init_motion_sensor() {
    gpio_set_input(MOTION_PIN);
}

static void init_rtc_sensor() {
    // TODO
}
        
static void init_rotary_encoder() {
    gpio_set_input(ROTARY_CLK);
    gpio_set_pullup(ROTARY_CLK);
    
    gpio_set_input(ROTARY_DT);
    gpio_set_pullup(ROTARY_DT);

    gpio_set_input(ROTARY_SW);
    gpio_set_pullup(ROTARY_SW);

    prev_clk_val = gpio_read(ROTARY_CLK);
    rotation = 0;
    rotary_clicked = false;
}


bool read_motion_data() {
    return true;
    // TODO - uncomment this to do it actually
    //return gpio_read(MOTION_PIN) > 0;
}

/**
 * Function: read_temp_data
 *
 * Requests temperature and humidity data from the DHT11 sensor. 
 * There is no other init function for this sensor.
 *
 * The function requests the data by setting TEMP_PIN to output and writing 0 then 1 to
 * the pin. TEMP_PIN is then set to input so the DHT11 can acknowledge the request by 
 * setting TEMP_PIN to 0, then 1. DHT11 then alternates between setting TEMP_PIN to 0
 * preceding each data bit (for a total of 40 bits), and then 1 for each data bit. 
 *
 * If TEMP_PIN is set to 1 for count > 16, the data bit is a 1. 
 * If TEMP_PIN is set to 1 for a count <= 16, the data bit is a 0.
 * 
 * The function trakcs when  the TEMP_PIN flips between 0 and 1, and for how long.
 * Flips 0 - 3 are part of ACK sequence after requesting data from the sensor.
 * Flip 4 is the first data bit. 
 *
 * Subsequent odd-numbered flips are when TEMP_PIN is set to 0 preceding each data bit. 
 * Even-numbered flips are when TEMP_PIN is set to 1 for each data bit. 
**/
bool read_temp_data(char** resultBuf, unsigned int bufLen, unsigned int settingId, unsigned int subsettingId) {
    unsigned int count = 0;
    unsigned int n_bits = 0; 
    int temp_data[5] = { 0 };

    // request data from sensor
    gpio_set_output(TEMP_PIN);
    gpio_write(TEMP_PIN, 0);
    timer_delay_ms(18);
    gpio_write(TEMP_PIN, 1);
    timer_delay_us(40);
    gpio_set_input(TEMP_PIN);

    int last_state = 1; // rpi sets pin 1 then waits for DHT to set it 0
 
    for (int flips = 0; flips < MAX_PIN_FLIPS; flips++ ) {
        count = 0;
        while (gpio_read(TEMP_PIN) == last_state ) { // spin while waiting for pin to flip 
            count++;
            timer_delay_us(1);
            if (count == TIMEOUT_COUNT) break;
        }
        last_state = gpio_read( TEMP_PIN );
        
        if (count == TIMEOUT_COUNT) break;
 
        // Flips 0-3 are part of ACK. Odd-numbered flips are the pause before data bits.
        // Only even-numbered flips are data bits. 
        if ( (flips >= 4) && (flips % 2 == 0) ) {
            temp_data[n_bits / 8] <<= 1;
            if ( count > 16 )
                temp_data[n_bits / 8] |= 1;
            n_bits++;
        }
    }
    
    unsigned int checksum = (temp_data[0] + temp_data[1] + temp_data[2] + temp_data[3]) & 0xFF; 
    
    if ( (n_bits >= 40) && (temp_data[4] == checksum ) ) {
        format_temperature_data(resultBuf, bufLen, temp_data, settingId, subsettingId);
        return true;
    }

    return false;
}

// read the amount of rotation and return
int read_rotary_data() {
    int current_rotation = (int)rotation / 2; // divide by 2 as every rotation seems to increment count by twice
    rotation = 0;

    return current_rotation;
}

// check if rotary has been clicked or not
bool is_rotary_clicked() {
   bool rotary_clicked_val = rotary_clicked;
   rotary_clicked = false;

   return rotary_clicked_val;
}

// reset rotary click action
void reset_rotary_click() {
    rotary_clicked = false;
}

/*
 * Interrupt handler for rotary
 *
 * We detect that there has been rotation by observing if value of ROTARY_CLK has changed or not
 * We detect the direction of the motion as following:
 *  - If ROTARY_CLK switched before ROTARY_DT, then we are turning counter-clockwise
 *  - Otherwise, we are turning clockwise 
 */
static void rotary_interrupt(unsigned int pc) {
    bool ok = gpio_check_event(ROTARY_CLK);
    bool ok_sw = gpio_check_event(ROTARY_SW);
    
    if (ok) { 
        unsigned int clk_val = gpio_read(ROTARY_CLK);
      
        if (clk_val != prev_clk_val) {
            unsigned int dt_val = gpio_read(ROTARY_DT);
            rotation += (clk_val == dt_val ? 1 : -1); 
        
            prev_clk_val = clk_val;
        } 
       
        gpio_clear_event(ROTARY_CLK);
    }

    if (ok_sw) {
        rotary_clicked = true;
        gpio_clear_event(ROTARY_SW);
    }
}

/*
 * Initialize all the interrupts
 */
static void init_interrupts() {
    gpio_enable_event_detection(ROTARY_CLK, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(ROTARY_CLK, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection(ROTARY_SW, GPIO_DETECT_FALLING_EDGE);

    bool ok = interrupts_attach_handler(rotary_interrupt);
    assert(ok);

    interrupts_enable_source(INTERRUPTS_GPIO3);
    interrupts_global_enable();
}

// initialize everything necessary for sensors
void sensors_init(void) {
    gpio_init();

    init_motion_sensor();
    init_rtc_sensor();
    init_rotary_encoder();

    init_interrupts();
}


/*
 Returns the number of chars written to buf
*/
static int uart_getline(char *buf, int bufsize) {
    if (!uart_haschar()) {
        return 0;
    }

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
            arr[ntokens] = strndup(start+1, nchars);   // make heap-copy, add to array 
            ntokens++; 
        }
    }
    return ntokens;
}


/*
date_time[] array holds 7 pieces of information:
    Day of the Week
    Month Name
    Month
    Day Number
    Year Number
    HH
    MM
    SS
*/

int read_date_time(char** resultBuf, unsigned int bufLen, unsigned int settingId, unsigned int subsettingId) {
    char *line = (char *)malloc (TIME_BUF_LEN);
  
    int len = uart_getline(line, TIME_BUF_LEN);
    if (len == 0) return 0;

    // Tokenize
    char *date_time[7]; 
    int ntokens = tokenize(line, date_time, len); 

    if (ntokens > 0) {
        // Format date and time data according to our defined settings
        format_date_data(resultBuf[0], bufLen, date_time, settingId);
        format_time_data(resultBuf[1], bufLen, date_time, subsettingId);
    } else {
        snprintf(resultBuf[0], bufLen, "Connection failed");
        snprintf(resultBuf[1], bufLen, "Connection failed");
    }

    // Free array
    for(int i = 0; i < ntokens; i++) {
        free((char *)date_time[i]);
    }

    return ntokens; 
}
