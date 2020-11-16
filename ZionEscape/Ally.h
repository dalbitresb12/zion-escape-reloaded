#pragma once

#include "NPC.h"
#include "Entity.h"
#include "BitmapManager.h"

using namespace System;
using namespace System::Drawing;

ref class Ally : public NPC {
public:
  Ally(Point pos)
    : NPC(EntityType::Ally, pos, 2, 1.f, 0.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\aliados\\aliado-bueno.png");
    this->SetImage(image, 4, 4);
  }

  bool ShouldDie() {
    Random r;
    int value = r.Next(1, 101); // (inicio, final + 1)
    return value > 57; //  (100 - num / 100) = % prob
  }
};
