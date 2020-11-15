#pragma once
using namespace System::Drawing;

enum class OpDir {
  Up, Down, Right, Left
  /*
 * Top --> Needs a Top Door
 * Down --> Needs a Down Door
 * Right --> Needs a Right Door
 * Left --> Need a Left Door
 */
};

ref class SceneSpawner {
  Rectangle drawingArea;
  OpDir openDirection;
 
public:
  SceneSpawner(OpDir openDirection , Graphics^ g, Rectangle drawingArea) {
    this->drawingArea = drawingArea;
    this->openDirection = openDirection;
    //Draw the Spawn (Color is optional)
    g->FillRectangle(Brushes::CornflowerBlue, this->drawingArea);
  }
  bool Collides(Rectangle area) {
    return this->drawingArea.IntersectsWith(area);
  }
  OpDir GetOpDir() {
    return this->openDirection;
  }
  Rectangle GetDrawingArea() {
    return this->drawingArea;
  }
  Point GetPos() {
    return this->drawingArea.Location;
  }

};
