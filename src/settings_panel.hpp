#pragma once

#include "canvas_object.hpp"
#include "mini_canvas.hpp"
#include "misc/lv_area.h"
#include "setting.hpp"
#include "utils.hpp"
#include <algorithm>
#include <vector>

struct SettingWithCanvasObject
{
   Setting* setting;
   CanvasObject* canvasObject;
};

lv_area_t operator-(lv_area_t const& coord1, lv_area_t const& coord2);


class SettingsPanel : public CanvasObject
{
   private:
   lv_draw_rect_dsc_t boundingRectDesc;
   std::vector<SettingWithCanvasObject> settings;
   std::uint32_t hoveredSettingIndex;
   lv_draw_label_dsc_t textDesc{};
   lv_draw_label_dsc_t hoveredTextDesc{};
   lv_draw_rect_dsc_t hoveredBackgroundDesc{};

   public:
   SettingsPanel(lv_area_t coords) : CanvasObject(coords), settings{}, hoveredSettingIndex(0)
   {
      lv_draw_label_dsc_init(&textDesc);
      lv_draw_label_dsc_init(&hoveredTextDesc);
      lv_draw_rect_dsc_init(&hoveredBackgroundDesc);
   }

   void setDesc(std::function<void(lv_draw_label_dsc_t& textDesc, lv_draw_label_dsc_t& hoveredTextDesc,
                                   lv_draw_rect_dsc_t& hoveredBackgroundDesc)> const& callback)
   {
      callback(textDesc, hoveredTextDesc, hoveredBackgroundDesc);
   }

   void addSetting(Setting* setting, CanvasObject* canvasObject)
   {
      settings.emplace_back(SettingWithCanvasObject{setting, canvasObject});
   }

   void hoverNext()
   {
      hoveredSettingIndex = Utils::clamp<std::uint32_t>(hoveredSettingIndex++, settings.size(), 0);
   }

   void hoverPrev()
   {
      hoveredSettingIndex = Utils::clamp<std::uint32_t>(hoveredSettingIndex--, settings.size(), 0);
   }

   bool tick() override
   {
      bool retVal = false;
      for (auto& it : settings)
      {
         retVal |= it.canvasObject->tick();
      }
      return retVal;
   }

   void draw(MiniCanvas* canvas) override
   {
      lv_layer_t layer;
      lv_canvas_init_layer(reinterpret_cast<lv_obj_t*>(canvas), &layer);
      // Draw all settings in a vertical list, placing the hovered one in a "selected" color and invert text color
      for (int i = 0; i < settings.size(); i++)
      {
         lv_area_t adjustedCoords = coords - canvas->img.obj.coords;

         settings[i].canvasObject->setPosition(
             lv_point_t{adjustedCoords.x1 + 150, static_cast<lv_coord_t>(adjustedCoords.y1 + (i * 30))});

         // Draw background first
         if (i == hoveredSettingIndex)
         {
            lv_area_t rectCoords{adjustedCoords.x1, adjustedCoords.y1, adjustedCoords.x1 + 130, adjustedCoords.y1 + 20};
            lv_draw_rect(&layer,&hoveredBackgroundDesc, &rectCoords);
         }

         // Draw text
         lv_area_t textCoords{ adjustedCoords.x1 + 5, static_cast<lv_coord_t>(adjustedCoords.y1 + 5 + (i * 30)), adjustedCoords.x1 + 200, static_cast<lv_coord_t>(adjustedCoords.y1 + 5 + (i * 30) + 30)};
         lv_draw_label_dsc_t& dsc = i == hoveredSettingIndex ? hoveredTextDesc : textDesc;
         dsc.text = settings[i].setting->getName().c_str();
         lv_draw_label(&layer, &dsc, &textCoords);

         // Draw setting widget
         settings[i].canvasObject->draw(canvas);
      }

      lv_canvas_finish_layer(reinterpret_cast<lv_obj_t*>(canvas), &layer);
   }
};