#ifndef FORMATTER_H
#define FORMATTER_H

#include <stdarg.h>

void format_temperature_data(char* buf, int bufsize, unsigned int settingId);

void format_time_data(char* buf, int bufsize, unsigned int settingId);

void format_date_data(char* buf, int bufsize, unsigned int settingId);

#endif
