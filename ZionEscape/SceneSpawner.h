#pragma once
using namespace System::Drawing;

enum class ParentDirection {
  Up, Down, Right, Left
};

ref class SceneSpawner {
  Rectangle drawingArea;
  ParentDirection parentDirection;
 
public:
  SceneSpawner(ParentDirection parentDirection, Graphics^ g, Rectangle drawingArea) {
    this->drawingArea = drawingArea;
    this->parentDirection = parentDirection;
    //Draw the Spawn (Color is optional)
    g->FillRectangle(Brushes::CornflowerBlue, this->drawingArea);
  }

  bool Collides(Rectangle area) {
    return this->drawingArea.IntersectsWith(area);
  }

  ParentDirection GetParentDirection() {
    return this->parentDirection;
  }

  Rectangle GetDrawingArea() {
    return this->drawingArea;
  }

  Point GetPos() {
    return this->drawingArea.Location;
  }
};
