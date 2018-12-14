#include "module.h"
#include "malloc.h"
#include "printf.h"
#include "sensors.h"
#include "output_formatter.h"

#define NUM_SUPPORTED_MODULES 5
#define COMPONENT_LEN 1024

#define NUM_WEATHER_COMPONENTS 4
#define NUM_HEADLINE_COMPONENTS 10

// Function prototypes
static bool is_valid_module(unsigned int moduleId);
static bool check_update_proximity();
static bool update_info_proximity();
static bool check_update_temperature();
static bool update_info_temperature();
static void temperature_module_init();
static bool check_update_datetime();
static bool update_info_datetime();
static void time_module_init();
static bool check_update_weather();
static bool update_info_weather();
static void weather_module_init();
static bool check_update_headline();
static bool update_info_headline();
static void headline_module_init();

static module_content_t* module_contents;

/*
 * Stores the mapping from modules to their corresponding functions.
 */
static const module_t modules[] = {
    {SD_MODULE_PROXIMITY, "Proximity", check_update_proximity, update_info_proximity},
    {SD_MODULE_TEMPERATURE, "Temperature", check_update_temperature, update_info_temperature},
    {SD_MODULE_DATETIME, "DateTime", check_update_datetime, update_info_datetime},
    {SD_MODULE_WEATHER, "Weather", check_update_weather, update_info_weather},
    {SD_MODULE_HEADLINE, "Headline", check_update_headline, update_info_headline}
};

void module_init() {
    module_contents = malloc(sizeof(module_content_t) * NUM_SUPPORTED_MODULES); 

    // initialize the module content array
    for (int moduleInd = 0; moduleInd < NUM_SUPPORTED_MODULES; moduleInd++) {
        char** components = malloc(sizeof(char*) * MAX_COMPONENTS);
        for (int componentInd = 0; componentInd < MAX_COMPONENTS; componentInd++) {
            components[componentInd] = malloc(sizeof(char) * COMPONENT_LEN);
        }
        coordinate_t* coordinates = malloc(sizeof(coordinate_t) * MAX_COMPONENTS);

        module_content_t content = {
            .components = components, 
            .coordinates = coordinates, 
            .numComponents = 0
        };

        module_contents[moduleInd] = content;
    }

    // initialize all necessary sensors
    temperature_module_init();
    time_module_init();
    weather_module_init();
    headline_module_init();
}

/*
 * Checks whether `moduleId` should be updated.
 */
bool check_module_update(unsigned int moduleId) {
    if (!is_valid_module(moduleId)) {
        return false;
    }

    return modules[moduleId].check_update();
}

/*
 * Updates the module information for module `moduleId` at the given
 * setting parameters.
 */
bool update_module_info(unsigned int moduleId, unsigned int settingId, unsigned int subSettingId) {
    if (!is_valid_module(moduleId)) {
        return false;
    }

    return modules[moduleId].update_info(settingId, subSettingId);
}

/*
 * Retrieves the module content for module `moduleId`.
 */
module_content_t* get_module_content(unsigned int moduleId) {
    if (!is_valid_module(moduleId)) {
        return NULL;
    }
    
    return (module_content_t*)(&module_contents[moduleId]);
}

/*
 * Module-specific functions for checking for updates, checking whether to
 * update information, and module initialization.
 */

/*
 * Proximity
 */
static bool check_update_proximity() {
    bool isMotionDetected = read_motion_data();
    return isMotionDetected;
}

static bool update_info_proximity(unsigned int settingId, unsigned int subSettingId) {
    return false;
}

/*
 * Temperature
 */
static bool check_update_temperature() {
    return true;
}

static bool update_info_temperature(unsigned int settingId, unsigned int subsettingId) {
    module_content_t* content = &module_contents[SD_MODULE_TEMPERATURE];
    
    bool result = read_temp_data(content->components, COMPONENT_LEN, settingId, subsettingId);
    
    return result;
}

static void temperature_module_init() {
    module_content_t* content = &module_contents[SD_MODULE_TEMPERATURE];
    
    snprintf(content->components[0], COMPONENT_LEN, "Temperature Module(Temperature)");
    content->coordinates[0].x = 0;
    content->coordinates[0].y = 0;

    snprintf(content->components[1], COMPONENT_LEN, "Temperature Module(Humidity)");
    content->coordinates[1].x = 0;
    content->coordinates[1].y = 40;

    content->numComponents = 2;
}

/*
 * Time
 */
static bool check_update_datetime() {
    return true;
}

static bool update_info_datetime(unsigned int settingId, unsigned int subSettingId) {
    module_content_t* content = &module_contents[SD_MODULE_DATETIME];
    
    int isRead = read_date_time(content->components, COMPONENT_LEN, settingId, subSettingId);
    
    return isRead > 0;
}

static void time_module_init() {
    module_content_t* content = &module_contents[SD_MODULE_DATETIME];

    snprintf(content->components[0], COMPONENT_LEN, "Time Module(DATE)");
    content->coordinates[0].x = 0;
    content->coordinates[0].y = 0;

    snprintf(content->components[1], COMPONENT_LEN, "Time Module(TIME)");
    content->coordinates[1].x = 0;
    content->coordinates[1].y = 20;
    content->numComponents = 2;
}

/*
 * Weather
 */
static bool check_update_weather() {
    return true;
}

static bool update_info_weather(unsigned int settingId, unsigned int subSettingId) {
    module_content_t* content = &module_contents[SD_MODULE_WEATHER];
    
    int isRead = read_weather(content->components, COMPONENT_LEN, settingId);
    return isRead > 0;
}

static void weather_module_init() {
    module_content_t* content = &module_contents[SD_MODULE_WEATHER];

    for (int i = 0; i < NUM_WEATHER_COMPONENTS; i++){
        snprintf(content->components[i], COMPONENT_LEN, "Weather %d", i);
        content->coordinates[i].x = 0;
        content->coordinates[i].y = 30 * i;
    }

    content->numComponents = NUM_WEATHER_COMPONENTS;
}

/*
 * Headline
 */
static bool check_update_headline() {
    return true;
}

static bool update_info_headline(unsigned int settingId, unsigned int subSettingId) {
    module_content_t* content = &module_contents[SD_MODULE_HEADLINE];
    
    int isRead = read_headlines(content->components, COMPONENT_LEN, settingId);
    return isRead > 0;
}

static void headline_module_init() {
    module_content_t* content = &module_contents[SD_MODULE_HEADLINE];
  
    for (int i = 0; i < NUM_HEADLINE_COMPONENTS; i++){
        snprintf(content->components[i], COMPONENT_LEN, "Headline %d", i);
        content->coordinates[i].x = 0;
        content->coordinates[i].y = 30 * i;
    }

    content->numComponents = NUM_HEADLINE_COMPONENTS;
}

/*
 * Helper functions
 */
static bool is_valid_module(unsigned int moduleId) {
    return (moduleId < NUM_SUPPORTED_MODULES);
}
