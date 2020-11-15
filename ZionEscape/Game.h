#pragma once
#include "Map.h"

ref class Game {
  Map^ map;

public:
  Game() {
    this->map = gcnew Map();
  }

  void Generation(Graphics^ g) {
    this->map->MapGeneration(g);
  }

  bool IsGenerated() {
    return this->map->GetGenerated();
  }
};
