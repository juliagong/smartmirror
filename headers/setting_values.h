#ifndef SETTINGVALUES_H
#define SETTINGVALUES_H

/*
 * Enums and constants related to settings
 * All the extern variables are defined in bottom of settings.c
 */

typedef struct {
    int settingLevel;
    const char** options;
    unsigned int numOptions;
} setting_options_t;

extern const char RETURN_STRING[];

enum setting_levels {
    SETTING_LEVEL_MAIN = 0,
    SETTING_LEVEL_DATE = 1,
    SETTING_LEVEL_TIME = 2,
    SETTING_LEVEL_TEMPERATURE = 3,
    SETTING_LEVEL_WEATHER = 4,
    SETTING_LEVEL_HEADLINE = 5,
    SETTING_LEVEL_THEME = 6
};

enum main_settings {
    SETTING_DATE = 0,
    SETTING_TIME = 1,
    SETTING_TEMPERATURE = 2,
    SETTING_WEATHER = 3,
    SETTING_HEADLINE = 4,
    SETTING_THEME = 5
};

#define MAIN_SETTINGS_COUNT 6
extern const char *MAIN_SETTINGS_STRING[];

enum time_settings {
    SETTING_TIME_1, /* 11:10 pm */
    SETTING_TIME_2, /* 23:10 */
    SETTING_TIME_3, /* 11:10:03 pm */
    SETTING_TIME_4  /* 23:10:03 */
};

#define TIME_SETTINGS_COUNT 2
extern const char *TIME_SETTINGS_STRING[];

enum date_settings {
    SETTING_DATE_1, /* 1/31/2019 */
    SETTING_DATE_2  /* Jan 31, 2019 */
};

#define DATE_SETTINGS_COUNT 2
extern const char *DATE_SETTINGS_STRING[];

enum temperature_settings {
    SETTING_TEMPERATURE_1, /* F */
    SETTING_TEMPERATURE_2, /* C */
    SETTING_TEMPERATURE_3  /* F / C */
};

#define TEMPERATURE_SETTINGS_COUNT 4
extern const char *TEMPERATURE_SETTINGS_STRING[];

enum weather_settings {
    SETTING_WEATHER_1,
    SETTING_WEATHER_2
};

#define WEATHER_SETTINGS_COUNT 2
extern const char *WEATHER_SETTINGS_STRING[];

enum headline_settings {
    SETTING_HEADLINE_1,
    SETTING_HEADLINE_2
};

#define HEADLINE_SETTINGS_COUNT 2
extern const char *HEADLINE_SETTINGS_STRING[];

#define THEME_SETTINGS_COUNT 5
extern const char *THEME_SETTINGS_STRING[];

enum theme_settings {
    SETTING_THEME_1,
    SETTING_THEME_2,
    SETTING_THEME_3,
    SETTING_THEME_4,
    SETTING_THEME_5
};
#endif
