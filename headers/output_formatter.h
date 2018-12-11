#ifndef FORMATTER_H
#define FORMATTER_H

enum datetime_data {
	DATETIME_HEADER = 0,
    DATETIME_DOW = 1,
    DATETIME_MONTH_NAME = 2,
    DATETIME_MONTH = 3,
    DATETIME_DAY = 4,
    DATETIME_YEAR = 5,
    DATETIME_HOUR = 6,
    DATETIME_MINUTE = 7,
    DATETIME_SECOND = 8
};

void format_temperature_data(char* buf, int bufsize, unsigned int settingId);

void format_time_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

void format_date_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

#endif
