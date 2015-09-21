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
#include "weather_complication.h"

static char complications_default_left[] = "foos";
static char complications_default_right[] = "bars";
static char complications_default[] = "";

int int_round(double r) {
  return (r > 0.0) ? (r + 0.5) : (r - 0.5);
}

void window_render(void) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void set_colors(GContext *ctx, GColor color) {
  graphics_context_set_fill_color(ctx, color);
  graphics_context_set_text_color(ctx, color);
  graphics_context_set_stroke_color(ctx, color);
}

static void set_invert_color(bool invert, GContext *ctx) {
  GColor color;
  if (persist_read_int(INVERT_BACKGROUND) != 0) {
    color = invert ?
      GColorWhite :
      GColorBlack;
  } else {
    color = invert ?
      GColorBlack :
      GColorWhite;
  }
  set_colors(ctx, color);
}

static void set_background_color(GContext *ctx) {
  set_invert_color(false, ctx);
}

static void set_foreground_color(GContext *ctx) {
  set_invert_color(true, ctx);
}

static void set_main_color(GContext *ctx) {
  #ifdef PBL_COLOR
    GColor color = persist_exists(MAIN_COLOR) ?
      GColorFromHEX(persist_read_int(MAIN_COLOR)) :
      GColorDarkGray;
    set_colors(ctx, color);
  #else
    set_foreground_color(ctx);
  #endif
}

static void set_hands_color(GContext *ctx) {
  #ifdef PBL_COLOR
    GColor color = persist_exists(HANDS_COLOR) ?
      GColorFromHEX(persist_read_int(HANDS_COLOR)) :
      GColorBlack;
    set_colors(ctx, color);
  #else
    set_foreground_color(ctx);
  #endif
}

static void set_seconds_color(GContext *ctx) {
  #ifdef PBL_COLOR
    GColor color = persist_exists(SECONDS_COLOR) ?
      GColorFromHEX(persist_read_int(SECONDS_COLOR)) :
      GColorRed;
    set_colors(ctx, color);
  #else
    set_foreground_color(ctx);
  #endif
}

static void complications_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  // int inset_x = 8; int inset_y = -3; int text_h = 14;

  // set_main_color(ctx);
  // graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // set_background_color(ctx);
  // GRect background_rect = GRect(0, text_h, bounds.size.w, bounds.size.h - text_h * 2);
  // graphics_fill_rect(ctx, background_rect, 4, GCornersAll);

  // set_main_color(ctx);
  // for (int i = 0; i < NUM_CLOCK_TICKS; i++)
    // graphics_fill_rect(ctx, tick_rects[i], 0, GCornerNone);

  // set_background_color(ctx);
  // GFont *font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

  // GRect top_frame = GRect(inset_x, inset_y, bounds.size.w - inset_x * 2, text_h);
  // GRect bottom_frame = GRect(inset_x, bounds.size.h + inset_y - text_h, bounds.size.w - inset_x * 2, text_h);
  int inset_x = 8; int inset_y = 6; int text_h = 14;

  set_background_color(ctx);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  set_main_color(ctx);
  for (int i = 0; i < NUM_CLOCK_TICKS; i++)
    graphics_fill_rect(ctx, tick_rects[i], 0, GCornerNone);

  set_foreground_color(ctx);
  GFont *font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

  GRect top_frame = GRect(inset_x, inset_y, bounds.size.w - inset_x * 2, text_h + inset_y);
  GRect bottom_frame = GRect(inset_x, bounds.size.h - inset_y - text_h - 5, bounds.size.w - inset_x * 2, text_h);

  char text[9];
  strcpy(text, complications_tl.left);
  graphics_draw_text(
    ctx,
    strcat(text, complications_tl.right),
    font,
    top_frame,
    GTextOverflowModeFill,
    GTextAlignmentLeft,
    NULL
  );
  strcpy(text, complications_tr.left);
  graphics_draw_text(
    ctx,
    strcat(text, complications_tr.right),
    font,
    top_frame,
    GTextOverflowModeFill,
    GTextAlignmentRight,
    NULL
  );
  strcpy(text, complications_bl.left);
  graphics_draw_text(
    ctx,
    strcat(text, complications_bl.right),
    font,
    bottom_frame,
    GTextOverflowModeFill,
    GTextAlignmentLeft,
    NULL
  );
  strcpy(text, complications_br.left);
  graphics_draw_text(
    ctx,
    strcat(text, complications_br.right),
    font,
    bottom_frame,
    GTextOverflowModeFill,
    GTextAlignmentRight,
    NULL
  );
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  set_hands_color(ctx);

  gpath_move_to(hour_arrow, center);
  gpath_rotate_to(hour_arrow, time_complication_hour_angle);
  gpath_draw_filled(ctx, hour_arrow);

  gpath_move_to(minute_arrow, center);
  gpath_rotate_to(minute_arrow, time_complication_minute_angle);
  gpath_draw_filled(ctx, minute_arrow);

  float battery_line_length = (battery_complication_charge_percent / 100.0) * (MINUTE_HAND_LENGTH + HAND_LENGTH_OPP);
  GPoint battery_line_point_opp = {
    .x = int_round(-sin_lookup(time_complication_minute_angle) * HAND_LENGTH_OPP / TRIG_MAX_RATIO) + center.x,
    .y = int_round(cos_lookup(time_complication_minute_angle) * HAND_LENGTH_OPP / TRIG_MAX_RATIO) + center.y,
  };
  GPoint battery_line_point = {
    .x = int_round(sin_lookup(time_complication_minute_angle) * battery_line_length / TRIG_MAX_RATIO) + battery_line_point_opp.x,
    .y = int_round(-cos_lookup(time_complication_minute_angle) * battery_line_length / TRIG_MAX_RATIO) + battery_line_point_opp.y,
  };
  #ifdef PBL_PLATFORM_BASALT
    graphics_context_set_stroke_width(ctx, 1);
  #endif
  set_background_color(ctx);
  graphics_draw_line(ctx, battery_line_point_opp, battery_line_point);

  if (battery_complication_charge_percent > 20 && persist_read_int(HIDE_SECONDS) == 0) {
    set_seconds_color(ctx);
    #ifdef PBL_PLATFORM_BASALT
      graphics_context_set_stroke_width(ctx, 3);
    #endif

    GPoint second_hand = {
      .x = int_round(sin_lookup(time_complication_second_angle) * SECOND_HAND_LENGTH / TRIG_MAX_RATIO) + center.x,
      .y = int_round(-cos_lookup(time_complication_second_angle) * SECOND_HAND_LENGTH / TRIG_MAX_RATIO) + center.y,
    };
    GPoint second_hand_opp = {
      .x = int_round(-sin_lookup(time_complication_second_angle) * HAND_LENGTH_OPP / TRIG_MAX_RATIO) + center.x,
      .y = int_round(cos_lookup(time_complication_second_angle) * HAND_LENGTH_OPP / TRIG_MAX_RATIO) + center.y,
    };
    graphics_draw_line(ctx, second_hand_opp, second_hand);
    if (battery_complication_is_charging) { graphics_fill_circle(ctx, second_hand_opp, 5); }
    else if (battery_complication_is_plugged) {}
    else { graphics_fill_circle(ctx, second_hand, 5); }
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 3);
  }

  if (!bluetooth_complication_bluetooth) {
    set_background_color(ctx);
    graphics_fill_circle(ctx, GPoint(center.x, center.y), 3);
  }
}

static void layer_create_update_add(Layer *layer, Layer *window_layer, LayerUpdateProc update_proc) {
  GRect bounds = layer_get_bounds(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, update_proc);
  layer_add_child(window_layer, layer);
}

static void init_complication(ComplicationZone *complication) {
  *complication = (ComplicationZone) {
    .left = complications_default,
    .right = complications_default,
  };
}

static char* complication_data(int d) {
  switch (d) {
    case TIME_COMPLICATION_YEAR: return time_complication_year;
    case TIME_COMPLICATION_MONTH: return time_complication_month;
    case TIME_COMPLICATION_MONTH_NAME: return time_complication_month_name;
    case TIME_COMPLICATION_WEEK: return time_complication_week;
    case TIME_COMPLICATION_YEAR_DAY: return time_complication_year_day;
    case TIME_COMPLICATION_MONTH_DAY: return time_complication_month_day;
    case TIME_COMPLICATION_MONTH_DAY_SUFFIXED: return time_complication_month_day_suffixed;
    case TIME_COMPLICATION_WEEK_DAY: return time_complication_week_day;
    case TIME_COMPLICATION_DAY_NAME: return time_complication_day_name;
    case TIME_COMPLICATION_HOUR: return time_complication_hour;
    case TIME_COMPLICATION_MINUTE: return time_complication_minute;
    case TIME_COMPLICATION_SECOND: return time_complication_second;
    case TIME_COMPLICATION_AMPM: return time_complication_ampm;
    case TIME_COMPLICATION_DST: return time_complication_dst;
    case BLUETOOTH_COMPLICATION_BLUETOOTH_CONNECTION: return bluetooth_complication_bluetooth_connection;
    case BATTERY_COMPLICATION_CHARGER_CONNECTION: return battery_complication_charger_connection;
    case BATTERY_COMPLICATION_CHARGE_LEVEL: return battery_complication_charge_level;
    case WEATHER_COMPLICATION_TEMPERATURE: return weather_complication_temperature;
    case WEATHER_COMPLICATION_HIGH: return weather_complication_high;
    case WEATHER_COMPLICATION_LOW: return weather_complication_low;
    case WEATHER_COMPLICATION_HUMIDITY: return weather_complication_humidity;
    case WEATHER_COMPLICATION_WIND_SPEED: return weather_complication_wind_speed;
    default: return complications_default;
  }
}

static void set_complication(ComplicationZone *complication, int l, int r) {
  complication->left = complication_data(l);
  complication->right = complication_data(r);
}

static void load_complication_settings(void) {
  set_complication(
    &complications_tl,
    persist_read_int(COMPLICATIONS_TL_L),
    persist_read_int(COMPLICATIONS_TL_R)
  );
  set_complication(
    &complications_tr,
    persist_read_int(COMPLICATIONS_TR_L),
    persist_read_int(COMPLICATIONS_TR_R)
  );
  set_complication(
    &complications_bl,
    persist_read_int(COMPLICATIONS_BL_L),
    persist_read_int(COMPLICATIONS_BL_R)
  );
  set_complication(
    &complications_br,
    persist_read_int(COMPLICATIONS_BR_L),
    persist_read_int(COMPLICATIONS_BR_R)
  );

  if (persist_read_int(HIDE_SECONDS) == 1) {
    tick_timer_service_subscribe(MINUTE_UNIT, time_complication_handler);
  }
  else {
    tick_timer_service_subscribe(SECOND_UNIT, time_complication_handler);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  time_complication_update(tm, YEAR_UNIT);
  battery_complication_update(battery_state_service_peek());
  bluetooth_complication_update(bluetooth_connection_service_peek());

  layer_create_update_add(complications_layer, window_layer, complications_update_proc);
  layer_create_update_add(hands_layer, window_layer, hands_update_proc);

  init_complication(&complications_tl);
  init_complication(&complications_tr);
  init_complication(&complications_bl);
  init_complication(&complications_br);

  load_complication_settings();
}

static void window_unload(Window *window) {
  layer_destroy(complications_layer);
  layer_destroy(hands_layer);

  gpath_destroy(minute_arrow);
  gpath_destroy(hour_arrow);
}

static void inbox_received_handler(DictionaryIterator *settings, void *context) {
  Tuple *setting = dict_read_first(settings);
  while (setting) {
    persist_write_int(setting->key, setting->value->int32);
    setting = dict_read_next(settings);
  }
  load_complication_settings();
  weather_complication_update();
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

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  app_message_deregister_callbacks();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
