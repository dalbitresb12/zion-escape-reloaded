#pragma once
#include "Entity.h"
using namespace System::Windows::Forms;

ref class Player : public Entity {
public:
  Player() {
    
  }

  void Move(Graphics^ g, short dx, short dy, Keys key) override {
    if (key == Keys::A) {
      if (drawingArea.X > 0) {
        this->SetRow(1);
        drawingArea.X -= dx;
      }
    }
    if (key == Keys::D) {
      if (drawingArea.X + drawingArea.Width < g->VisibleClipBounds.Width) {
        this->SetRow(2);
        drawingArea.X += dx;
      }
    }

    if (key == Keys::W) {
      if (drawingArea.Y > 0) {
        this->SetRow(3);
        drawingArea.Y -= dy;
      }
    }

    if (key == Keys::S) {
      if (drawingArea.Y + drawingArea.Height < g->VisibleClipBounds.Height) {
        this->SetRow(0);
        drawingArea.Y += dy;
      }
    }

    SetCol(GetCol() + 1);
    if (GetCol() > 3) this->SetCol(0);
  }

};

