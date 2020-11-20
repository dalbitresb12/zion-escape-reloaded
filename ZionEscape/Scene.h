#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneSpawner.h"
#include "BitmapManager.h"
#include "Enums.h"

using namespace System::Drawing;
using namespace System::Collections::Generic;

class DoorLocations {
public:
  bool Up, Down, Left, Right;

  DoorLocations() {
    this->SetAll(false, false, false, false);
  }

  DoorLocations(bool up, bool down, bool left, bool right) {
    this->SetAll(up, down, left, right);
  }

  ~DoorLocations() {}

  bool IsAllTrue() {
    return Up && Down && Left && Right;
  }

  void SetAll(bool value) {
    SetAll(value, value, value, value);
  }

  void SetAll(bool x, bool y) {
    SetAll(y, y, x, x);
  }

  void SetAll(bool up, bool down, bool left, bool right) {
    this->Up = up;
    this->Down = down;
    this->Left = left;
    this->Right = right;
  }
};

ref class Scene {
  Bitmap^ background;
  List<Direction>^ doors;
  Dictionary<Direction, Scene^>^ neighbours;
  Dictionary<Direction, SceneSpawner^>^ spawners;
  Rectangle drawingArea;

public:
  Scene(DoorLocations doorLocations, Point pos) {
    this->spawners = gcnew Dictionary<Direction, SceneSpawner^>;
    this->neighbours = gcnew Dictionary<Direction, Scene^>;
    this->InitDoorsList(doorLocations);
    this->ImageSelector();
    this->drawingArea = Rectangle(pos, this->background->Size);
  }

  ~Scene() {
    if (doors != nullptr) {
      doors->Clear();
      delete doors;
    }

    if (spawners != nullptr) {
      for each (KeyValuePair<Direction, SceneSpawner^> element in spawners)
        delete element.Value;
      spawners->Clear();
      delete spawners;
    }

    if (neighbours != nullptr) {
      for each (KeyValuePair<Direction, Scene^> element in neighbours)
        delete element.Value;
      neighbours->Clear();
      delete neighbours;
    }
  }

  //Temporal Image Selector -> Works as  a reference
  void ImageSelector() {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    bool up = GetDoorValue(Direction::Up);
    bool down = GetDoorValue(Direction::Down);
    bool left = GetDoorValue(Direction::Left);
    bool right = GetDoorValue(Direction::Right);

    if (!up && down && !right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\D.png");

    else if (!up && down && right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\DR.png");

    else if (up && down && right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\G.png");

    else if (!up && !down && !right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\L.png");

    else if (!up && !down && right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\RL.png");

    else if (!up && !down && right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\R.png");

    else if (up && !down && !right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\T.png");

    else if (up && down && !right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TD.png");

    else if (up && !down && !right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TL.png");

    else if (up && !down && right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TR.png");

    else if (!up && down && !right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\DL.png");

    else if (!up && down && right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\DRL.png");

    else if (up && down && !right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TDL.png");

    else if (up && down && right && !left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TDR.png");

    else if (up && !down && right && left)
      background = bmpManager->GetImage("assets\\sprites\\colliders\\TRL.png");
  }

  void Draw(Graphics^ g) {
    g->DrawImage(this->background, this->drawingArea);
  }

  void CreateSpawner(Point pos) {
    Point location;
    Size size = Size(this->background->Width, this->background->Height);

    for each (Direction dir in doors) {
      if (dir == Direction::Up || dir == Direction::Down)
        location = Point(pos.X, pos.Y + (dir == Direction::Up ? -size.Height : size.Height));
      if (dir == Direction::Left || dir == Direction::Right)
        location = Point(pos.X + (dir == Direction::Left ? -size.Width : size.Width), pos.Y);

      Rectangle rect = Rectangle(location, size);
      AddSpawner(dir, gcnew SceneSpawner(EnumUtilities::GetInverseDirection(dir), rect));
    }
  }

  void AddSpawner(Direction direction, SceneSpawner^ spawner) {
    DeleteSpawner(direction);
    spawners->Add(direction, spawner);
  }

  void DeleteSpawner(Direction direction) {
    if (spawners->ContainsKey(direction)) {
      SceneSpawner^ element;
      if (spawners->TryGetValue(direction, element))
        delete element;
      spawners->Remove(direction);
    }
  }

  void AddNeighbour(Direction direction, Scene^ scene) {
    neighbours->Add(direction, scene);
  }

  void SetDoorValue(Direction direction, bool value) {
    if (value && !doors->Contains(direction))
      doors->Add(direction);
    else if (!value && doors->Contains(direction))
      doors->Remove(direction);
    this->ImageSelector();
  }

  bool GetDoorValue(Direction direction) {
    return doors->Contains(direction);
  }

  Rectangle GetDrawingArea() {
    return this->drawingArea;
  }

  Point GetPos() {
    return this->drawingArea.Location;
  }

  Dictionary<Direction, SceneSpawner^>^ GetSpawners() {
    return this->spawners;
  }

private:
  void InitDoorsList(DoorLocations locations) {
    if (doors != nullptr) {
      doors->Clear();
    } else {
      doors = gcnew List<Direction>;
    }

    if (locations.Up) doors->Add(Direction::Up);
    if (locations.Down) doors->Add(Direction::Down);
    if (locations.Left) doors->Add(Direction::Left);
    if (locations.Right) doors->Add(Direction::Right);
  }
};

#endif // !_SCENE_H_


