#include "profile.h"
#include "module.h"
#include "gl.h"
#include "malloc.h"
#include "printf.h"

#define NUM_PROFILES 1

unsigned int default_module_ids[] = { SD_MODULE_TEMPERATURE, SD_MODULE_DATETIME };
coordinate_t default_coordinates[] = {
       {.x = 0, .y = 0},
       {.x = 0, .y = 100}
};

coordinate_t default_coordinates_reversed[] = {
       {.x = 0, .y = 100},
       {.x = 0, .y = 0}
};

profile_t* profiles;

profile_t* get_profile(unsigned int profileId){
    if (profileId >= NUM_PROFILES) {
        return (profile_t*)0;
    }
    return &profiles[profileId];
}

module_config_t* get_module_config(unsigned int profileId, unsigned int moduleId) {
    unsigned int numModules = 4;
    module_config_t* cur = profiles[profileId].moduleConfig;

    for (int i = 0; i < numModules; i++){
        if (cur[i].moduleId == moduleId){
            return &cur[i];
        }    
    }

    // TODO - maybe return default?
    return (module_config_t*)0;
}

static void create_default_profile() {
    unsigned int numAllModules = 5;
    unsigned int numModules = 2;
    
    module_config_t* defaultModuleConfig = malloc(sizeof(module_config_t) * numAllModules);
    defaultModuleConfig[0] = (module_config_t){.moduleId = SD_MODULE_PROXIMITY, .moduleSettingId = 0, 
        .moduleSubsettingId = 0, .moduleColor = GL_WHITE};
    defaultModuleConfig[1] = (module_config_t){.moduleId = SD_MODULE_TEMPERATURE, .moduleSettingId = 0, 
        .moduleSubsettingId = 0, .moduleColor = GL_WHITE};
    defaultModuleConfig[2] = (module_config_t){.moduleId = SD_MODULE_DATETIME, .moduleSettingId = 0, 
        .moduleSubsettingId = 0, .moduleColor = GL_WHITE};
    defaultModuleConfig[3] = (module_config_t){.moduleId = SD_MODULE_WEATHER, .moduleSettingId = 0, 
        .moduleSubsettingId = 0, .moduleColor = GL_WHITE};
    defaultModuleConfig[4] = (module_config_t){.moduleId = SD_MODULE_HEADLINE, .moduleSettingId = 0, 
        .moduleSubsettingId = 0, .moduleColor = GL_WHITE};

    page_config_t* defaultPageConfig = malloc(sizeof(page_config_t) * 2);
    defaultPageConfig[0] = (page_config_t){.pageId = 0, .moduleIds = default_module_ids, 
        .coordinates = default_coordinates, .numModules = numModules};
    defaultPageConfig[1] = (page_config_t){.pageId = 1, .moduleIds = default_module_ids, 
        .coordinates = default_coordinates_reversed, .numModules = numModules};
   
    profile_t* defaultProfile = malloc(sizeof(profile_t));
    defaultProfile[0] = (profile_t){.profileId = DEFAULT_PROFILE_ID, .moduleConfig = defaultModuleConfig, 
        .pageConfig = defaultPageConfig, .numScreens = 2, .homeScreenId = 0};

    profiles[DEFAULT_PROFILE_ID] = *defaultProfile; 
}

void profile_init() {
    profiles = malloc(sizeof(profile_t) * NUM_PROFILES);
    create_default_profile();
}

