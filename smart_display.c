#include "module.h"
#include "module_display.h"
#include "sensors.h"
#include "profile.h"
#include "timer.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static void display(){
    while(1){
        // show screen only if there is movement 
        if (check_module_update(SD_MODULE_PROXIMITY)){
            draw_page();
        } else{
            blank_screen();
        }

        timer_delay(2);
    }
}

void main(){
    module_init();
    profile_init();
    sensors_init();
    md_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    display();
}
