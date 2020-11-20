#pragma once

#ifndef _ENUMS_H_
#define _ENUMS_H_

public enum class EntityType {
  Player, Obstacle, Assassin, Corrupt, Ally
};

public enum class Direction {
  Up, Down, Left, Right
};

namespace EnumUtilities {
  static Direction GetInverseDirection(Direction dir) {
    switch (dir) {
      case Direction::Up:
        return Direction::Down;
        break;
      case Direction::Down:
        return Direction::Up;
        break;
      case Direction::Left:
        return Direction::Right;
        break;
      case Direction::Right:
        return Direction::Left;
        break;
      default:
        return Direction::Up;
        break;
    }
  }
}

#endif // !_ENUMS_H_
