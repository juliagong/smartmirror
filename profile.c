#include "profile.h"
#include "module.h"
#include "gl.h"
#include "malloc.h"
#include "printf.h"

#define NUM_PROFILES 1
#define NUM_PAGES 3

#define NUM_ALL_MODULES 5
#define NUM_HOME_MODULES 2
#define NUM_CONTENT_MODULES 2

// contains the respective modules ids for the modules that appear on each page
unsigned int home_module_ids[] = { SD_MODULE_DATETIME, SD_MODULE_TEMPERATURE };
unsigned int weather_module_ids[] = { SD_MODULE_DATETIME, SD_MODULE_WEATHER };
unsigned int headline_module_ids[] = { SD_MODULE_DATETIME, SD_MODULE_HEADLINE };

// coordinates for each module on each page
coordinate_t home_coordinates[] = {
       {.x = 0, .y = 700},
       {.x = 0, .y = 90}
};

coordinate_t content_coordinates[] = {
       {.x = 0, .y = 700},
       {.x = 0, .y = 90}
};

profile_t* profiles;

/*
 * Returns the address of the profile requested, if valid.
 */
profile_t* get_profile(unsigned int profileId) {
    if (profileId >= NUM_PROFILES) {
        return (profile_t*)0;
    }
    return &profiles[profileId];
}

/*
 * Returns the corresponding module_config_t for the requested module.
 */
module_config_t* get_module_config(unsigned int profileId, unsigned int moduleId) {
    unsigned int numModules = 4;
    module_config_t* cur = profiles[profileId].moduleConfig;

    for (int i = 0; i < numModules; i++) {
        if (cur[i].moduleId == moduleId) {
            return &cur[i];
        }    
    }

    // TODO - maybe return default?
    return (module_config_t*)0;
}

/*
 * Creates default profile that will be displayed on the screen.
 */
static void create_default_profile() {
    module_config_t* defaultModuleConfig = malloc(sizeof(module_config_t) * NUM_ALL_MODULES);
    defaultModuleConfig[0] = (module_config_t){.moduleId = SD_MODULE_PROXIMITY, .moduleSettingId = 0, 
        .moduleSubsettingId = 0};
    defaultModuleConfig[1] = (module_config_t){.moduleId = SD_MODULE_TEMPERATURE, .moduleSettingId = 0, 
        .moduleSubsettingId = 0};
    defaultModuleConfig[2] = (module_config_t){.moduleId = SD_MODULE_DATETIME, .moduleSettingId = 0, 
        .moduleSubsettingId = 0};
    defaultModuleConfig[3] = (module_config_t){.moduleId = SD_MODULE_WEATHER, .moduleSettingId = 0, 
        .moduleSubsettingId = 0};
    defaultModuleConfig[4] = (module_config_t){.moduleId = SD_MODULE_HEADLINE, .moduleSettingId = 0, 
        .moduleSubsettingId = 0};

    page_config_t* defaultPageConfig = malloc(sizeof(page_config_t) * NUM_PAGES);
    defaultPageConfig[0] = (page_config_t){.pageId = 0, .moduleIds = home_module_ids,
	.coordinates = home_coordinates, .numModules = NUM_HOME_MODULES};
    defaultPageConfig[1] = (page_config_t){.pageId = 1, .moduleIds = weather_module_ids, 
        .coordinates = content_coordinates, .numModules = NUM_CONTENT_MODULES};
    defaultPageConfig[2] = (page_config_t){.pageId = 2, .moduleIds = headline_module_ids, 
        .coordinates = content_coordinates, .numModules = NUM_CONTENT_MODULES};
   
    profile_t* defaultProfile = malloc(sizeof(profile_t));
    defaultProfile[0] = (profile_t){.profileId = DEFAULT_PROFILE_ID, .moduleConfig = defaultModuleConfig, 
        .pageConfig = defaultPageConfig, .numScreens = NUM_PAGES, .homeScreenId = 0, .themeSettingId = 0, .fontSettingId = 0};

    profiles[DEFAULT_PROFILE_ID] = *defaultProfile; 
}

void profile_init() {
    profiles = malloc(sizeof(profile_t) * NUM_PROFILES);
    create_default_profile();
}

