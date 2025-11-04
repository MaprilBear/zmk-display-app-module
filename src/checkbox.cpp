#include "checkbox.hpp"
#include "mini_canvas.hpp"
#include "misc/lv_area.h"
#include "utils.hpp"
#include "zephyr/logging/log.h"
#include <lvgl.h>


LOG_MODULE_DECLARE(display_app, CONFIG_ZMK_LOG_LEVEL);

lv_area_t operator-(lv_area_t const &coord1, lv_point_t const &offset);

bool Checkbox::tick() {
  bool retVal = setting.getValue() != oldValue;
  oldValue = setting.getValue();
  return retVal;
}

void Checkbox::draw(MiniCanvas *canvas) {
  lv_layer_t layer;
  lv_canvas_init_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);

  lv_area_t adjustedCoords =
      coords - lv_point_t{canvas->img.obj.coords.x1, canvas->img.obj.coords.y1};

  LOG_PRINTK("X1: %d\nY1: %d\n", adjustedCoords.x1, adjustedCoords.y1);
  LOG_PRINTK("X2: %d\nY2: %d\n", adjustedCoords.x2, adjustedCoords.y2);

  lv_draw_rect(&layer, &boundingDesc, &adjustedCoords);

  if (setting.getValue()) {
    lv_draw_rect(&layer, &onDesc, &adjustedCoords);
  }

  lv_canvas_finish_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);
}