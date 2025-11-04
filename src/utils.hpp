#ifndef UTILS_H
#define UTILS_H

#include <lvgl.h>
#include <cstdint>
#include <algorithm>

namespace Utils
{
   template <class T> constexpr T clamp(T val, T max, T min)
   {
      return std::min(max, std::max(min, val));
   }

   inline lv_color16_t color_swap(lv_color16_t color)
   {
      std::uint8_t swap = color.red;
      color.red = color.blue;
      color.blue = swap;

      return color;
   }
}

#endif