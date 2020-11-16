#pragma once

#include "NPC.h"
#include "Entity.h"
#include "BitmapManager.h"

ref class Assassin : public NPC {
public:
  Assassin(Point pos)
    : NPC(EntityType::Assassin, pos, 3, 3.f, 2.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\asesinos\\asesino.png");
    this->SetImage(image, 3, 4);
  }
};
