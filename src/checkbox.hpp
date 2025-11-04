#pragma once

#include "canvas_object.hpp"
#include "setting.hpp"
#include <lvgl.h>

class Checkbox : public CanvasObject
{
    private:
    ToggleSetting& setting;
    bool oldValue = false;
    lv_draw_rect_dsc_t boundingDesc{};
    lv_draw_rect_dsc_t onDesc{};

    public:
    Checkbox(lv_area_t coords, ToggleSetting& setting) : CanvasObject(coords), setting(setting)
    {
        lv_draw_rect_dsc_init(&boundingDesc);
        lv_draw_rect_dsc_init(&onDesc);
    }

    void setDesc(std::function<void(lv_draw_rect_dsc_t& boundingDesc, lv_draw_rect_dsc_t& onDesc)> const& callback)
    {
       callback(boundingDesc, onDesc);
    }

    bool tick() override;

    void draw(MiniCanvas* canvas) override;
};