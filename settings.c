#include "settings.h"
#include "setting_values.h"
#include "gl.h"
#include "custom_font_draw.h"
#include <stdbool.h>
#include "sensors.h"
#include "profile.h"
#include "printf.h"
#include "strings.h"
#include "timer.h"

#define CHAR_WIDTH 28
#define RECT_HEIGHT 50
#define MARGIN 20
#define TEXT_MARGIN 10


static module_config_t* settings;

static const setting_options_t setting_options[] = {
    {SETTING_LEVEL_MAIN, MAIN_SETTINGS_STRING, MAIN_SETTINGS_COUNT},
    {SETTING_LEVEL_DATE, DATE_SETTINGS_STRING, DATE_SETTINGS_COUNT},
    {SETTING_LEVEL_TIME, TIME_SETTINGS_STRING, TIME_SETTINGS_COUNT},
    {SETTING_LEVEL_TEMPERATURE, TEMPERATURE_SETTINGS_STRING, TEMPERATURE_SETTINGS_COUNT},
    {SETTING_LEVEL_WEATHER, WEATHER_SETTINGS_STRING, WEATHER_SETTINGS_COUNT},
    {SETTING_LEVEL_HEADLINE, HEADLINE_SETTINGS_STRING, HEADLINE_SETTINGS_COUNT}
};

/*
 * Display settings page
 */
void get_settings_page(module_config_t* profileSettings){
    settings = profileSettings;

    cursor_t* cursor = &(cursor_t){.settingLevel = SETTING_LEVEL_MAIN, .selectedOption = 0, .curPos = 0}; 
    bool drawScreen = true;
    display_settings(cursor);
    unsigned int offset;

    while(1){
        offset = read_rotary_data();
        drawScreen = move_cursor(cursor, offset);

        // We check for click only if shift is 0 to process action one at a time
        if (!drawScreen && is_rotary_clicked()){
            bool shouldExit = select_option(cursor);
            if (shouldExit){
                break;
            }
            drawScreen = true;
        }

        if (drawScreen) {
            display_settings(cursor);
            drawScreen = false;
        }
        timer_delay_ms(100);
    }
}

/*
 * Show current options of setting level
 * Determine which is the current option
 */
void display_settings(cursor_t* cursor){ 
    gl_clear(GL_BLACK);

    int settingLevel = cursor->settingLevel;
    unsigned int selectedOption = cursor->selectedOption;
    unsigned int curPos = cursor->curPos;
    
    setting_options_t settingOption = setting_options[settingLevel];
    
    const char** options = settingOption.options; 
    unsigned int numOptions = settingOption.numOptions;
    const char* title = (settingLevel == SETTING_LEVEL_MAIN) ? "Settings" : MAIN_SETTINGS_STRING[settingLevel];

    // draw title
    gl_draw_string_with_size(200, 50, (char*)title, GL_WHITE, 3);
    gl_draw_rect(180, 150, 500, 10, GL_WHITE);
    color_t c = GL_WHITE;

    // draw options
    for (int i = 0; i <= numOptions; i++){
        unsigned int y = 200 + i * (RECT_HEIGHT + MARGIN);
        char* str = (char*) (i < numOptions ? options[i] : RETURN_STRING);

        if (i == curPos){
            unsigned int rectWidth = (strlen(str) + 2) * CHAR_WIDTH;
            gl_draw_empty_rect(200, y, rectWidth, RECT_HEIGHT, GL_WHITE, 3 /* linewidth */); 
        }

        if (i < numOptions){
            // unless we are in main menu, show the current selected option in green
            c = (settingLevel != SETTING_LEVEL_MAIN && i == selectedOption) ? GL_GREEN : GL_WHITE;

            gl_draw_string_with_size(200 + TEXT_MARGIN, y + TEXT_MARGIN, str, c, 2);

        } else{ // return
            gl_draw_string_with_size(200 + TEXT_MARGIN, y + TEXT_MARGIN, (char*)RETURN_STRING, GL_RED, 2);
        }
    }

    gl_swap_buffer();
}


/*
 * Moves current cursor to the specified direction
 * Returns true if cursor has changed position, and false otherwise
 */
bool move_cursor(cursor_t* cursor, int direction){
    if (direction == 0){
        return false;
    }
    
    setting_options_t settingOption = setting_options[cursor->settingLevel];
    unsigned int numOptions = settingOption.numOptions;
    unsigned int curPos = cursor->curPos;


    if ((direction > 0 && curPos == numOptions) || (direction < 0 && curPos == 0)) {
        return false;
    }

    cursor->curPos = (direction > 0 ? curPos + 1 : curPos - 1);
    return true;
}

/*
 * Return the configuration for the module that corresponds to the setting page of cursor
 */
static module_config_t* get_module_config_at_cursor(cursor_t* cursor){
    module_config_t* moduleConfig = (module_config_t*)0;

    switch(cursor->settingLevel){
        case SETTING_LEVEL_DATE:
        case SETTING_LEVEL_TIME:
            moduleConfig = &settings[SD_MODULE_TIME];
            break;
        case SETTING_LEVEL_TEMPERATURE:
            moduleConfig = &settings[SD_MODULE_TEMPERATURE];
            break;
        case SETTING_LEVEL_WEATHER:
            moduleConfig = &settings[SD_MODULE_WEATHER];
            break;
        case SETTING_LEVEL_HEADLINE:
            moduleConfig = &settings[SD_MODULE_HEADLINE];
            break;
        default:
            break;
    }

    return moduleConfig;
}

/*
 * Get the current option for the module that corresponds to the 
 * setting page we are viewing
 */
static int get_current_option(cursor_t* cursor){
    module_config_t* moduleConfig = get_module_config_at_cursor(cursor);

    if (cursor->settingLevel == SETTING_LEVEL_TIME){
        return moduleConfig->moduleSubsettingId;
    }

    return moduleConfig->moduleSettingId;
}

/*
 * Saves current cursor's selected value as setting for the module
 * that corresponds to the setting page we are viewing
 */
static void save_current_option(cursor_t* cursor){
    module_config_t* moduleConfig = get_module_config_at_cursor(cursor);

    if (cursor->settingLevel == SETTING_LEVEL_TIME){
        moduleConfig->moduleSubsettingId = cursor->selectedOption;
    } 

    moduleConfig->moduleSettingId = cursor->selectedOption;
}

bool select_option(cursor_t* cursor){
    setting_options_t settingOption = setting_options[cursor->settingLevel];

    // returning to previous page
    if (cursor->curPos == settingOption.numOptions){
        if (cursor->settingLevel == SETTING_LEVEL_MAIN){
            return true; // exit out of setting page
        }

        // go back to main menu
        cursor->settingLevel = SETTING_LEVEL_MAIN;
        cursor->curPos = 0;

        return false;
    }

    // If we are on main level, we have to descend into lower level
    if (cursor->settingLevel == SETTING_LEVEL_MAIN) {
        cursor->settingLevel = cursor->curPos + 1; // +1 for the offset from index to setting level
        cursor->selectedOption = get_current_option(cursor);
        cursor->curPos = 0;
    } else{ // Otherwise, show the current option as selected
        cursor->selectedOption = cursor->curPos;
        save_current_option(cursor);  
    }

    return false;
}



/*
 * Definition for variables in setting_values.h
 */
const char RETURN_STRING[] = "Return";

const char *MAIN_SETTINGS_STRING[] = {
    "Date",
    "Time",
    "Temperature",
    "Weather",
    "Headline"
};

const char *TIME_SETTINGS_STRING[] = {
    "11:10pm",
    "23:10",
    "11:10:03pm",
    "23:10:03"
};

const char *DATE_SETTINGS_STRING[] = {
    "1/31/2019",
    "Jan 31, 2019"
};

const char *TEMPERATURE_SETTINGS_STRING[] = {
    "Fahrenheit",
    "Celsius",
    "Fahrenheit / Celsius",
    "Celsius / Fahrenheit"
};

const char *WEATHER_SETTINGS_STRING[] = {
    "TODO-WEATHER Option 1",
    "TODO-WEATHER Option 2"
};

const char *HEADLINE_SETTINGS_STRING[] = {
    "TODO-Headline Option 1",
    "TODO-Headline Option 2"
};

