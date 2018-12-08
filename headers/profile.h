#ifndef PROFILE_H
#define PROFILE_H

#include "gl.h"
#include "module.h"

/*
 * TODO - DESCRIPTION
 * Profile keeps track of information about each user.
 * This includes information regarding
 * - Name
 * - UserId
 * - Linked Color Id
 * - Desired display configuration
 * - Desired module configurations
 */
static const int DEFAULT_PROFILE_ID = 0;

typedef struct{
    unsigned int moduleId;
    unsigned int moduleSettingId;
    unsigned int moduleSubsettingId;
    color_t moduleColor;
} module_config_t;

typedef struct{
    unsigned int pageId;
    unsigned int* moduleIds;
    coordinate_t* coordinates;
    unsigned int numModules;
} page_config_t;

typedef struct{
    unsigned int profileId;
    //color_t profileColor;
    module_config_t* moduleConfig;
    page_config_t* pageConfig;
    unsigned int numScreens;
    unsigned int homeScreenId;
} profile_t;

/*
 * Returns the profile information for a given Id
 * Includes information about
 *  - User profile color
 *  - Configuration/setting for each module
 *  - 
 */
profile_t* get_profile(unsigned int profileId);

module_config_t* get_module_config(unsigned int profileId, unsigned int moduleId);

void profile_init();

#endif
