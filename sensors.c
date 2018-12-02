#include "sensors.h"
#include "interrupts.h"
#include "gpio.h"
#include "printf.h"
#include <stdbool.h>
#include "timer.h"


//////////////// Temperature & Humidity  Sensor ////////////////
#define MAX_PIN_FLIPS 85       // Max number of times the pin switches between 0 or 1
#define TIMEOUT_COUNT 255      // Used to exit loop if timeout occurs while waiting for data 
#define TEMP_PIN  GPIO_PIN26

static void init_temp_sensor(){
    // TODO
}

static void init_prox_sensor(){
    // TODO
}

static void init_rtc_sensor(){
    // TODO
}

/*
 This function uses TEMP_PIN set to output to request data from the DHT11 temperature and 
 humidity sensor. The function then sets TEMP_PIN to input and monitors it for flips between
 0 and 1. Flips 0 - 3 are part of ACK sequence after requesting data from the sensor.
 Flip 4 is the first data bit. Even-numbered flips are data bits while odd-numbered flips 
 are the pauses (low) that precede each data bit. 
*/
bool read_temp_data(char* resultBuf, unsigned int bufLen) {
    unsigned int counter = 0;
    unsigned int n_bits = 0; 
    int temp_data[5] = { 0 };

    // Request data from sensor
    gpio_set_output(TEMP_PIN);
    gpio_write(TEMP_PIN, 0);
    timer_delay_ms(18);
    gpio_write(TEMP_PIN, 1);
    timer_delay_us(40);
    gpio_set_input( TEMP_PIN);

    int last_state = 1;        // rpi sets pin 1 then waits for DHT to set it 0
 
    for (int flips = 0; flips < MAX_PIN_FLIPS; flips++ ) {
        counter = 0;
        while (gpio_read(TEMP_PIN) == last_state ) { // Spine while waiting for pin to flip 
            counter++;
            timer_delay_us(1);
            if (counter == TIMEOUT_COUNT) break; 
        }
        last_state = gpio_read( TEMP_PIN );
        
        if (counter == TIMEOUT_COUNT) break;
 
        // Flips 0-3 are part of ACK. Odd-numbered bits are the pause before data bits. 
        if ( (flips >= 4) && (flips % 2 == 0) ) {
            temp_data[n_bits / 8] <<= 1;
            if ( counter > 16 )
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

// Initialize everything necessary for sensors
void sensors_init(void){
    // TODO - setup gpio pins
    
    // TODO - setup interrupts


    init_temp_sensor();
    init_prox_sensor();
    init_rtc_sensor();
}
