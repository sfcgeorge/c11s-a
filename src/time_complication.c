#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"

const char MONTH_NAMES[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char DAY_NAMES[7][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
const char DAY_SUFFIXES[32][5] = {"", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th", "21st", "22nd", "23rd", "24th", "25th", "26th", "27th", "28th", "29th", "30th", "31st"};

void time_complication_update(struct tm *tm, TimeUnits units_changed) {
  // Get weather update every 30 minutes
  if (persist_read_int(DISABLE_WEATHER) != 1 && tm->tm_min % 30 == 0) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, 0, 0);
    app_message_outbox_send();
  }

  if (tm->tm_sec >= 10) { snprintf(time_complication_second, 4, ".%d", tm->tm_sec); }
  else if (tm->tm_sec > 0 && tm->tm_sec < 10) { snprintf(time_complication_second, 4, ".0%d", tm->tm_sec); }
  else { snprintf(time_complication_second, 4, ".00"); }
  time_complication_second_angle = TRIG_MAX_ANGLE * tm->tm_sec / 60;

  if (units_changed <= SECOND_UNIT) return;

  if (tm->tm_min >= 10) { snprintf(time_complication_minute, 4, ":%d", tm->tm_min); }
  else if (tm->tm_min > 0 && tm->tm_min < 10) { snprintf(time_complication_minute, 4, ":0%d", tm->tm_min); }
  else { snprintf(time_complication_minute, 4, ":00"); }
  time_complication_minute_angle = TRIG_MAX_ANGLE * tm->tm_min / 60;

  if (units_changed <= MINUTE_UNIT) return;

  int tm_hour = tm->tm_hour;
  if (!clock_is_24h_style()) {
    tm_hour = tm_hour % 12;
    if (tm_hour == 0) tm_hour = 12;
  }
  if (tm_hour >= 10) { snprintf(time_complication_hour, 3, "%d", tm_hour); }
  else if (tm_hour > 0 && tm_hour < 10) { snprintf(time_complication_hour, 3, "0%d", tm_hour); }
  else { snprintf(time_complication_hour, 3, "00"); }
  strncpy(time_complication_ampm, (tm->tm_hour < 12 ? "AM" : "PM"), 3);
  strncpy(time_complication_dst, (tm->tm_isdst == 1 ? "DST" : ""), 4);
  time_complication_hour_angle = (TRIG_MAX_ANGLE * (((tm->tm_hour % 12) * 6) + (tm->tm_min / 10))) / (12 * 6);

  if (units_changed <= HOUR_UNIT) return;

  snprintf(time_complication_week, 3, "%d", (int)(tm->tm_yday / 7 + 1));
  snprintf(time_complication_year_day, 4, "%d", tm->tm_yday + 1);
  snprintf(time_complication_month_day, 3, "%d", tm->tm_mday);
  strncpy(time_complication_month_day_suffixed, DAY_SUFFIXES[tm->tm_mday], 5);
  snprintf(time_complication_week_day, 2, "%d", tm->tm_wday + 1);
  strncpy(time_complication_day_name, DAY_NAMES[tm->tm_wday], 3);

  if (units_changed <= DAY_UNIT) return;

  snprintf(time_complication_month, 3, "%d", tm->tm_mon + 1);
  strncpy(time_complication_month_name, MONTH_NAMES[tm->tm_mon], 4);

  if (units_changed <= MONTH_UNIT) return;

  snprintf(time_complication_year, 5, "%d", 1900 + tm->tm_year);
}

void time_complication_handler(struct tm *tm, TimeUnits units_changed) {
  time_complication_update(tm, units_changed);
  window_render();
}
