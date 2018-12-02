#ifndef MODULE_DISPLAY_H
#define MODULE_DISPLAY_H

#include "profile.h"
/*
 * Defines a graphic library for a module-based display
 * Built on top of graphic library, but redefines
 * many functions to support more flexible behavior
 */

void md_init(unsigned int width, unsigned int height);

void blank_screen();

void draw_page();

void draw_module(unsigned int moduleId, coordinate_t coordinate);

void get_next_page();

void get_prev_page();

void switch_profile(unsigned int profileId);

#endif
