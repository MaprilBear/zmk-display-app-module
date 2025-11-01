#pragma once

#include <lvgl.h>
#include <widgets/canvas/lv_canvas_private.h>
#include <cstdint>

struct MiniCanvas : public lv_canvas_t
{
   std::uint8_t* canvasBuffer{};
};