#pragma once
#include "Sprite.h"
using namespace System;

ref class Entity : public Sprite {
protected:
  String^ name;
  int x, y;
  bool movable;
  float health;
  float damagePoints;
public:
  Entity(String^ name, Point pos, bool movable, float health, float damagePoints) {
    this->name = name;
    this->x = pos.X;
    this->y = pos.Y;
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
    //Debe ser drawingArea protected
    //return this->drawingArea.IntersectsWith(object.drawingArea);
  }
  virtual void Move(short dx, short dy) {
    //x y no están enlazados con la posicion de drawingArea
    this->x += dx;
    this->y += dy;
    /*
    * Una posibilidad podría hacer esto
    this->drawingArea.X += dx;
    this->drawingArea.Y += dy;
    * O también colocar el x y en sprite
    */
  }
};

