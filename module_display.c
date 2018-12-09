#include "module.h"
#include "profile.h"
#include "gl.h"
#include "module_display.h"
#include "sensors.h"
#include "printf.h"
#include "custom_font_draw.h"
#include "settings.h"

static profile_t* current_profile; // keeps track of profile of current page
static unsigned int current_profile_id;
static unsigned int current_page;

static unsigned int screen_width;
static unsigned int screen_height;
/**
 * Function: blank_screen
 *
 * Draws a blank black screen for the "off" mode of the mirror.
**/
void blank_screen() {
    gl_clear(GL_BLACK);
    gl_swap_buffer();
}

static void draw_page_number(){
    char buf[4];
    snprintf(buf, 4, "-%d-", current_page);

    // TODO - add base color in profile, use here 
    gl_draw_string(screen_width / 2 - 50, screen_height - 30, buf, GL_WHITE);
}

/**
 * Function: draw_page
 *
 * Displays the current page on the monitor using draw_module.
**/
void draw_page() {
    gl_clear(GL_BLACK);

    page_config_t currentPageConfig = current_profile->pageConfig[current_page];
    
    unsigned int numModules = currentPageConfig.numModules;
    unsigned int* moduleIds = currentPageConfig.moduleIds;
    coordinate_t* coordinates = currentPageConfig.coordinates; 

    for (unsigned int moduleInd = 0; moduleInd < numModules; moduleInd++){
        draw_module(moduleIds[moduleInd], coordinates[moduleInd]);
    } 


    draw_page_number();
    
    gl_swap_buffer();
}

/**
 * Function: draw_module
 *
 * Gets module content and setting and draws module on screen.
**/
void draw_module(unsigned int moduleId, coordinate_t coordinate) {
    // get module setting
    module_config_t* moduleConfig = get_module_config(current_profile_id, moduleId);
    color_t moduleColor = moduleConfig->moduleColor;
    
    // get latest module information if needed
    // TODO - we might not need this
    if(check_module_update(moduleId)){
        update_module_info(moduleId, moduleConfig->moduleSettingId, moduleConfig->moduleSubsettingId);
    } 
  
    // get module content
    module_content_t* content = get_module_content(moduleId);
    unsigned int numComponents = content->numComponents;
    char** components = content->components;
    coordinate_t* componentCoords = content->coordinates;

    for (unsigned int componentId = 0; componentId < numComponents; componentId ++){
        unsigned int compX = componentCoords[componentId].x + coordinate.x;
        unsigned int compY = componentCoords[componentId].y + coordinate.y;

        gl_draw_string_with_size(compX, compY, components[componentId], moduleColor, 2);
    }
}

/**
 * Function: get_next_page
 *
 * Gets and displays the next page.
**/
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

void open_settings(){
    get_settings_page(current_profile->moduleConfig);    
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
