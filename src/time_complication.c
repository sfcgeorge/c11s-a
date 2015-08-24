#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"

const char MONTH_NAMES[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char DAY_NAMES[7][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
const char DAY_SUFFIXES[32][5] = {"", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th", "21st", "22nd", "23rd", "24th", "25th", "26th", "27th", "28th", "29th", "30th", "31st"};

void time_complication_update(void) {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  snprintf(time_complication_year, 5, "%d", 1900 + tm->tm_year);
  snprintf(time_complication_month, 3, "%d", tm->tm_mon + 1);
  strncpy(time_complication_month_name, MONTH_NAMES[tm->tm_mon], 4);
  snprintf(time_complication_week, 3, "%d", (int)(tm->tm_yday / 7 + 1));
  snprintf(time_complication_year_day, 4, "%d", tm->tm_yday + 1);
  snprintf(time_complication_month_day, 3, "%d", tm->tm_mday);
  strncpy(time_complication_month_day_suffixed, DAY_SUFFIXES[tm->tm_mday], 5);
  snprintf(time_complication_week_day, 2, "%d", tm->tm_wday + 1);
  strncpy(time_complication_day_name, DAY_NAMES[tm->tm_wday], 3);
  snprintf(time_complication_hour, 3, "%d", tm->tm_hour);
  snprintf(time_complication_minute, 3, "%d", tm->tm_min);
  snprintf(time_complication_second, 3, "%d", tm->tm_sec);
  strncpy(time_complication_ampm, (tm->tm_hour < 12 ? "AM" : "PM"), 3);
  strncpy(time_complication_dst, (tm->tm_isdst == 1 ? "DST" : ""), 4);

  time_complication_hour_angle = (TRIG_MAX_ANGLE * (((tm->tm_hour % 12) * 6) + (tm->tm_min / 10))) / (12 * 6);
  time_complication_minute_angle = TRIG_MAX_ANGLE * tm->tm_min / 60;
  time_complication_second_angle = TRIG_MAX_ANGLE * tm->tm_sec / 60;
}

void time_complication_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_complication_update();
  window_render();
}
