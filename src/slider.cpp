#include "slider.hpp"
#include "mini_canvas.hpp"
#include "misc/lv_area.h"
#include "utils.hpp"
#include "zephyr/logging/log.h"
#include <lvgl.h>


LOG_MODULE_DECLARE(display_app, CONFIG_ZMK_LOG_LEVEL);

lv_area_t operator-(lv_area_t const &coord1, lv_point_t const &offset) {
  return lv_area_t{
      coord1.x1 - offset.x,
      coord1.y1 - offset.y,
      coord1.x2 - offset.x,
      coord1.y2 - offset.y,
  };
}

bool Slider::tick() {
  bool retVal = setting.getValue() != oldValue;
  oldValue = setting.getValue();
  return retVal;
}

void Slider::draw(MiniCanvas *canvas) {
  lv_layer_t layer;
  lv_canvas_init_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);

  lv_area_t adjustedCoords =
      coords - lv_point_t{canvas->img.obj.coords.x1, canvas->img.obj.coords.y1};

  LOG_PRINTK("X1: %d\nY1: %d\n", adjustedCoords.x1, adjustedCoords.y1);
  LOG_PRINTK("X2: %d\nY2: %d\n", adjustedCoords.x2, adjustedCoords.y2);

  // Draw a bounding box the size of the coords
  lv_draw_rect(&layer, &boundingRectDesc, &adjustedCoords);

  // Calculate the percentage that should be filled
  std::int32_t range = setting.getMax() - setting.getMin();
  std::int32_t percent =
      (100 * (setting.getValue() - setting.getMin())) / range;
  std::int32_t sliderWidth =
      ((adjustedCoords.x2 - adjustedCoords.x1) * percent) / 100;

  LOG_PRINTK("Range: %d\n Percent: %d\n Slider Width: %d\n", range, percent,
             sliderWidth);

  // Draw the slider
  lv_area_t rectCoords{adjustedCoords.x1, adjustedCoords.y1,
                       adjustedCoords.x1 + sliderWidth, adjustedCoords.y2};
  LOG_PRINTK("\n\nX1: %d\nY1: %d\n", rectCoords.x1, rectCoords.y1);
  LOG_PRINTK("X2: %d\nY2: %d\n\n\n", rectCoords.x2, rectCoords.y2);
  lv_draw_rect(&layer, &slideRectDesc, &rectCoords);

  lv_canvas_finish_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);
}