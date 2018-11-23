#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

//////////////// Motion Sensor ////////////////
#define MOTION_PIN GPIO_PIN26

void setup_motion_sensor(void)
{
    gpio_set_input(MOTION_PIN);
}

void check_for_motion(void)
{
    timer_delay(2);
    printf("Warmed up! Checking for motion...\n");
    int count = 0;
    while (1) {
    if (gpio_read(MOTION_PIN)) {
        printf("Motion detected %d!\n", count + 1);
        timer_delay(1);
        count++;
    }
    }
}
 
//////////////// Temperature & Humidity  Sensor ////////////////
#define MAX_PIN_FLIPS 85       // Max number of times the pin switches between 0 or 1
#define TIMEOUT_COUNT 255      // Used to exit loop if timeout occurs while waiting for data 
#define TEMP_PIN  GPIO_PIN26

/*
 This function uses TEMP_PIN set to output to request data from the DHT11 temperature and 
 humidity sensor. The function then sets TEMP_PIN to input and monitors it for flips between
 0 and 1. Flips 0 - 3 are part of ACK sequence after requesting data from the sensor.
 Flip 4 is the first data bit. Even-numbered flips are data bits while odd-numbered flips 
 are the pauses (low) that precede each data bit. 
*/
void read_temp_data() {
    unsigned int counter = 0;
    unsigned int n_bits = 0; 
    int temp_data[5] = { 0 };
    float fahrenheit = 0.0; 

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
        printf( "Temperature = %d.%d C / %d.%d F  Humidity = %d.%d%% \n", temp_data[2], temp_data[3], f_int, f_frac, temp_data[0], temp_data[1] );
    } else {
        printf( "Data not good, skip\n" );
    }
}

void test_temp_sensor(void) {
    int iter = 10; 
    printf( "Testing DHT11 Temperature & Humidity sensor...\n" );
    for (int i = 0; i < iter; i++) {
        printf("Reading #%d/%d\n", i+1, iter); 
        read_temp_data();
        timer_delay(2);
    }
}


void main(void)
{
    uart_init();
    gpio_init();
    timer_init();

    // // Test motion sensor
    // setup_motion_sensor();
    // check_for_motion();

    // Test temperature sensor
    test_temp_sensor(); 

}

