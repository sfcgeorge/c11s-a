#pragma once

#include <pebble.h>

double time_complication_minute_angle;
double time_complication_hour_angle;
double time_complication_second_angle;

void time_complication_handler(struct tm *tick_time, TimeUnits units_changed);
void time_complication_update(void);
