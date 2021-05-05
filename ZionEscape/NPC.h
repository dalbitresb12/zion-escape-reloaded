#pragma once

#ifndef _NPC_H_
#define _NPC_H_

#include "Entity.h"
#include "Node.h"

using namespace System::Collections::Generic;

ref class NPC abstract : public Entity {
  Entity^ tracking;
  Node^ trackingTarget;

public:
  List<Node^>^ path;

  NPC(EntityType entityType, Point pos, unsigned short velocity, float healthPoints, float damagePoints)
    : Entity(entityType, pos, velocity, healthPoints, damagePoints) {
  }

  ~NPC() {
    if (path != nullptr) {
      for each (Node ^ path in this->path)
        delete path;
      this->path->Clear();
      delete this->path;
    }
  }

  property Entity^ Tracking {
    Entity^ get() {
      return tracking;
    }
    void set(Entity^ value) {
      tracking = value;
    }
  }

  property Node^ Target {
    Node^ get() {
      return trackingTarget;
    }
    void set(Node^ value) {
      trackingTarget = value;
    }
  }

  bool NeedsPathUpdate(Grid^ grid) {
    if (path == nullptr)
      return true;

    if (trackingTarget == nullptr && path->Count > 0)
      return true;

    if (trackingTarget == nullptr)
      return true;

    Node^ currentNode = grid->GetNodeFromWorldPoint(Position);
    List<Node^>^ neighboors = grid->GetNeighbours(trackingTarget);

    for each (Node ^ node in neighboors) {
      if (node->Equals(currentNode))
        return false;
    }

    if (path->Count == 0)
      return true;

    return path->Count > 0 && path[0] != trackingTarget;
  }

  Point GetDelta() {
    if (path == nullptr || !(path->Count > 0))
      return Point(0, 0);

    Point currentWaypoint = path[path->Count - 1]->worldPos;

    int deltaX = currentWaypoint.X - drawingArea.X;
    int deltaY = currentWaypoint.Y - drawingArea.Y;

    return Point(deltaX, deltaY);
  }

  void Move(int deltaX, int deltaY, Grid^ grid) override {
    if (path == nullptr || !(path->Count > 0)) {
      StopAnimation();
      return;
    }

    StartAnimation();

    if (deltaX != 0)
      drawingArea.X += deltaX < 0 ? -velocity : velocity;
    if (deltaY != 0)
      drawingArea.Y += deltaY < 0 ? -velocity : velocity;


    Node^ currentWaypoint = path[path->Count - 1];
    Node^ currentNode = grid->GetNodeFromWorldPoint(drawingArea.Location);

    if (currentNode->Equals(currentWaypoint)) {
      path->RemoveAt(path->Count - 1);
    }

    for each (Direction direction in GetDirectionFromDelta(deltaX, deltaY))
      this->SetSpriteDirection(direction);
  }
};

#endif // !_NPC_H_
