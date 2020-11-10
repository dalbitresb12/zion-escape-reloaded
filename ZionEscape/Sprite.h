#pragma once
using namespace System;
using namespace System::Drawing;

ref class Sprite {
  Bitmap^ image;
  short col, row, nCols, nRows;
  Rectangle drawingArea;
public:
  void SetPosition(Point point) {
    this->drawingArea.Location = point;
  }
  Point GetPosition() {
    return drawingArea.Location;
  }
  void SetCol(short index) {
    this->col = index;
  }
  void SetRow(short index) {
    this->row = index;
  }
  void Draw(Graphics^ world) {
    world->DrawImage(image, this->drawingArea, this->GetCrop(), GraphicsUnit::Pixel);
    //Se debe controlar en cuáles casos animar el Sprite. Tanto por si está en movimiento o por si se trata de algún Obstáculo.
    this->col = (this->col + 1) % nCols;
  }
  Rectangle GetCrop() {
    short width = this->image->Width / this->nCols;
    short height = this->image->Height / this->nRows;
    short x = this->col * width;
    short y = this->row * height;

    return Rectangle(x, y, width, height);
  }

};
