#include "sensors.h"
#include "interrupts.h"
#include "gpio.h"
#include "printf.h"
#include <stdbool.h>
#include "timer.h"
#include "gpioextra.h"
#include "assert.h"

// Motion Sensor
#define MOTION_PIN GPIO_PIN26

// Temperature & Humidity Sensor
#define MAX_PIN_FLIPS 85 // max number of times the pin switches between 0 or 1
#define TIMEOUT_COUNT 255 // used to exit loop if timeout occurs while waiting for data 
#define TEMP_PIN GPIO_PIN26

//https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/Rotary.cpp
// Rotary Encoder
// TODO - when we change this pins, remember to change the interrupts_enable_source(...) in interrupts_init
#define ROTARY_CLK GPIO_PIN17
#define ROTARY_DT GPIO_PIN18

#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

#define R_START 0x0
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

static unsigned char state;
static int rotation;

static void init_rotary_encoder() {
    gpio_set_input(ROTARY_CLK);
    gpio_set_pullup(ROTARY_CLK);
    
    gpio_set_input(ROTARY_DT);
    gpio_set_pullup(ROTARY_DT);

    rotation = 0;
    state = 0;
}


int read_rotary_data() {
    printf("Called at sensors.c :%d\n", rotation);
    int current_rotation = (int)rotation;
    rotation = 0;

    return current_rotation;
}

bool read_temp_data(char* resultBuf, unsigned int bufLen){
    return false;
}

bool read_motion_data(){
    return false;
}

static unsigned char process(){
    // Grab state of input pins.
    unsigned int pin1 = gpio_read(ROTARY_CLK);
    unsigned int pin2 = gpio_read(ROTARY_DT);
    unsigned char pinState = (pin1 << 1) | pin2;

    // Determine new state from the pins and state table.
    state = ttable[state & 0xf][pinState];

    // Return emit bits, ie the generated event.
    return state & 0x30;
}

static void rotary_interrupt(unsigned int pc) {
    bool ok = gpio_check_event(ROTARY_CLK);
    bool ok2 = gpio_check_event(ROTARY_DT);

    if (ok || ok2) { 
        unsigned char result = process();

        if (result == DIR_CW){
            rotation += 1;
        } else if (result == DIR_CCW){
            rotation -= 1;
        }
      
        if (ok) {
            gpio_clear_event(ROTARY_CLK);
        } 

        if (ok2) {
            gpio_clear_event(ROTARY_DT);
        }
    }
}

static void init_interrupts() {
    gpio_enable_event_detection(ROTARY_CLK, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(ROTARY_DT, GPIO_DETECT_FALLING_EDGE);
    
    gpio_enable_event_detection(ROTARY_CLK, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection(ROTARY_DT, GPIO_DETECT_RISING_EDGE);
    
    bool ok = interrupts_attach_handler(rotary_interrupt);
    assert(ok);

    interrupts_enable_source(INTERRUPTS_GPIO3);
    interrupts_global_enable();
}

// initialize everything necessary for sensors
void sensors_init(void) {
    gpio_init();

    init_rotary_encoder();

    init_interrupts();
}

