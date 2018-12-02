#include "profile.h"
#include "assert.h"
#include "printf.h"

void test_get_profile(){
    profile_t* defaultProfile = get_profile(DEFAULT_PROFILE_ID);
    printf("Default profile id: %d\n", defaultProfile->profileId);
    assert(defaultProfile->profileId == DEFAULT_PROFILE_ID);
    assert(defaultProfile->numScreens == 2);
    assert(defaultProfile->homeScreenId == 0);

    // check page config
    page_config_t* defaultPageConfig = defaultProfile->pageConfig;
    assert(defaultPageConfig[0].pageId == 0);
    assert(defaultPageConfig[0].numModules = 2);
    assert(defaultPageConfig[0].coordinates[0].x  == 0);
    assert(defaultPageConfig[0].coordinates[0].y  == 0);
    assert(defaultPageConfig[0].moduleIds[0]  == SD_MODULE_TEMPERATURE);
    assert(defaultPageConfig[0].moduleIds[1]  == SD_MODULE_TIME);
    assert(defaultPageConfig[1].pageId == 1);
    assert(defaultPageConfig[1].numModules = 2);
    assert(defaultPageConfig[1].coordinates[0].x  == 300);
    assert(defaultPageConfig[1].coordinates[0].y  == 0);
    assert(defaultPageConfig[1].moduleIds[0]  == SD_MODULE_TEMPERATURE);
    assert(defaultPageConfig[1].moduleIds[1]  == SD_MODULE_TIME);
    
    // check module config
    module_config_t* defaultModuleConfig = defaultProfile->moduleConfig;
    assert(defaultModuleConfig[0].moduleId == SD_MODULE_TEMPERATURE);
    assert(defaultModuleConfig[0].moduleColor == GL_BLACK);
    assert(defaultModuleConfig[1].moduleId == SD_MODULE_TIME);
    assert(defaultModuleConfig[1].moduleColor == GL_BLACK);
    
    printf("Test get profile successful\n");
}

void main(void){
    profile_init();
    test_get_profile();
    printf("Test profile ended successfully.\n");
}
