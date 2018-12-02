#include "module_display.h"
#include "assert.h"
#include "module.h"
#include "printf.h"
#include "strings.h"
#include "timer.h"
#include "uart.h"
#include "gpio.h"
#include "sensors.h"

void test_blank_screen(){
    printf("Screen should be blank now.\n");
    blank_screen();
}

void test_draw_page(){
    printf("Testing module placement\n");
    draw_page();
    timer_delay(3);
}

void test_pages(){
    printf("Getting next page, should change\n");
    get_next_page();
    timer_delay(3);

    printf("Trying to go to next page again, should not changei\n");
    get_next_page();
    timer_delay(3);

    printf("Going back to first page, should changei\n");
    get_prev_page();
    timer_delay(3);

    printf("Trying to go to previous page again, should not change\n");
    get_prev_page();
    timer_delay(3);
}


void main(void){
    unsigned int width = 640;
    unsigned int height = 480;

    module_init();
    profile_init();
    sensors_init();
    md_init(width, height);

    uart_init();
    gpio_init();
    timer_init();

    test_draw_page();

    test_pages();

    test_blank_screen();

    printf("Test_module_display successfully completed.\n");
}
