#include "setting_values.h"
#include "output_formatter.h"
#include <stdarg.h>
#include "printf.h"
#include "strings.h"

void format_temperature_data(char* buf, int bufsize, unsigned int settingId){
    // TODO
    switch (settingId){
        case SETTING_TEMPERATURE_1:
            break;
        case SETTING_TEMPERATURE_2:
            break;
        case SETTING_TEMPERATURE_3:
            break;
        default:
            break;
    }
};

void format_date_data(char* buf, int bufsize, char** datetime, unsigned int settingId){
    switch (settingId){
        case SETTING_DATE_1: // 1/31/2019
            snprintf(buf, bufsize, "%s/%s/%s", datetime[DATETIME_MONTH], datetime[DATETIME_DAY], datetime[DATETIME_YEAR]); 
            break;
        case SETTING_DATE_2: // Jan 31, 2019
            snprintf(buf, bufsize, "%s %s, %s", datetime[DATETIME_MONTH_NAME], datetime[DATETIME_DAY], datetime[DATETIME_YEAR]); 
            break;
        default:
            break;
    }
};

void format_time_data(char* buf, int bufsize, char** datetime, unsigned int settingId){
    unsigned int hour;
    switch (settingId){
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
