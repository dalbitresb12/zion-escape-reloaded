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

  void Generation(Graphics^ g) {
    this->map->MapGeneration(g);
  }

  bool IsGenerated() {
    return this->map->GetGenerated();
  }
};

#endif // !_GAME_H_


