#pragma once

#include "BitmapManager.h"
#include "Entity.h"

ref class Player : public Entity {
public:
  Player(Point pos)
    : Entity(EntityType::Player, pos, 3, 10.f, 2.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\principal\\principal_m.png");
    this->SetImage(image, 4, 4);
  }

  Player(Bitmap^ image, short nCols, short nRows, Point pos)
    : Entity(EntityType::Player, pos, 3, 10.f, 2.f) {
    this->SetImage(image, nCols, nRows);
  }
};
