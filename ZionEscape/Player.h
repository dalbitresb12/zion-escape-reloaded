#pragma once
#include "Entity.h"
using namespace System::Windows::Forms;

ref class Player : public Entity {
public:
  Player() {
    dx = 3; dy = 3;
  }
  void drawPlayer(Graphics^ g) {
    Rectangle cropArea = this->GetCropArea();
    g->DrawImage(this->image, this->drawingArea, cropArea, GraphicsUnit::Pixel);
  }

  void movePlayer(Graphics^ g, Keys key) {
    if (key == Keys::A) {
      if (drawingArea.X > 0) {
        row = 1;
        drawingArea.X -= dx;
      }
    }
    if (key == Keys::D) {
      if (drawingArea.X + drawingArea.Width < g->VisibleClipBounds.Width) {
        row = 2;
        drawingArea.X += dx;
      }
    }

    if (key == Keys::W) {
      if (drawingArea.Y > 0) {
        row = 3;
        drawingArea.Y -= dy;
      }
    }

    if (key == Keys::S) {
      if (drawingArea.Y + drawingArea.Height < g->VisibleClipBounds.Height) {
        row = 0;
        drawingArea.Y += dy;
      }
    }

    col++;
    if (col > 3) col = 0;
  }

};

