#pragma once

#include <pebble.h>

char weather_complication_temperature[5];
char weather_complication_high[5];
char weather_complication_low[5];
char weather_complication_humidity[5];
char weather_complication_wind_speed[5];

void weather_complication_update(void);
void weather_message(void);
