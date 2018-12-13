#ifndef SENSORS_H
#define SENSORS_H

#include <stdbool.h>

/*
 * Defines all the functions that interact with sensors
 */

/*
 * Initialize all the sensors
 */
void sensors_init(void);

/*
 * Reads current temperature and humidity data, and save them into resultBuf
 * SettingId determines the format of temperature
 * subsettingId determines whether to display the humidity information or not
 *
 * Returns true if successfully read/updated
 */
bool read_temp_data(char** resultBuf, unsigned int bufLen, unsigned int settingId, unsigned int subsettingId);

/*
 * Uses proximity sensor to determine if there has been movement
 *
 * Returns true if there was, and false otherwise
 */
bool read_motion_data();

/*
 * Uses ESP-32 to retrieve the current date/time information from web
 * The result is saved into resultBuf
 * SettingId determines the format of date, while subsettingId determines format of time
 *
 * Returns integer greater than 0 if retrieval was successful
 */
int read_date_time(char** resultBufs, unsigned int bufLen, unsigned int settingId, unsigned int subsettingId);

/*
 * Uses ESP-32 to retrieve the current weather information from web
 * The result is saved into resultBuf
 *
 * Returns integer greater than 0 if retrieval was successful
 */
int read_weather(char** resultBufs, unsigned int bufLen, unsigned int settingId);

/*
 * Uses ESP-32 to retrieve the current headline information from web
 * The result is saved into resultBuf
 *
 * Returns integer greater than 0 if retrieval was successful
 */
int read_headlines(char** resultBufs, unsigned int bufLen, unsigned int settingId);

/*
 * Read the rotation of rotary; return the direction and amount of rotation
 */ 
int read_rotary_data();

/*
 * Check if rotary has been clicked and return if so
 */
 bool is_rotary_clicked();

/* 
 * Reset the rotary click to false
 */
void reset_rotary_click();

#endif 
