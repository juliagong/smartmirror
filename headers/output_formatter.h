#ifndef FORMATTER_H
#define FORMATTER_H

/*
 * Output_formatter formats the data we retrieved into an output.
 * We use the current settings to determine our output format.
 */


/*
 * Enums to keep track of location of each piece of information
 * in the given data array
 */
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
    WEATHER_FAHRENHEIT_INT = 1,
    WEATHER_FAHRENHEIT_DEC = 2,
    WEATHER_CELSIUS_INT = 3,
    WEATHER_CELSIUS_DEC = 4,
    WEATHER_WIND_SPEED_MPH = 5,
    WEATHER_HUMIDITY_PERCENT = 6,
    WEATHER_CONDITION_CODE = 7,
};

enum temperature_data {
    TEMPERATURE_HUMIDITY_INT = 0,
    TEMPERATURE_HUMIDITY_DECIMAL = 1,
    TEMPERATURE_CELSIUS_INT = 2,
    TEMPERATURE_CELSIUS_DECIMAL = 3
};

/*
 * Format functions for different modules
 */
void format_temperature_data(char** buf, int bufsize, int* temp_data, unsigned int settingId, unsigned int subsettingId);
void format_time_data(char* buf, int bufsize, char** datetime, unsigned int settingId);
void format_date_data(char* buf, int bufsize, char** datetime, unsigned int settingId);
void format_weather_data(char** buf, int bufsize, char** datetime, unsigned int settingId);
void format_headlines_data(char** buf, int bufsize, unsigned int settingId);

#endif
