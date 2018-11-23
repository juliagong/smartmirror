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
* This function requests temperature and humidity data from the DHT11 sensor. 
* There is no other init function for this sensor.
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
* Subsequent odd-numbered flips are when TEMP_PIN is set to 0 preceding each data bit. 
* Even-numbered flips are when TEMP_PIN is set to 1 for each data bit. 
*/
bool read_temp_data(char* resultBuf, unsigned int bufLen) {
    unsigned int count = 0;
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
        count = 0;
        while (gpio_read(TEMP_PIN) == last_state ) { // Spin while waiting for pin to flip 
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
        snprintf(resultBuf, bufLen, "Temperature = %d.%d C Humidity = %d.%d%% \n", temp_data[2], temp_data[3], temp_data[0], temp_data[1] );
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
