#pragma once

#include "zephyr/logging/log.h"
#include <cstdint>
#include <functional>
#include "utils.hpp"
#include <string>

enum SettingAction
{
   TOGGLE,
   INCREASE,
   DECREASE
};

class Setting
{
   protected:
   std::string name;

   public:
   Setting(std::string name) : name(name)
   {
   }

   std::string getName()
   {
      return name;
   }

   virtual void action(SettingAction action) = 0;
};

class SliderSetting : public Setting
{
   protected:
   std::int32_t value;
   std::int32_t max;
   std::int32_t min;
   std::function<void(std::int32_t)> callback;

   public:
   SliderSetting(std::string name, std::int32_t start, std::int32_t max, std::int32_t min,
                 std::function<void(std::int32_t)> const& callback)
       : Setting(name), value(start), max(max), min(min), callback(callback)
   {
   }

   void addDelta(std::int32_t delta)
   {
      std::int32_t newValue = Utils::clamp<std::int32_t>(value + delta, max, min);
      std::int32_t actualDelta = newValue - value;
      LOG_PRINTK("\n\nCurrent Val: %d\n New Val: %d\n Actual Delta %d\n", value, newValue, actualDelta);
      callback(actualDelta);
      value = newValue;
   }

   void setValue(std::int32_t val)
   {
      std::int32_t newValue = Utils::clamp<std::int32_t>(val, max, min);
      std::int32_t delta = newValue - value;
      callback(delta);
      value = newValue;
   }

   std::int32_t getValue()
   {
      return value;
   }

   std::int32_t getMax()
   {
      return max;
   }

   std::int32_t getMin()
   {
      return min;
   }

   void action(SettingAction action) override
   {
      switch (action)
      {
         case SettingAction::INCREASE:
            addDelta(1);
            break;
         case SettingAction::DECREASE:
            addDelta(-1);
            break;
      }
   }
};

class ToggleSetting : public Setting
{
   protected:
   bool currentToggle;
   std::function<void(bool)> callback;

   public:
   ToggleSetting(std::string name, bool start, std::function<void(bool)> callback) : Setting(name), currentToggle(start), callback(callback)
   {
   }

   bool getValue()
   {
      return currentToggle;
   }

   void toggle()
   {
      currentToggle = !currentToggle;
      callback(currentToggle);
   }

   void action(SettingAction action) override
   {
      if (action == SettingAction::TOGGLE)
      {
         LOG_PRINTK("Toggling!");
         toggle();
      }
   }
};