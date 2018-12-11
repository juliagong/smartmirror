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
#include "font.h"

#define CHAR_WIDTH 28
#define RECT_HEIGHT 50
#define MARGIN 20
#define TEXT_MARGIN 10

#define NUM_THEME_COLORS 4

static module_config_t* settings;
static unsigned int* p_themeSettingId;
static unsigned int* p_fontSettingId;

static const setting_options_t setting_options[] = {
    {SETTING_LEVEL_MAIN, MAIN_SETTINGS_STRING, MAIN_SETTINGS_COUNT},
    {SETTING_LEVEL_DATE, DATE_SETTINGS_STRING, DATE_SETTINGS_COUNT},
    {SETTING_LEVEL_TIME, TIME_SETTINGS_STRING, TIME_SETTINGS_COUNT},
    {SETTING_LEVEL_TEMPERATURE, TEMPERATURE_SETTINGS_STRING, TEMPERATURE_SETTINGS_COUNT},
    {SETTING_LEVEL_WEATHER, WEATHER_SETTINGS_STRING, WEATHER_SETTINGS_COUNT},
    {SETTING_LEVEL_HEADLINE, HEADLINE_SETTINGS_STRING, HEADLINE_SETTINGS_COUNT},
    {SETTING_LEVEL_THEME, THEME_SETTINGS_STRING, THEME_SETTINGS_COUNT},
    {SETTING_LEVEL_FONT, FONT_SETTINGS_STRING, FONT_SETTINGS_COUNT}
};

/*
 * Display settings page
 */
void get_settings_page(profile_t* profile) {
    settings = profile->moduleConfig;
    p_themeSettingId = &(profile->themeSettingId);
    p_fontSettingId = &(profile->fontSettingId);

    cursor_t* cursor = &(cursor_t){.settingLevel = SETTING_LEVEL_MAIN, .selectedOption = 0, .curPos = 0}; 
    bool drawScreen = true;
    display_settings(cursor);
    unsigned int offset;

    while(1) {
        offset = read_rotary_data();
        drawScreen = move_cursor(cursor, offset);

        // We check for click only if shift is 0 to process action one at a time
        if (!drawScreen && is_rotary_clicked()) {
            bool shouldExit = select_option(cursor);
            if (shouldExit) {
                break;
            }
            drawScreen = true;
            reset_rotary_click();
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
void display_settings(cursor_t* cursor) {
    color_scheme_t* color_scheme = get_color_scheme(*p_themeSettingId);
    unsigned int text_color = color_scheme->text_color;

    gl_clear(color_scheme->bg_color);

    int settingLevel = cursor->settingLevel;
    unsigned int selectedOption = cursor->selectedOption;
    unsigned int curPos = cursor->curPos;
    
    setting_options_t settingOption = setting_options[settingLevel];
    
    const char** options = settingOption.options; 
    unsigned int numOptions = settingOption.numOptions;
    const char* title = (settingLevel == SETTING_LEVEL_MAIN) ? "Settings" : MAIN_SETTINGS_STRING[settingLevel - 1];

    // draw title
    unsigned int fontOffset = *p_fontSettingId == SETTING_FONT_1 ? -10 : 0;
    gl_draw_string_with_size(200 + fontOffset, 50 + fontOffset, (char*)title, text_color, 3);
    gl_draw_rect(180, 150, 550, 10, text_color);

    unsigned int charWidth = font_get_width() * 2;

    // draw options
    for (int i = 0; i <= numOptions; i++) {
        unsigned int y = 200 + i * (RECT_HEIGHT + MARGIN);
        char* str = (char*) (i < numOptions ? options[i] : RETURN_STRING);

        if (i == curPos) {
            unsigned int rectWidth = (strlen(str) + 2) * charWidth;
            gl_draw_empty_rect(200, y, rectWidth, RECT_HEIGHT, text_color, 3 /* linewidth */); 
        }

        if (i < numOptions) {
            // unless we are in main menu, show the current selected option in green
            color_t c = (settingLevel != SETTING_LEVEL_MAIN && i == selectedOption) ? color_scheme->select_color : text_color;

            gl_draw_string_with_size(200 + TEXT_MARGIN + fontOffset, y + TEXT_MARGIN + fontOffset, str, c, 2);

        } else { // return
            gl_draw_string_with_size(200 + TEXT_MARGIN + fontOffset, y + TEXT_MARGIN + fontOffset, (char*)RETURN_STRING, color_scheme->return_color, 2);
        }
    }

    gl_swap_buffer();
}


/*
 * Moves current cursor to the specified direction
 * Returns true if cursor has changed position, and false otherwise
 */
bool move_cursor(cursor_t* cursor, int direction) {
    if (direction == 0) {
        return false;
    }
    
    setting_options_t settingOption = setting_options[cursor->settingLevel];
    unsigned int numOptions = settingOption.numOptions;
    unsigned int curPos = cursor->curPos;

    if (direction > 0 && curPos == numOptions) {
        cursor->curPos = 0;
    } else if (direction < 0 && curPos == 0) {
        cursor->curPos = numOptions;
    } else {
        cursor->curPos = (direction > 0 ? curPos + 1 : curPos - 1);
    }

    return true;
}

/*
 * Return the configuration for the module that corresponds to the setting page of cursor
 */
static module_config_t* get_module_config_at_cursor(cursor_t* cursor) {
    module_config_t* moduleConfig = (module_config_t*)0;

    switch(cursor->settingLevel) {
        case SETTING_LEVEL_DATE:
        case SETTING_LEVEL_TIME:
            moduleConfig = &settings[SD_MODULE_DATETIME];
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
static int get_current_option(cursor_t* cursor) {
    if (cursor->settingLevel == SETTING_LEVEL_THEME) {
        return *p_themeSettingId;
    } else if (cursor->settingLevel == SETTING_LEVEL_FONT) {
        return *p_fontSettingId;
    }

    module_config_t* moduleConfig = get_module_config_at_cursor(cursor);

    if (cursor->settingLevel == SETTING_LEVEL_TIME) {
        return moduleConfig->moduleSubsettingId;
    }

    return moduleConfig->moduleSettingId;
}


static void set_font_option(unsigned int fontSettingId){
    if (fontSettingId == SETTING_FONT_1){
        font_set_font((font_t*)&font_segoeui);
    } else {
        font_set_font((font_t*)&font_default);
    }
}
/*
 * Saves current cursor's selected value as setting for the module
 * that corresponds to the setting page we are viewing
 */
static void save_current_option(cursor_t* cursor) {
    if (cursor->settingLevel == SETTING_LEVEL_THEME) {
        *p_themeSettingId = cursor->selectedOption;
    } else if (cursor->settingLevel == SETTING_LEVEL_FONT) {
        *p_fontSettingId = cursor->selectedOption;
        set_font_option(*p_fontSettingId);
    }
    
    module_config_t* moduleConfig = get_module_config_at_cursor(cursor);

    if (cursor->settingLevel == SETTING_LEVEL_TIME) {
        moduleConfig->moduleSubsettingId = cursor->selectedOption;
    } 

    moduleConfig->moduleSettingId = cursor->selectedOption;
}

bool select_option(cursor_t* cursor) {
    setting_options_t settingOption = setting_options[cursor->settingLevel];

    // returning to previous page
    if (cursor->curPos == settingOption.numOptions) {
        if (cursor->settingLevel == SETTING_LEVEL_MAIN) {
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
    } else { // Otherwise, show the current option as selected
        cursor->selectedOption = cursor->curPos;
        save_current_option(cursor);  
    }

    return false;
}

color_scheme_t* get_color_scheme(unsigned int themeSettingId){
    return (color_scheme_t*)COLOR_SCHEMES[themeSettingId];
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
    "Headline",
    "Customize Theme",
    "Change Font"
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

const char *THEME_SETTINGS_STRING[] = {
    "Classic Binary",
    "Midnight Office Hours",
    "Royal Hacker's Hex",
    "Sunset Soiree in C",
    "Plantiful Pleasantree"
};

/*
 * Color themes format: { background color, text color, selection color, return color }
 */
#define MIDNIGHT_BLUE 0xFF000B77
#define SAND_YELLOW 0xFFEFECC4
#define VIBRANT_BLUE 0xFF6FD2F9
#define BAMBI_BROWN 0xFFD88D6A

#define ROYAL_PURPLE 0xFF430272
#define GOLD 0xFFFFD65E
#define PALE_GREEN 0xFFD5EFB6
#define LIGHT_MAGENTA 0xFFF0BCFF

#define CRANBERRY 0xFF8C0237
#define MUTED_LEMON 0xFFF9F089
#define ORANGE_ZEST 0xFFFFD884
#define RED_SALMON 0xFFFF8B60

#define FOREST_GREEN 0xFF083004
#define PEACH 0xFFFFE1C1
#define LIME_GREEN 0xFFE3FCBD
#define HICKORY_TAN 0xFFC49D79

unsigned int COLOR_SCHEMES[5][4] = {
    { GL_BLACK, GL_WHITE, GL_GREEN, GL_RED },
    { MIDNIGHT_BLUE, SAND_YELLOW, VIBRANT_BLUE, BAMBI_BROWN },
    { ROYAL_PURPLE, GOLD, PALE_GREEN, LIGHT_MAGENTA },
    { CRANBERRY, MUTED_LEMON, ORANGE_ZEST, RED_SALMON },
    { FOREST_GREEN, LIME_GREEN, PEACH, HICKORY_TAN },
};

const char *FONT_SETTINGS_STRING[] = {
    "Default(Segoeui)",
    "System Font"
};
