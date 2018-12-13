#include "module.h"
#include "module_display.h"
#include "sensors.h"
#include "profile.h"
#include "timer.h"
#include "settings.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

static void display() {
    while(1) {
        // show screen only if there has been movement 
        if (check_module_update(SD_MODULE_PROXIMITY)) {
            if (is_rotary_clicked()) {
                open_settings(); // blocking action
                reset_rotary_click();
            }
            
            move_page(read_rotary_data());
            draw_page();
        } else {
            blank_screen();
        }
    }
}

void main() {
    module_init();
    profile_init();
    sensors_init();
    md_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    display();
}
