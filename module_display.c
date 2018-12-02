#include "module.h"
#include "profile.h"
#include "gl.h"
#include "module_display.h"
#include "sensors.h"

static profile_t* current_profile;
static unsigned int current_profile_id;
static unsigned int current_page;

void blank_screen(){
    gl_clear(GL_BLACK);
    gl_swap_buffer();
}

// TODO - change the hard coded values
static void draw_page_number(){
    char* buf[4];
    vsnprintf(buf, 4, "-%d-", current_page);
    gl_draw_string(300, 500, buf);
}

void draw_page(){
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

void draw_module(unsigned int moduleId, coordinate_t coordinate){
    // get latest module information if needed
    // TODO - we might not need this
    if(check_module_update(moduleId)){
        update_module_info(moduleId);
    } 
  
    // get module content
    module_content_t* content = get_module_content(moduleId);
    unsigned int numComponents = content->numComponents;
    char** components = content->components;
    coordinate_t* componentCoords = content->coordinates;

    // get module setting
    module_config_t* moduleConfig = get_module_config(current_profile_id, moduleId);
    color_t moduleColor = moduleConfig->moduleColor;

    for (unsigned int componentId = 0; componentId < numComponents; componentId ++){
        unsigned int compX = componentCoords[componentId].x + coordinate.x;
        unsigned int compY = componentCoords[componentId].y + coordinate.y;

        gl_draw_string(compX, compY, components[componentId], moduleColor);
    }
}

void get_next_page(){
    if (current_page < current_profile->numScreens - 1){
        current_page += 1; 
        draw_page(); 
    } 
}

void get_prev_page(){
    if (current_page > 0){
        current_page -= 1;
        draw_page();
    }
}

void switch_profile(unsigned int profileId){
    current_profile = get_profile(profileId);
    draw_page();
}

void md_init(unsigned int width, unsigned int height){
    gl_init(width, height, GL_DOUBLEBUFFER);
    module_init();
    profile_init();

    current_profile_id = 0;
    current_profile = get_profile(current_profile_id);
}
