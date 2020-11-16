#pragma once

#include "NPC.h"
#include "Entity.h"
#include "BitmapManager.h"

ref class Corrupt : public NPC {
public:
  Corrupt(Point pos)
    : NPC(EntityType::Corrupt, pos, 2, 5.f, 1.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\corruptos\\corrupto.png");
    this->SetImage(image, 4, 4);
  }

  void ConvertToAlly() {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\aliados\\aliado-malo.png");
    this->SetImage(image, 4, 4);
  }
};

