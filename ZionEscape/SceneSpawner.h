#pragma once
using namespace System::Drawing;

enum class ParentDirection {
  //The new scene must have a door looking in the direction:
  Up, Down, Right, Left
};

ref class SceneSpawner {
  Rectangle drawingArea;
  ParentDirection parentDirection;
 
public:
  SceneSpawner(ParentDirection parentDirection, Rectangle drawingArea) {
    this->drawingArea = drawingArea;
    this->parentDirection = parentDirection;
  }
  ParentDirection GetParentDirection() {
    return this->parentDirection;
  }
  Point GetPos() {
    return this->drawingArea.Location;
  }
};
