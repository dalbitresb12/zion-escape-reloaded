#pragma once
using namespace System::Drawing;

ref class SceneSpawner {
  Rectangle drawingArea;
  //Door direction of the Scene
  short opDir;
  /*
  * 1 --> Needs a Top Door
  * 2 --> Needs a Down Door
  * 3 --> Needs a Right Door
  * 4 --> Need a Left Door
  */
public:
  SceneSpawner(short opDir, Graphics^ g, Rectangle drawingArea) {
    this->drawingArea = drawingArea;
    this->opDir = opDir;
    //Draw the Spawn (Color is optional)
    g->FillRectangle(Brushes::CornflowerBlue, this->drawingArea);
  }
  short GetOpDir() {
    return this->opDir;
  }
  Rectangle GetDrawingArea() {
    return this->drawingArea;
  }
  Point GetPos() {
    return this->drawingArea.Location;
  }
};
