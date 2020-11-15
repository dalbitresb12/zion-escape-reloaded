#pragma once

#include "BitmapManager.h"
#include "Entity.h"

using namespace System::Windows::Forms;

ref class Player : public Entity {
public:
  Player()
    : Entity(EntityType::Player, Point(0, 0), 10.f, 2.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\principal\\principal_m.png");
    this->SetImage(image, 4, 4);
  }

  Player(Bitmap^ image, short nCols, short nRows)
    : Entity(EntityType::Player, Point(0, 0), 10.f, 2.f) {
    this->SetImage(image, nCols, nRows);
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

    this->ShiftCol();
  }
};

