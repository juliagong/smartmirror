#include "module.h"
#include "module_display.h"
#include "sensors.h"
#include "profile.h"
#include "timer.h"
#include "settings.h"

#define SCREEN_WIDTH 1680
#define SCREEN_HEIGHT 1050

static void display() {
    while(1) {
        // show screen only if there is movement 
        if (check_module_update(SD_MODULE_PROXIMITY)) {
            if (is_rotary_clicked()){
                open_settings(); // blocking action
            }
            
            move_page(read_rotary_data());
            draw_page();
        } else {
            blank_screen();
        }

        // timer_delay(1);
    }
}

void main() {
    module_init();
    profile_init();
    sensors_init();
    md_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    display();
}
