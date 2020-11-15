#pragma once

#include "BitmapManager.h"
#include "Entity.h"

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
};

