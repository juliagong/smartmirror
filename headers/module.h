#ifndef MODULE_H
#define MODULE_H

#include <stdbool.h>
/*
 * Module defines and controls behaviors of a single module
 * This mostly includes sensors, but is generalized to support
 * other forms of modules as well
 *
 */


/*
 * Defines a location for a component
 */
typedef struct{
    unsigned int x;
    unsigned int y;
} coordinate_t;


/*
 * Define a module, includes its name and id along with
 * - A function to check if there is new information
 * - A function to update the information
 */
typedef struct{
    unsigned int moduleId;
    char* name;
    bool (*check_update)();
    bool (*update_info)(unsigned int settingId, unsigned int subSettingId);
} module_t;

static const int MAX_COMPONENTS = 10;
/*
 * Defines the content of a module
 * - There are at most #MAX_COMPONENTS components
 * - Each component is a string
 * - coordinates define the location of each component within the module
 */
typedef struct{
    char** components;
    coordinate_t* coordinates;
    unsigned int numComponents;
} module_content_t;


/*
 * Perform all the initialization needed for modules
 */
void module_init(void);

/*
 * Checks whether given module has new information or not
 * Returns true if it does and false otherwise
 * If given module id is invalid return false
 */
bool check_module_update(unsigned int moduleId);

/*
 * Updates the given module's information
 * Returns true if successfully updated and false otherwise
 * If given module id is invalid return false
 */
bool update_module_info(unsigned int moduleId, unsigned int settingId, unsigned int subSettingId);

/*
 * Get content of module
 * For now, we are restricted to string components within a module
 */
module_content_t* get_module_content(unsigned int moduleId);

enum modules {
    SD_MODULE_PROXIMITY,
    SD_MODULE_TEMPERATURE,
    SD_MODULE_DATETIME,
    SD_MODULE_WEATHER,
    SD_MODULE_HEADLINE,
    SD_MODULE_THEME
};


#endif
