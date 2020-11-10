#pragma once
#include "Sprite.h"
using namespace System;

ref class Entity : public Sprite {
protected:
  String^ name;
  bool movable;
  float health;
  float damagePoints;
public:
  Entity(short nCols, short nRows, bool animatable, String^ name, Point pos, bool movable, float health, float damagePoints)
    :Sprite(nCols, nRows, animatable) {
    this->name = name;
    this->drawingArea.X = pos.X;
    this->drawingArea.Y = pos.Y;
    this->movable = movable;
    this->health = health;
    this->damagePoints = damagePoints;
  }
  ~Entity() {
    delete this->name;
  }
  bool IsMovable() {
    return this->movable;
  }
  float GetHealth() {
    return this->health;
  }
  void SetHealth(float health) {
    this->health = health;
  }
  float GetDamagePoint() {
    return this->damagePoints;
  }
  bool Collision(Entity object) {
    return this->drawingArea.IntersectsWith(object.drawingArea);
  }
  virtual void Move(short dx, short dy) {
    this->drawingArea.X += dx;
    this->drawingArea.Y += dy;
  }
};

