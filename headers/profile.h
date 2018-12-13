#ifndef PROFILE_H
#define PROFILE_H

#include "gl.h"
#include "module.h"

/*
 * Profile keeps track of information about each user.
 * This includes information regarding
 * - UserId
 * - Configuration for each page
 * - Configuration for each module
 */

static const int DEFAULT_PROFILE_ID = 0;

/*
 * Configuration for each module
 * Determines which setting the module should use
 * 
 * SubsettingId is used in two cases:
 * - In datetime module to change configuration for time
 * - In temperature module to change display of humidity information
 */
typedef struct{
    unsigned int moduleId;
    unsigned int moduleSettingId;
    unsigned int moduleSubsettingId;
} module_config_t;

/*
 * Configuration for each page
 * Determines which module will be placed at which coordinate
 */
typedef struct{
    unsigned int pageId;
    unsigned int* moduleIds;
    coordinate_t* coordinates;
    unsigned int numModules;
} page_config_t;

/*
 * The whole profile with information regarding
 * - Configuration of each module
 * - Configuration of all pages
 * - Theme / Font settings
 */
typedef struct{
    unsigned int profileId;
    module_config_t* moduleConfig;
    page_config_t* pageConfig;
    unsigned int numScreens;
    unsigned int homeScreenId;
    unsigned int themeSettingId;
    unsigned int fontSettingId;
} profile_t;

/*
 * Returns the profile information for a given Id
 */
profile_t* get_profile(unsigned int profileId);

/*
 * Returns the configuration for the given module for given profileId
 */
module_config_t* get_module_config(unsigned int profileId, unsigned int moduleId);

void profile_init();

#endif
