#include "sensors.h"
#include "interrupts.h"
#include "gpio.h"
#include "printf.h"
#include <stdbool.h>
#include "timer.h"
#include "gpioextra.h"
#include "assert.h"

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
    return gpio_read(MOTION_PIN) > 0;
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
bool read_temp_data(char* resultBuf, unsigned int bufLen) {
    unsigned int count = 0;
    unsigned int n_bits = 0; 
    int temp_data[5] = { 0 };
    float fahrenheit = 0.0;

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
        fahrenheit = temp_data[2] * 9. / 5. + 32;
        int f_int = fahrenheit;                 // Holds the integer portion of the float.
        int f_frac = (fahrenheit - f_int) * 10; // Stores one decimal place
        snprintf(resultBuf, bufLen, "Temperature = %d.%d C / %d.%d F  Humidity = %d.%d%% \n", temp_data[2], temp_data[3], f_int, f_frac, temp_data[0], temp_data[1] );
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
bool is_rotary_clicked(){
   bool rotary_clicked_val = rotary_clicked;
   rotary_clicked = false;

   return rotary_clicked_val;
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
