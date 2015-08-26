#include <pebble.h>
#include "c11s-a.h"
#include "weather_complication.h"

static int convert_temperature_unit(int temperature) {
  if (persist_read_int(TEMPERATURE_UNIT) == 1) return int_round(temperature * 1.8) + 32;
  return temperature;
}

static int convert_wind_unit(int wind) {
  if (persist_read_int(TEMPERATURE_UNIT) == 1) return int_round(wind * 0.621371);
  return wind;
}

void weather_complication_update(void) {
  snprintf(weather_complication_temperature, 5, "%d\u00B0", convert_temperature_unit(persist_read_int(WEATHER_TEMPERATURE)));
  snprintf(weather_complication_high, 5, "<%d", convert_temperature_unit(persist_read_int(WEATHER_HIGH)));
  snprintf(weather_complication_low, 5, ">%d", convert_temperature_unit(persist_read_int(WEATHER_LOW)));
  snprintf(weather_complication_humidity, 5, "%d%%", (int)persist_read_int(WEATHER_HUMIDITY));
  snprintf(weather_complication_wind_speed, 5, "%d", (int)convert_wind_unit(persist_read_int(WEATHER_WIND_SPEED)));
}

void weather_message(void) {
  if (persist_read_int(DISABLE_WEATHER) != 1) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, 0, 0);
    app_message_outbox_send();
  }
}
