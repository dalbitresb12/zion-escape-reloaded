#pragma once

#ifndef _MATHUTILS_H_
#define _MATHUTILS_H_

using namespace System;

namespace MathUtils {
  class Mathf {
  public:
    template <typename T>
    static T Clamp(T value, T min, T max) {
      if (value < min)
        return min;
      else if (value > max)
        return max;
      else
        return value;
    }

    template <typename T>
    static T Clamp01(T value) {
      return Clamp(value, (T)0.0, (T)1.0);
    }

    template <typename T>
    static int RoundToInt(T value) {
      return (int)Math::Round(value);
    }
  };
}

#endif // !_MATHUTILS_H_
