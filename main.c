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

//////////////// Temp & Humidity Sensor ////////////////
#define MAXTIMINGS  85
#define TEMP_PIN    GPIO_PIN26
int temp_data[5] = { 0 };

/*
* In order to request that DHT11 sends data, have to set the pin to output and write low then high. 
*/
void request_temp_data(void) {
    int ack = 0;
    int attempt = 0; 
    

    while (!ack) {
        counter++; 
        printf("Requesting temperature data... (Attempt %d) \n", attempt);
        gpio_set_output(TEMP_PIN);
        gpio_write(TEMP_PIN, 0);
        timer_delay_ms(25);         // Data sheet says wait at least 18ms
        gpio_write(TEMP_PIN, 1); 
        timer_delay_us(20);         // Pull-up for 20us

        // Prepare to receive ack
        gpio_set_input(TEMP_PIN);    

        // Wait 40us for ack. If no ack, send another request 
        int counter = 0;
        while ( !ack && counter < 40 ) { // Wait for sensor to pull down the signal
            if (gpio_read(TEMP_PIN) == 0) ack = 1; 
            timer_delay_us( 1 );
            counter++;
        }
    }
}

/*
A complete data transmission is 40bit, and the sensor sends higher data bit first.
Data format: 8bit integral RH data + 8bit decimal RH data + 8bit integral T data + 8bit decimal T data + 8bit check sum. 
Check Sum: "8bit integral RH data + 8bit decimal RH data + 8bit integral T data + 8bit decimal T data"

*/
void read_temp_data() {
    
    int last_state = 1;        // Last state high
    unsigned int counter = 0;
    unsigned int j = 0; 
    float   f; 

    request_temp_data();


    printf("Receiving temperature data... \n");

/* 
 This code is off the website, but I haven't gone through it carefully enough to understand if it's doing what it's supposed to. 
 Will work on this more later.  
*/
    // for (int i = 0; i < MAXTIMINGS; i++ ) {
    //     counter = 0;
    //     while ( gpio_read( TEMP_PIN ) == last_state ) { // Check every usec for update and break if no change after 255 usecs
    //         counter++;
    //         timer_delay_us( 1 );
    //         if ( counter == 255 ) break;
    //     }
    //     last_state = gpio_read(TEMP_PIN);
 
    //     if ( counter == 255 ) break;
 
    //     if ( (i >= 4) && (i % 2 == 0) ) {
    //         temp_data[j / 8] <<= 1;
    //         if ( counter > 16 )
    //             temp_data[j / 8] |= 1;
    //         j++;
    //     }
    // }
 
    // if ( (j >= 40) &&
    //      (temp_data[4] == ( (temp_data[0] + temp_data[1] + temp_data[2] + temp_data[3]) & 0xFF) ) )
    // {
    //     f = temp_data[2] * 9. / 5. + 32;
    //     printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
    //         temp_data[0], temp_data[1], temp_data[2], temp_data[3], f );
    // }else  {
    //     printf( "Data not good, skip\n" );
    // }
}
 

void test_temp_sensor(void) {
    printf( "Testing DHT11 Temperature & Humidity sensor...\n" );
    for (int i = 0; i < 10; i++) {
        read_temp_data();
        timer_delay(1);
    }
    return(0);
}


void main(void)
{
    uart_init();
    gpio_init();
    timer_init();

    // Test motion sensor
    setup_motion_sensor();
    check_for_motion();

    // Test temperature sensor
    // setup_temp_sensor();
    // test_temp_sensor(); 

}

