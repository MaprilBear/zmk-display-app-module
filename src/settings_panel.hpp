#pragma once

#include "canvas_object.hpp"
#include "mini_canvas.hpp"
#include "misc/lv_area.h"
#include "setting.hpp"
#include "utils.hpp"
#include <algorithm>
#include "setting.hpp"
#include "zephyr/logging/log.h"
#include <vector>

struct SettingWithCanvasObject
{
   Setting* setting;
   CanvasObject* canvasObject;
};

class SettingsPanel : public CanvasObject
{
   private:
   lv_draw_rect_dsc_t boundingRectDesc;
   std::vector<SettingWithCanvasObject> settings;
   std::uint32_t hoveredSettingIndex = 0;
   lv_draw_label_dsc_t textDesc{};
   lv_draw_label_dsc_t hoveredTextDesc{};
   lv_draw_rect_dsc_t hoveredBackgroundDesc{};
   lv_draw_rect_dsc_t selectedBackgroundDesc{};
   lv_draw_label_dsc_t selectedTextDesc{};
   bool selected = false;
   bool updated = false;

   public:
   SettingsPanel(lv_area_t coords) : CanvasObject(coords), settings{}, hoveredSettingIndex(0)
   {
      lv_draw_label_dsc_init(&textDesc);
      lv_draw_label_dsc_init(&hoveredTextDesc);
      lv_draw_rect_dsc_init(&hoveredBackgroundDesc);
      lv_draw_rect_dsc_init(&selectedBackgroundDesc);
      lv_draw_label_dsc_init(&selectedTextDesc);
   }

   void setDesc(std::function<void(
        lv_draw_label_dsc_t &textDesc, lv_draw_label_dsc_t &hoveredTextDesc,
        lv_draw_rect_dsc_t &hoveredBackgroundDesc, lv_draw_rect_dsc_t &selectedBackgroundDesc, lv_draw_label_dsc_t &selectedTextDesc)> const &callback);

   void addSetting(Setting* setting, CanvasObject* canvasObject);

   void action(SettingAction action);

   bool hasSelection();

   void select();
   void deselect();

   void hoverNext();

   void hoverPrev();

   bool tick() override;


   void draw(MiniCanvas* canvas) override;
};