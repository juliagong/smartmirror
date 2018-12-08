#include "settings.h"
#include "printf.h"
#include "profile.h"
#include "sensors.h"
#include "module_display.h"
#include "uart.h"

void test_setting_page(){
    sensors_init();
    md_init(1024, 768);

    profile_t* current_profile = get_profile(0);

    get_settings_page(current_profile->moduleConfig);
}


void main(void){
    uart_init();    
    test_setting_page();

}
