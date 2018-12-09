#ifndef SENSORS_H
#define SENSORS_H

#include <stdbool.h>
/*
 * TODO - description
 *
 */

/*
 * Initialize all the sensors
 */
static const int temp_remove = 1;

void sensors_init(void);

bool read_temp_data(char* resultBuf, unsigned int bufLen);

bool read_motion_data();

int read_rotary_data();

bool is_rotary_clicked();

void reset_rotary_click();

int read_date_time(char** resultBufs, unsigned int bufLen, unsigned int settingId, unsigned int subSettingId);

#endif 
