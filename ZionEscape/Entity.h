#pragma once
#include "Sprite.h"
using namespace System;

enum class EntityType {
  Player, Obstacle, Assassin, Corrupt, Ally
};

enum class MovementDirection {
  Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight
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

  virtual void Move(MovementDirection direction) {
    switch (direction) {
    case MovementDirection::Up:
      this->drawingArea.X -= velocity;
      break;
    case MovementDirection::Down:
      this->drawingArea.X += velocity;
      break;
    case MovementDirection::Left:
      this->drawingArea.Y -= velocity;
      break;
    case MovementDirection::Right:
      this->drawingArea.Y += velocity;
      break;
    case MovementDirection::UpLeft:
      this->drawingArea.Y -= velocity;
      this->drawingArea.X -= velocity;
      break;
    case MovementDirection::UpRight:
      this->drawingArea.Y -= velocity;
      this->drawingArea.X += velocity;
      break;
    case MovementDirection::DownLeft:
      this->drawingArea.Y += velocity;
      this->drawingArea.X -= velocity;
      break;
    case MovementDirection::DownRight:
      this->drawingArea.Y += velocity;
      this->drawingArea.X += velocity;
      break;
    }
  }

  virtual void Move(Graphics^ g, short dx, short dy, Keys key) {
    return;
  }
};

