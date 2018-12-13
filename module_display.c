#include "module.h"
#include "profile.h"
#include "gl.h"
#include "module_display.h"
#include "sensors.h"
#include "printf.h"
#include "custom_font_draw.h"
#include "settings.h"
#include "setting_values.h"

static profile_t* current_profile;      // keeps track of profile of current page
static unsigned int current_profile_id; 
static unsigned int current_page;

static unsigned int screen_width;
static unsigned int screen_height;

static unsigned int module_frequency[] = { 1, 1, 1, 10, 10}; // determines how often the module is updated
static unsigned int module_count[] = {1, 1, 1, 10, 10};

/*
 * Draws a blank black screen for the "off" mode of the mirror.
 */
void blank_screen() {
    color_scheme_t* color_scheme = get_color_scheme(current_profile->themeSettingId);
    gl_clear(color_scheme->bg_color);
    gl_swap_buffer();
}

/*
 * Draws the page number on the bottom of the screen
 */
static void draw_page_number(color_t c) {
    char buf[4];
    snprintf(buf, 4, "-%d-", current_page + 1);

    gl_draw_string(screen_width / 2 - 50, screen_height - 30, buf, c);
}

/*
 * Displays the current page on the monitor using draw_module.
 */
void draw_page() {
    color_scheme_t* color_scheme = get_color_scheme(current_profile->themeSettingId);
    gl_clear(color_scheme->bg_color);

    color_t text_color = color_scheme->text_color;

    page_config_t currentPageConfig = current_profile->pageConfig[current_page];
    
    // draw title for each page
    if (current_page == 0) { // draw greeting on home page
	    gl_draw_string_with_size(0, 0, "Howdy from CS 107e SmartMirror!", text_color, 2);
    } else if (current_page == 1) { // title for weather data
	    gl_draw_string_with_size(0, 0, "Weather", text_color, 2);
    } else if (current_page == 2) { // title for headline data
	    gl_draw_string_with_size(0, 0, "Today's Headlines", text_color, 2);
    }

    // draw all modules that should appear on this page
    unsigned int numModules = currentPageConfig.numModules;
    unsigned int* moduleIds = currentPageConfig.moduleIds;
    coordinate_t* coordinates = currentPageConfig.coordinates; 
    
    for (unsigned int moduleInd = 0; moduleInd < numModules; moduleInd++) {
        draw_module(moduleIds[moduleInd], coordinates[moduleInd], text_color);
    } 

    draw_page_number(text_color);
    
    gl_swap_buffer();
}

/*
 * Gets module content and setting and draws module on screen.
 */
void draw_module(unsigned int moduleId, coordinate_t coordinate, color_t c) {
    // get module setting
    module_config_t* moduleConfig = get_module_config(current_profile_id, moduleId);
    
    // get latest module information if needed
    if(check_module_update(moduleId)) {
        module_count[moduleId] += 1;

        // update modules in different frequenices
        // time, temperature: every time
        // weather, headlines: every 10 minutes
        if (module_count[moduleId] >= module_frequency[moduleId]) {
            bool isUpdated = update_module_info(moduleId, moduleConfig->moduleSettingId, moduleConfig->moduleSubsettingId);
            if (isUpdated) {
                module_count[moduleId] = 0;
            }
        }
    } 
  
    // get module content
    module_content_t* content = get_module_content(moduleId);
    unsigned int numComponents = content->numComponents;
    char** components = content->components;
    coordinate_t* componentCoords = content->coordinates;

    for (unsigned int componentId = 0; componentId < numComponents; componentId ++) {
        unsigned int compX = componentCoords[componentId].x + coordinate.x;
        unsigned int compY = componentCoords[componentId].y + coordinate.y;

        gl_draw_string_with_size(compX, compY, components[componentId], c, 1);
    }
}

/*
 * Gets and displays the next page.
 */
void move_page(int change) {
    if (change == 0){
        return;
    }

    int newVal = current_page + change;
    if (change > 0) {
        int maxVal = current_profile->numScreens - 1; 
        current_page = newVal >= maxVal ? maxVal : newVal;
    } else {
        current_page = newVal < 0 ? 0 : newVal;
    }
}

void open_settings() {
    get_settings_page(current_profile);
}

void switch_profile(unsigned int profileId) {
    current_profile = get_profile(profileId);
    draw_page();
}

void md_init(unsigned int width, unsigned int height) {
    screen_width = width;
    screen_height = height;
    
    gl_init(width, height, GL_DOUBLEBUFFER);
    module_init();
    profile_init();

    current_profile_id = 0;
    current_profile = get_profile(current_profile_id);
}
