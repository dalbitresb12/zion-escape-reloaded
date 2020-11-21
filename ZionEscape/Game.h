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

  void MapGeneration() {
    this->map->StartGeneration();
  }

  void DrawMapGizmos(Graphics^ world) {
    if (this->map->IsGenerated())
      this->map->Draw(world);
  }

  bool IsGenerated() {
    return this->map->IsGenerated();
  }

  int GetMapSeed() {
    return this->map->GetSeed();
  }
};

#endif // !_GAME_H_


