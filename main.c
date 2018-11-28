#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

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

void main(void)
{
    uart_init();
    gpio_init();
    timer_init();

    setup_motion_sensor();
    check_for_motion();
}

