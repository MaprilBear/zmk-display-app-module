#include "settings_panel.hpp"
#include "misc/lv_color.h"
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(display_app, CONFIG_ZMK_LOG_LEVEL);

namespace {
lv_area_t operator-(lv_area_t const &coord1, lv_area_t const &coord2) {
  return lv_area_t{
      coord1.x1 - coord2.x1,
      coord1.y1 - coord2.y1,
      coord1.x2 - coord2.x2,
      coord1.y2 - coord2.y2,
  };
}
} // namespace

void SettingsPanel::setDesc(
    std::function<void(
        lv_draw_label_dsc_t &textDesc, lv_draw_label_dsc_t &hoveredTextDesc,
        lv_draw_rect_dsc_t &hoveredBackgroundDesc, lv_draw_rect_dsc_t &selectedBackgroundDesc, lv_draw_label_dsc_t &selectedTextDesc)> const &callback) {
  callback(textDesc, hoveredTextDesc, hoveredBackgroundDesc, selectedBackgroundDesc, selectedTextDesc);
}

void SettingsPanel::addSetting(Setting *setting, CanvasObject *canvasObject) {
  settings.emplace_back(SettingWithCanvasObject{setting, canvasObject});
}

void SettingsPanel::action(SettingAction action) {
  if (!selected) {
    selected = true;
  } else {
    Setting *currentSetting = settings[hoveredSettingIndex].setting;
    currentSetting->action(action);
  }
  updated = true;
}

bool SettingsPanel::hasSelection() { return selected; }

void SettingsPanel::select() { selected = true; updated = true; }

void SettingsPanel::deselect() {
  selected = false;
  updated = true;
}

void SettingsPanel::hoverNext() {
  if (selected) {
    return;
  }

  hoveredSettingIndex =
      Utils::clamp<std::uint32_t>(hoveredSettingIndex + 1, settings.size() - 1, 0);

  updated = true;
}

void SettingsPanel::hoverPrev() {
  if (selected) {
    return;
  }

  hoveredSettingIndex =
      Utils::clamp<std::uint32_t>(hoveredSettingIndex - 1, settings.size() - 1, 0);

  updated = true;
}

bool SettingsPanel::tick() {
  bool retVal = updated;
  for (auto &it : settings) {
    retVal |= it.canvasObject->tick();
  }
  updated = false;
  return retVal;
}

void SettingsPanel::draw(MiniCanvas *canvas) {
  LOG_DBG("DRAW! %d", hoveredSettingIndex);
  // Draw all settings in a vertical list, placing the hovered one in a
  // "selected" color and invert text color

     lv_layer_t layer;
    lv_canvas_init_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);

    lv_canvas_fill_bg(reinterpret_cast<lv_obj_t*>(canvas), lv_color_white(), LV_OPA_100);

  for (int i = 0; i < settings.size(); i++) {
    lv_area_t adjustedCoords = coords - canvas->img.obj.coords;

    settings[i].canvasObject->setPosition(
        lv_point_t{adjustedCoords.x1 + 150,
                   static_cast<lv_coord_t>(adjustedCoords.y1 + (i * 30))});

    // Draw background first
    if (i == hoveredSettingIndex) {
      lv_area_t rectCoords{adjustedCoords.x1, adjustedCoords.y1 + (i * 30),
                           adjustedCoords.x1 + 130, adjustedCoords.y1 + (i * 30) + 20};
      lv_draw_rect(&layer, selected ? &selectedBackgroundDesc : &hoveredBackgroundDesc, &rectCoords);
    }

    // Draw text
    lv_area_t textCoords{
        adjustedCoords.x1 + 5,
        static_cast<lv_coord_t>(adjustedCoords.y1 + 5 + (i * 30)),
        adjustedCoords.x1 + 200,
        static_cast<lv_coord_t>(adjustedCoords.y1 + 5 + (i * 30) + 30)};
    lv_draw_label_dsc_t &dsc =
        i == hoveredSettingIndex ? (selected ? selectedTextDesc : hoveredTextDesc) : textDesc;
    dsc.text = settings[i].setting->getName().c_str();
    lv_draw_label(&layer, &dsc, &textCoords);

    lv_canvas_finish_layer(reinterpret_cast<lv_obj_t *>(canvas), &layer);

    // Draw setting widget
    settings[i].canvasObject->draw(canvas);
  }
}