#include "module.h"
#include "malloc.h"
#include "printf.h"
#include "sensors.h"
#include "output_formatter.h"

#define NUM_SUPPORTED_MODULES 3
#define COMPONENT_LEN 1024

static bool is_valid_module(unsigned int moduleId);
static bool check_update_proximity();
static bool update_info_proximity();
static void proximity_module_init();
static bool check_update_temperature();
static bool update_info_temperature();
static void temperature_module_init();
static bool check_update_datetime();
static bool update_info_datetime();
static void time_module_init();

static module_content_t* module_contents;

/*
 * Mapping from modules to corresponding functions
 */
static const module_t modules[] = {
    {SD_MODULE_PROXIMITY, "Proximity", check_update_proximity, update_info_proximity},
    {SD_MODULE_TEMPERATURE, "Temperature", check_update_temperature, update_info_temperature},
    {SD_MODULE_DATETIME, "DateTime", check_update_datetime, update_info_datetime}
};

void module_init() {
    module_contents = malloc(sizeof(module_content_t*) * NUM_SUPPORTED_MODULES); 
    // initialize the module content array
    for (int moduleInd = 0; moduleInd < NUM_SUPPORTED_MODULES; moduleInd++) {
        char** components = malloc(sizeof(char*) * MAX_COMPONENTS);
        for (int componentInd = 0; componentInd < MAX_COMPONENTS; componentInd++) {
            components[componentInd] = malloc(sizeof(char) * COMPONENT_LEN);
        }
        coordinate_t* coordinates = malloc(sizeof(coordinate_t) * MAX_COMPONENTS);

        module_content_t content = { .components = components, .coordinates = coordinates, .numComponents = 0};
        module_contents[moduleInd] = content;
    }

    proximity_module_init();
    temperature_module_init();
    time_module_init();
}

unsigned int get_num_modules(){
    return NUM_SUPPORTED_MODULES;
}

bool check_module_update(unsigned int moduleId){
    if (!is_valid_module(moduleId)){
        return false;
    }

    return modules[moduleId].check_update();
}

bool update_module_info(unsigned int moduleId, unsigned int settingId, unsigned int subSettingId){
    if (!is_valid_module(moduleId)){
        return false;
    }

    return modules[moduleId].update_info(settingId, subSettingId);
}

module_content_t* get_module_content(unsigned int moduleId){
    if (!is_valid_module(moduleId)){
        return NULL;
    }
    
    return (module_content_t*)(&module_contents[moduleId]);
}

/*
 * Module specific functions
 */

/*
 * Proximity
 */
static bool check_update_proximity(){

    // TODO - use this for acutal code
    bool isMotionDetected = read_motion_data();
    return isMotionDetected;
}

static bool update_info_proximity(unsigned int settingId, unsigned int subSettingId){
    // TODO
    return false;
}

static void proximity_module_init(){
    module_content_t* content = &module_contents[SD_MODULE_PROXIMITY];
    
    snprintf(content->components[0], COMPONENT_LEN, "Proximity Module");
    content->coordinates[0].x = 0;
    content->coordinates[0].y = 0;
    content->numComponents = 1;
}

/*
 * Temperature
 */
static bool check_update_temperature(){
    // TODO - DK - this should be true if we want to be using temperature sensor
    return true;
}

static bool update_info_temperature(unsigned int settingId, unsigned int subSettingId){
    module_content_t* content = &module_contents[SD_MODULE_TEMPERATURE];
    
    bool result = read_temp_data(content->components[0], COMPONENT_LEN);
    
    return result;
}

static void temperature_module_init(){
    module_content_t* content = &module_contents[SD_MODULE_TEMPERATURE];
    
    snprintf(content->components[0], COMPONENT_LEN, "Temperature Module");
    content->coordinates[0].x = 0;
    content->coordinates[0].y = 0;
    content->numComponents = 1;
}

/*
 * Time
 */

static bool check_update_datetime(){
    return true;
}

static bool update_info_datetime(unsigned int settingId, unsigned int subSettingId){
    module_content_t* content = &module_contents[SD_MODULE_DATETIME];
    
    int isRead = read_date_time(content->components, COMPONENT_LEN, settingId, subSettingId);
    
    return isRead > 0;
}

static void time_module_init(){
    module_content_t* content = &module_contents[SD_MODULE_DATETIME];
    
    snprintf(content->components[0], COMPONENT_LEN, "Time Module(DATE)");
    content->coordinates[0].x = 0;
    content->coordinates[0].y = 0;
    
    snprintf(content->components[1], COMPONENT_LEN, "Time Module(TIME)");
    content->coordinates[1].x = 0;
    content->coordinates[1].y = 40;
    content->numComponents = 2;
}


/*
 * Helper functions
 */
static bool is_valid_module(unsigned int moduleId){
    return (moduleId < NUM_SUPPORTED_MODULES);
}
