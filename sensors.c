#include "sensors.h"
#include "interrupts.h"
#include "gpio.h"

static void init_temp_sensor(){
    // TODO
}

static void init_prox_sensor(){
    // TODO
}

static void init_rtc_sensor(){
    // TODO
}

// Initialize everything necessary for sensors
void sensors_init(void){
    // TODO - setup gpio pins
    
    // TODO - setup interrupts


    init_temp_sensor();
    init_prox_sensor();
    init_rtc_sensor();
}
