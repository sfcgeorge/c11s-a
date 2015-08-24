#pragma once

#include <pebble.h>

void window_render(void);

#define NUM_CLOCK_TICKS 12

// Settings keys
#define COMPLICATIONS_TL_L 1
#define COMPLICATIONS_TL_R 2
#define COMPLICATIONS_TR_L 3
#define COMPLICATIONS_TR_R 4
#define COMPLICATIONS_BL_L 5
#define COMPLICATIONS_BL_R 6
#define COMPLICATIONS_BR_L 7
#define COMPLICATIONS_BR_R 8

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

static const int32_t SECOND_HAND_LENGTH = 51;
static const int32_t SECOND_HAND_OPP_LENGTH = 18;

static int TICK_SIZE = 6;

static const int tick_points[NUM_CLOCK_TICKS][2] = {
  {141, 84}, {141, 124}, {119, 165}, {72, 165}, {25, 165}, {3, 124}, {3, 84}, {3, 44}, {25, 3}, {72, 3}, {119, 3}, {141, 44}
};

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 18 },
    { 5, 18 },
    { 5, -64 },
    { -5, -64 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 18 },
    { 5, 18 },
    { 5, -42 },
    { -5, -42 }
  }
};
