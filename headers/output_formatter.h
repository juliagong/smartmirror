#ifndef FORMATTER_H
#define FORMATTER_H

enum datetime_data {
    DATETIME_DOW = 0,
    DATETIME_MONTH_NAME = 1,
    DATETIME_MONTH = 2,
    DATETIME_DAY = 3,
    DATETIME_YEAR = 4,
    DATETIME_HOUR = 5,
    DATETIME_MINUTE = 6,
    DATETIME_SECOND = 7
};

enum weather_data {
    WEATHER_DESCRIPTION =0,
    WEATHER_TEMPERATURE_FAHRENHEIT = 1,
    WEATHER_TEMPERATURE_CELSIUS = 2,
    WEATHER_WIND_SPEED_MPH = 3,
    WEATHER_HUMIDITY_PERCENT = 4,
    WEATHER_CONDITION_CODE = 5,
};

void format_temperature_data(char* buf, int bufsize, unsigned int settingId);

void format_time_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

void format_date_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

void format_weather_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

void format_headlines_data(char* buf, int bufsize, char** datetime, unsigned int settingId);

#endif
