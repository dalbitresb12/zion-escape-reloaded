#pragma once
using namespace System;
using namespace System::Drawing;

ref class Sprite {
  Bitmap^ image;
  Size cropSize;
  short col, row, nCols, nRows;
  bool animatable;

protected:
  Rectangle drawingArea;

public:
  Sprite(bool animatable) {
    this->image = nullptr;
    this->nCols = 0;
    this->nRows = 0;
    this->col = 0;
    this->row = 0;
    this->animatable = animatable;
  }

  Sprite(Bitmap^ image, short nCols, short nRows, bool animatable) {
    this->SetImage(image, nCols, nRows);
    this->col = 0;
    this->row = 0;
    this->animatable = animatable;
  }

  void SetImage(Bitmap^ image, short nCols, short nRows) {
    this->image = image;
    this->cropSize = Size(image->Width / nCols, image->Height / nRows);
    this->col = 0;
    this->row = 0;
    this->nCols = nCols;
    this->nRows = nRows;
  }

  void SetPosition(Point point) {
    this->drawingArea.Location = point;
  }

  Point GetPosition() {
    return drawingArea.Location;
  }

  bool IsAnimatable() {
    return animatable;
  }

  short GetCol() {
    return col;
  }

  void SetCol(short index) {
    this->col = index;
  }

  void ShiftCol() {
    this->col = (this->col + 1) % this->nCols;
  }

  short GetRow() {
    return row;
  }

  void SetRow(short index) {
    this->row = index;
  }

  void Draw(Graphics^ world) {
    if (image == nullptr) return;

    world->DrawImage(image, this->drawingArea, this->GetCropArea(), GraphicsUnit::Pixel);

    // If the bool animatable is false, the col won't be added by 1. Because of this, it won't be animated
    if (this->animatable)
      this->ShiftCol();
  }

  Rectangle GetCropArea() {
    Point pos = Point(col * cropSize.Width, row * cropSize.Height);
    return Rectangle(pos, cropSize);
  }
};
