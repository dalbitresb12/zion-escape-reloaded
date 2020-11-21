#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "Map.h"

ref class Game {
  Map^ map;

public:
  Game() {
    this->map = gcnew Map();
  }
  ~Game() {
    delete map;
  }

  void StartGeneration(Graphics^ g) {
    this->map->StartGeneration(g);
  }

  bool IsGenerated() {
    return this->map->IsGenerated();
  }

  int GetMapSeed() {
    return this->map->GetSeed();
  }
};

#endif // !_GAME_H_


