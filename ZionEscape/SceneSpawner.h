#pragma once

#ifndef _SCENESPAWNER_H_
#define _SCENESPAWNER_H_

#include "Enums.h"

using namespace System::Drawing;

ref class SceneSpawner {
  Rectangle drawingArea;
  // The new scene must have a door looking in the direction
  Direction parentDirection;
 
public:
  SceneSpawner(Direction parentDirection, Rectangle drawingArea) {
    this->drawingArea = drawingArea;
    this->parentDirection = parentDirection;
  }

  ~SceneSpawner() {}

  Direction GetParentDirection() {
    return this->parentDirection;
  }

  Point GetPos() {
    return this->drawingArea.Location;
  }
};

#endif // _SCENESPAWNER_H_
