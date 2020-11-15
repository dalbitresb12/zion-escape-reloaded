#pragma once
#include "Sprite.h"
using namespace System;

//Entity publicly inheriting Sprite
ref class Entity : public Sprite {
protected:
  String^ name;
  bool movable;
  float health;
  float damagePoints;
public:
  Entity() {}
  Entity(short nCols, short nRows, bool animatable, String^ name, Point pos, bool movable, float health, float damagePoints)
    :Sprite(nCols, nRows, animatable) {
    this->name = name;
    this->drawingArea.Location = pos;
    this->movable = movable;
    this->health = health;
    this->damagePoints = damagePoints;
  }
  ~Entity() {}
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
    //Collision needs the drawingArea of another Entity to return if the object collides or not
    return this->drawingArea.IntersectsWith(object.drawingArea);
  }
  virtual void Move(short dx, short dy) {
    this->drawingArea.X += dx;
    this->drawingArea.Y += dy;
  }
  virtual void Move(Graphics^ g, short dx, short dy, Keys key) { return; }

};

