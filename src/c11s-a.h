#pragma once

#include <pebble.h>

int int_round(double r);
void window_render(void);

#define NUM_CLOCK_TICKS 12

// Settings keys
#define INVERT_BACKGROUND 50
#define MAIN_COLOR 51
#define SECONDS_COLOR 52
#define HANDS_COLOR 53

#define COMPLICATIONS_TL_L 1
#define COMPLICATIONS_TL_R 2
#define COMPLICATIONS_TR_L 3
#define COMPLICATIONS_TR_R 4
#define COMPLICATIONS_BL_L 5
#define COMPLICATIONS_BL_R 6
#define COMPLICATIONS_BR_L 7
#define COMPLICATIONS_BR_R 8

#define HIDE_SECONDS 20
#define DISABLE_WEATHER 30
#define TEMPERATURE_UNIT 31

#define WEATHER_TEMPERATURE 40
#define WEATHER_HIGH 41
#define WEATHER_LOW 42
#define WEATHER_HUMIDITY 43
#define WEATHER_WIND_SPEED 44

// Settings values
#define TIME_COMPLICATION_YEAR 1
#define TIME_COMPLICATION_MONTH 2
#define TIME_COMPLICATION_MONTH_NAME 3
#define TIME_COMPLICATION_WEEK 4
#define TIME_COMPLICATION_YEAR_DAY 5
#define TIME_COMPLICATION_MONTH_DAY 6
#define TIME_COMPLICATION_MONTH_DAY_SUFFIXED 7
#define TIME_COMPLICATION_WEEK_DAY 8
#define TIME_COMPLICATION_DAY_NAME 9
#define TIME_COMPLICATION_HOUR 10
#define TIME_COMPLICATION_MINUTE 11
#define TIME_COMPLICATION_SECOND 12
#define TIME_COMPLICATION_AMPM 13
#define TIME_COMPLICATION_DST 14

#define BLUETOOTH_COMPLICATION_BLUETOOTH_CONNECTION 20

#define BATTERY_COMPLICATION_CHARGER_CONNECTION 30
#define BATTERY_COMPLICATION_CHARGE_LEVEL 31

#define WEATHER_COMPLICATION_TEMPERATURE 40
#define WEATHER_COMPLICATION_HIGH 41
#define WEATHER_COMPLICATION_LOW 42
#define WEATHER_COMPLICATION_HUMIDITY 43
#define WEATHER_COMPLICATION_WIND_SPEED 44


static Window *window;

static Layer *bg_layer, *hands_layer, *complications_layer;

typedef struct {
  char *left; // 4 + eol
  char *right;
} ComplicationZone;

ComplicationZone complications_tl;
ComplicationZone complications_tr;
ComplicationZone complications_bl;
ComplicationZone complications_br;

static GRect tick_rects[NUM_CLOCK_TICKS];
static GPath *minute_arrow, *hour_arrow;

static const float SECOND_HAND_LENGTH = 50;

static int TICK_SIZE = 8;

static const int tick_points[NUM_CLOCK_TICKS][2] = {
  // {141, 84}, {141, 124}, {119, 165}, {72, 165}, {25, 165}, {3, 124}, {3, 84}, {3, 44}, {25, 3}, {72, 3}, {119, 3}, {141, 44} // no bars small
  {140, 84}, {140, 123}, {118, 164}, {72, 164}, {26, 164}, {4, 123}, {4, 84}, {4, 45}, {26, 4}, {72, 4}, {118, 4}, {140, 45} // no bars
  // {141, 84}, {141, 124}, {112, 153}, {72, 153}, {32, 153}, {3, 124}, {3, 84}, {3, 44}, {32, 15}, {72, 15}, {112, 15}, {141, 44} // inset with bars small
  // {140, 84}, {140, 123}, {111, 152}, {72, 152}, {33, 152}, {4, 123}, {4, 84}, {4, 45}, {33, 16}, {72, 16}, {111, 16}, {140, 45} // inset with bars
};

static const float MINUTE_HAND_LENGTH = 66;
static const float HAND_LENGTH_OPP = 17.0;
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 17 },
    { 5, 17 },
    { 5, -66 },
    { -5, -66 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 17 },
    { 5, 17 },
    { 5, -44 },
    { -5, -44 }
  }
};
