#pragma once

#include <pebble.h>

void window_render(void);

#define NUM_CLOCK_TICKS 12

static Window *window;

static Layer *bg_layer, *hands_layer;

static GRect tick_rects[NUM_CLOCK_TICKS];
static GPath *minute_arrow, *hour_arrow;

// static BatteryChargeState battery;
// static bool bluetooth;

static const int32_t SECOND_HAND_LENGTH = 50;
static const int32_t SECOND_HAND_OPP_LENGTH = 18;

static int TICK_SIZE = 6;
// static const int tick_points[NUM_CLOCK_TICKS][2] = {
  // {142, 84}, {143, 91}, {143, 99}, {143, 107}, {143, 116}, {142, 124}, {143, 136}, {143, 148}, {143, 163}, {132, 167}, {119, 166}, {109, 167}, {99, 167}, {90, 167}, {81, 167}, {72, 166}, {63, 167}, {54, 167}, {45, 167}, {35, 167}, {25, 166}, {12, 167}, {1, 163}, {1, 148}, {1, 136}, {2, 124}, {1, 116}, {1, 107}, {1, 99}, {1, 91}, {2, 84}, {1, 77}, {1, 69}, {1, 61}, {1, 52}, {2, 44}, {1, 32}, {1, 20}, {1, 5}, {12, 1}, {25, 2}, {35, 1}, {45, 1}, {54, 1}, {63, 1}, {72, 2}, {81, 1}, {90, 1}, {99, 1}, {109, 1}, {119, 2}, {132, 1}, {143, 5}, {143, 20}, {143, 32}, {142, 44}, {143, 52}, {143, 61}, {143, 69}, {143, 77}
// };
static const int tick_points[NUM_CLOCK_TICKS][2] = {
  {141, 84}, {141, 124}, {119, 165}, {72, 165}, {25, 165}, {3, 124}, {3, 84}, {3, 44}, {25, 3}, {72, 3}, {119, 3}, {141, 44}
};

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -4, 18 },
    { 4, 18 },
    { 4, -64 },
    { -4, -64 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -4, 18 },
    { 4, 18 },
    { 4, -44 },
    { -4, -44 }
  }
};
