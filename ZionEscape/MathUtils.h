#pragma once

using namespace System;

namespace MathUtils {
  static float Clamp(float value, float min, float max) {
    if (value < min)
      return min;
    else if (value > max)
      return max;
    else
      return value;
  }

  static float Clamp01(float value) {
    return Clamp(value, 0, 1);
  }

  static int RoundToInt(float value) {
    return (int)Math::Round(value);
  }
}
