#ifndef SETTINGS_H
#define SETTINGS_H
#include "profile.h"
#include "setting_values.h"
/*
 * Cursor that represents current navigation within settings page
 */
typedef struct{
    int settingLevel;
    unsigned int curPos;
} cursor_t;

/*
 * Open settings page and allows user to change settings
 */
void get_settings_page(profile_t* profile);

/*
 * Provided the cursor, display the current setting menu
 * Shows the currently applied setting in green, and depicts cursor position with a box
 */
void display_settings(cursor_t* cursor);

/*
 * Moves current cursor to the specified direction.
 * Returns ture if cursor has moved, false otherwise
 */
bool move_cursor(cursor_t* cursor, int direction);

/*
 * Once user 'clicks', chooses the action at cursor and takes corresponding action.
 * It will be either 1. choose the option 2. return to previous menu/screen
 * Returns true if we are exiting out of settings page, false otherwise.
 */
bool select_option(cursor_t* cursor);

/*
 * Returns the current color scheme
 */
color_scheme_t* get_color_scheme(unsigned int themeSettingId);

#endif
