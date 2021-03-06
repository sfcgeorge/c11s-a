#pragma once

#include <pebble.h>

// int tm_sec;         [> seconds,  range 0 to 59          <]
// int tm_min;         [> minutes, range 0 to 59           <]
// int tm_hour;        [> hours, range 0 to 23             <]
// int tm_mday;        [> day of the month, range 1 to 31  <]
// int tm_mon;         [> month, range 0 to 11             <]
// int tm_year;        [> The number of years since 1900   <]
// int tm_wday;        [> day of the week, range 0 to 6    <]
// int tm_yday;        [> day in the year, range 0 to 365  <]
// int tm_isdst;       [> daylight saving time             <]
char time_complication_year[5];
char time_complication_month[3];
char time_complication_month_name[4];
char time_complication_week[3];
char time_complication_year_day[4];
char time_complication_month_day[3];
char time_complication_month_day_suffixed[5];
char time_complication_week_day[2];
char time_complication_day_name[3];
char time_complication_hour[3];
char time_complication_minute[4];
char time_complication_second[4];
char time_complication_ampm[3];
char time_complication_dst[4];

double time_complication_minute_angle;
double time_complication_hour_angle;
double time_complication_second_angle;

void time_complication_handler(struct tm *tm, TimeUnits units_changed);
void time_complication_update(struct tm *tm, TimeUnits units_changed);

