#pragma once
using namespace System;
using namespace System::Drawing;

ref class Sprite {
  Bitmap^ image;
  short col, row, nCols, nRows;
  bool animatable;
protected:
  Rectangle drawingArea;
public:
  Sprite() {}
  Sprite(short nCols, short nRows, bool animatable) {
    this->nCols = nCols;
    this->nRows = nRows;
    this->animatable = animatable;
  }
  void SetPosition(Point point) {
    this->drawingArea.Location = point;
  }
  Point GetPosition() {
    return drawingArea.Location;
  }
  bool GetAnimatable() {
    return animatable;
  }
  void SetCol(short index) {
    this->col = index;
  }
  void SetRow(short index) {
    this->row = index;
  }
  void Draw(Graphics^ world) {
    world->DrawImage(image, this->drawingArea, this->GetCropArea(), GraphicsUnit::Pixel);
    // If the bool animatable is false, the col won't be added by 1. Because of this, it won't be animated
    if (!this->animatable) return;
    this->col = (this->col + 1) % nCols;
  }
  Rectangle GetCropArea() {
    short width = this->image->Width / this->nCols;
    short height = this->image->Height / this->nRows;
    short x = this->col * width;
    short y = this->row * height;

    return Rectangle(x, y, width, height);
  }
};
