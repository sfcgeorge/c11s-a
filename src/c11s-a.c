/* Structure (pulled from thin air but I like it)
 *
 * This file contains all *_update_procs. They must only be used for rendering
 * as they get called often, even if unchanged.
 *
 * Separate methods must be used for calculations and setting variables.
 * These functions should be called by handlers in this file.
 *
 * All layers and rendering is in this file, separate files are used for
 * individual features. Header files are used for corresponding variables.
 */

#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"
#include "battery_complication.h"
#include "bluetooth_complication.h"

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  for (int i = 0; i < NUM_CLOCK_TICKS; i++)
    graphics_fill_rect(ctx, tick_rects[i], 0, 0x0000);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  gpath_move_to(hour_arrow, center);
  gpath_rotate_to(hour_arrow, time_complication_hour_angle);
  gpath_draw_filled(ctx, hour_arrow);

  gpath_move_to(minute_arrow, center);
  gpath_rotate_to(minute_arrow, time_complication_minute_angle);
  gpath_draw_filled(ctx, minute_arrow);

  if (battery_complication_charge_percent > 20) {
    #ifdef PBL_COLOR
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    #endif
    #ifdef PBL_PLATFORM_BASALT
      graphics_context_set_stroke_width(ctx, 3);
    #endif

    GPoint second_hand = {
      .x = (int16_t)(sin_lookup(time_complication_second_angle) * SECOND_HAND_LENGTH / TRIG_MAX_RATIO) + center.x ,
      .y = (int16_t)(-cos_lookup(time_complication_second_angle) * SECOND_HAND_LENGTH / TRIG_MAX_RATIO) + center.y ,
    };
    GPoint second_hand_opp = {
      .x = (int16_t)(-sin_lookup(time_complication_second_angle) * SECOND_HAND_OPP_LENGTH / TRIG_MAX_RATIO) + center.x,
      .y = (int16_t)(cos_lookup(time_complication_second_angle) * SECOND_HAND_OPP_LENGTH / TRIG_MAX_RATIO) + center.y,
    };
    graphics_draw_line(ctx, second_hand_opp, second_hand);
    graphics_fill_circle(ctx, second_hand, 5);
  }

  if (!bluetooth_complication_bluetooth)
    graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, GPoint(center.x, center.y), 3);
}

void window_render(void) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void layer_create_update_add(Layer *layer, Layer *window_layer, LayerUpdateProc update_proc) {
  GRect bounds = layer_get_bounds(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, update_proc);
  layer_add_child(window_layer, layer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  time_complication_update();
  battery_complication_update(battery_state_service_peek());
  bluetooth_complication_update(bluetooth_connection_service_peek());

  layer_create_update_add(bg_layer, window_layer, bg_update_proc);
  layer_create_update_add(hands_layer, window_layer, hands_update_proc);
}

static void window_unload(Window *window) {
  layer_destroy(bg_layer);
  layer_destroy(hands_layer);
  gpath_destroy(minute_arrow);
  gpath_destroy(hour_arrow);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  for (int i = 0; i < NUM_CLOCK_TICKS; i++)
    tick_rects[i] = (GRect) {
      .origin = { tick_points[i][0] - TICK_SIZE / 2, tick_points[i][1] - TICK_SIZE / 2 },
      .size = { TICK_SIZE, TICK_SIZE }
    };

  minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  hour_arrow = gpath_create(&HOUR_HAND_POINTS);

  tick_timer_service_subscribe(SECOND_UNIT, time_complication_handler);
  battery_state_service_subscribe(battery_complication_handler);
  bluetooth_connection_service_subscribe(bluetooth_complication_handler);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
