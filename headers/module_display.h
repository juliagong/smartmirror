#ifndef MODULE_DISPLAY_H
#define MODULE_DISPLAY_H

#include "profile.h"
#include "gl.h"
/*
 * Defines a graphic library for a module-based display
 * Built on top of graphic library, but redefines
 * many functions to support more flexible behavior
 */

void md_init(unsigned int width, unsigned int height);

/*
 * Displays blank screen
 */
void blank_screen();

/*
 * Takes configuration of current page, and calls draw_module for each module on the page
 */
void draw_page();

/*
 * Display the given module at the coordinate, using the text_color
 */
void draw_module(unsigned int moduleId, coordinate_t coordinate, color_t text_color);

/*
 * Move to a different page. If we cannot move as requested 
 * (e.g. moving right in right-most page), we no-op
 */
void move_page(int change);

/*
 * Open up settings page.
 * All the previous information that was displayed in screen should disappear
 */
void open_settings();

/*
 * Switch between different profiles.
 * This feature is currently not supported.
 */
void switch_profile(unsigned int profileId);

#endif
