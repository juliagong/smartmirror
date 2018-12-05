#include "sensors.h"
#include "timer.h"
#include "printf.h"

void test_rotary_encoder() {

    while(1){
        printf("Waiting for 3 second\n");
        timer_delay(3);
        int rotation = read_rotary_data();
        printf("Rotated %d times \n", rotation);
    }
}

void main(void) {
    sensors_init();

    test_rotary_encoder();
}

