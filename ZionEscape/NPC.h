#pragma once

using namespace System::Collections::Generic;

ref class NPC : public Entity {
  bool pathEnded;

public:
  List<Node^>^ path;

  NPC(EntityType entityType, Point pos, int velocity, float healthPoints, float damagePoints)
    : Entity(entityType, pos, velocity, healthPoints, damagePoints) {}

  bool HasEndedPath() {
    return pathEnded;
  }

  void PathMovement(Grid^ mapGrid, Entity^ entity) {
    Pathfinder::FindPath(mapGrid, this->GetPosition(), entity->GetPosition(), this);
    this->Move(this->GetDelta());
  }

  Point GetDelta() {
    if (path == nullptr)
      return Point(0, 0);

    Point currentWaypoint = path[path->Count - 1]->worldPos;

    int deltaX = currentWaypoint.X - drawingArea.X;
    int deltaY = currentWaypoint.Y - drawingArea.Y;

    return Point(deltaX, deltaY);
  }

  void Move(int deltaX, int deltaY) override {
    if (path == nullptr)
      return;

    if (deltaX != 0)
      drawingArea.X += deltaX < 0 ? -velocity : velocity;
    if (deltaY != 0)
      drawingArea.Y += deltaY < 0 ? -velocity : velocity;

    Point currentWaypoint = path[path->Count - 1]->worldPos;

    if (drawingArea.Location == currentWaypoint) {
      path->Remove(path[path->Count - 1]);
    }
  }
};
