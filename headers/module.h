#ifndef MODULE_H
#define MODULE_H

#include <stdbool.h>
/*
 * TODO - description
 * Module defines and controls behaviors of a single module
 * This mostly includes sensors, but is generalized to support
 * other forms of modules as well
 *
 *
 */

static const int MAX_COMPONENTS = 5;

typedef struct{
    unsigned int x;
    unsigned int y;
} coordinate_t;

typedef struct{
    unsigned int width;
    unsigned int height;
} dimension_t;

typedef struct{
    unsigned int moduleId;
    char* name;
    bool (*check_update)();
    bool (*update_info)();
} module_t;

typedef struct{
    char** components;
    coordinate_t* coordinates;
    unsigned int numComponents;
} module_content_t;


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
bool update_module_info(unsigned int moduleId);

/*
 * Get content of module
 * For now, we are restricted to string components within a module
 */
module_content_t* get_module_content(unsigned int moduleId);

enum modules{
    SD_MODULE_PROXIMITY,
    SD_MODULE_TEMPERATURE,
    SD_MODULE_TIME
};


#endif
