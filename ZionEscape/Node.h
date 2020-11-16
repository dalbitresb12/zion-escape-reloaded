#pragma once

using namespace System;
using namespace System::Drawing;

ref class Node {
public:
  Point worldPos;
  Point gridPos;
  bool walkable;
  int gCost;
  int hCost;
  Node^ parent;

  Node(Point worldPos, Point gridPos, bool walkable) {
    this->worldPos = worldPos;
    this->gridPos = gridPos;
    this->walkable = walkable;

    this->gCost = 0;
    this->hCost = 0;
    this->parent = nullptr;
  }

  int GetFCost() {
    return this->gCost + this->hCost;
  }
};
