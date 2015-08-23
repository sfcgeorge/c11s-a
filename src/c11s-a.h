#pragma once

#include <pebble.h>

void window_render(void);

#define NUM_CLOCK_TICKS 12

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
