#include "printf.h"
#include "sensors.h"

test_temp_sensor(void) {
    int iter = 10;
    printf( "Testing DHT11 Temperature & Humidity sensor...\n" );
    for (int i = 0; i < iter; i++) {
        printf("Reading #%d/%d\n", i+1, iter);
        read_temp_data();
        timer_delay(2);
    }
}

void main(void) {
    test_temp_sensor();
}
