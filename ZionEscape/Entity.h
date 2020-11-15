#pragma once

#include "Sprite.h"

using namespace System;
using namespace System::Windows::Forms;

enum class EntityType {
  Player, Obstacle, Assassin, Corrupt, Ally
};

enum class Direction {
  Up, Down, Left, Right
};

// Entity publicly inheriting Sprite
ref class Entity : public Sprite {
protected:
  EntityType entityType;
  bool movable;
  float healthPoints;
  float damagePoints;
  int velocity;

public:
  Entity(EntityType entityType, Point pos, float healthPoints, float damagePoints)
    : Sprite(entityType != EntityType::Obstacle) {
    this->entityType = entityType;
    this->drawingArea.Location = pos;
    this->healthPoints = healthPoints;
    this->damagePoints = damagePoints;
    this->movable = entityType != EntityType::Obstacle;
  }

  bool IsMovable() {
    return this->movable;
  }

  float GetHealth() {
    return this->healthPoints;
  }

  void SetHealth(float healthPoints) {
    this->healthPoints = healthPoints;
  }

  float GetDamagePoints() {
    return this->damagePoints;
  }

  bool Collision(Entity^ object) {
    // Collision needs the drawingArea of another Entity to return if the object collides or not
    return this->drawingArea.IntersectsWith(object->drawingArea);
  }

  virtual void Move(Direction direction) {
    if (direction == Direction::Up || direction == Direction::Down)
      this->drawingArea.X += direction == Direction::Up ? -velocity : velocity;
    if (direction == Direction::Left || direction == Direction::Right)
      this->drawingArea.Y += direction == Direction::Left ? -velocity : velocity;
  }

  virtual void Move(int deltaX, int deltaY) {
    if (deltaX != 0)
      this->drawingArea.X += deltaX < 0 ? -velocity : velocity;
    if (deltaY != 0)
      this->drawingArea.Y += deltaY < 0 ? -velocity : velocity;
  }

  virtual void Move(Keys key) {
    this->Move(GetDirectionFromKey(key));
  }

  static Direction GetDirectionFromKey(Keys key) {
    switch (key) {
      case Keys::W:
        return Direction::Up;
      case Keys::S:
        return Direction::Down;
      case Keys::A:
        return Direction::Left;
      case Keys::D:
        return Direction::Right;
    }
  }

  virtual void Move(Graphics^ g, short dx, short dy, Keys key) {
    return;
  }
};

