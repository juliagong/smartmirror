#include "setting_values.h"
#include "output_formatter.h"
#include <stdarg.h>
#include "printf.h"
#include "strings.h"



void format_temperature_data(char** buf, int bufsize, int* temp_data, unsigned int settingId, unsigned int subsettingId) {
    float fahrenheit = temp_data[TEMPERATURE_CELSIUS_INT] * 9. / 5. + 32;
    int f_int = fahrenheit;                 // Holds the integer portion of the float
    int f_frac = (fahrenheit - f_int) * 10; // Stores one decimal place

    switch (settingId) {
        case SETTING_TEMPERATURE_1:
            snprintf(buf[0], bufsize, "Temperature: %d.%dF", f_int, f_frac);
            break;
        case SETTING_TEMPERATURE_2:
            snprintf(buf[0], bufsize, "Temperature: %d.%dC", temp_data[TEMPERATURE_CELSIUS_INT], temp_data[TEMPERATURE_CELSIUS_DECIMAL]);
            break;
        case SETTING_TEMPERATURE_3:
            snprintf(buf[0], bufsize, "Temperature: %d.%dF / %d.%dC", f_int, f_frac, 
                    temp_data[TEMPERATURE_CELSIUS_INT], temp_data[TEMPERATURE_CELSIUS_DECIMAL]);
            break;
        case SETTING_TEMPERATURE_4:
            snprintf(buf[0], bufsize, "Temperature: %d.%dC / %d.%dF",
                    temp_data[TEMPERATURE_CELSIUS_INT], temp_data[TEMPERATURE_CELSIUS_DECIMAL], f_int, f_frac);
            break;
        default:
            break;
    }

    switch (subsettingId){
        case SETTING_HUMIDITY_SHOW:
            snprintf(buf[1], bufsize, "Humidity: %d.%d%%", temp_data[TEMPERATURE_HUMIDITY_INT], temp_data[TEMPERATURE_HUMIDITY_DECIMAL]);
            break;
        case SETTING_HUMIDITY_HIDE:
            buf[1][0] = '\0';
            break;
        default:
            break;
    }
};

void format_date_data(char* buf, int bufsize, char** datetime, unsigned int settingId) {
    switch (settingId) {
        case SETTING_DATE_1: // 1/31/2019
            snprintf(buf, bufsize, "%s/%s/%s", 
		datetime[DATETIME_MONTH], datetime[DATETIME_DAY], datetime[DATETIME_YEAR]); 
            break;
        case SETTING_DATE_2: // Jan 31, 2019
            snprintf(buf, bufsize, "%s %s, %s", 
		datetime[DATETIME_MONTH_NAME], datetime[DATETIME_DAY], datetime[DATETIME_YEAR]); 
            break;
        default:
            break;
    }
};

void format_time_data(char* buf, int bufsize, char** datetime, unsigned int settingId) {
    unsigned int hour;
    switch (settingId) {
        case SETTING_TIME_1: // 12 hr format
            hour = strtonum(datetime[DATETIME_HOUR], (const char**)0);
            char* ampmString = (hour >= 12) ? "PM" : "AM";
            hour %= 12; 

            snprintf(buf, bufsize, "%d:%s %s", hour, datetime[DATETIME_MINUTE], ampmString);
            break;
        case SETTING_TIME_2: // 24 hr format
            snprintf(buf, bufsize, "%s:%s", datetime[DATETIME_HOUR], datetime[DATETIME_MINUTE]);
            break;
        default:
            break;
    }
};
