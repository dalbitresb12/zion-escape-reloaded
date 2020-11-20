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
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;

public:
  Scene(DoorLocations doorLocations, Point pos) {
    this->spawners = gcnew List<SceneSpawner^>;
    this->neighbours = gcnew Dictionary<Direction, Scene^>;
    this->InitDoorsList(doorLocations);
    this->ImageSelector();
    this->drawingArea = Rectangle(pos, this->background->Size);
  }

  ~Scene() {
    for each (SceneSpawner ^ spawner in spawners)
      delete spawner;
    spawners->Clear();
    delete spawners;

    for each (KeyValuePair<Direction, Scene^> element in neighbours)
      delete element.Value;
    neighbours->Clear();
    delete neighbours;
  }

  //Temporal Image Selector -> Works as  a reference
  void ImageSelector() {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    bool up = GetUp();
    bool down = GetDown();
    bool left = GetLeft();
    bool right = GetRight();

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
      /*if (spawners->ContainsKey(dir)) {
        SceneSpawner^ element;
        if (spawners->TryGetValue(dir, element))
          delete element;
        spawners->Remove(dir);
      }*/
      spawners->Add(gcnew SceneSpawner(EnumUtilities::GetInverseDirection(dir), rect));
    }
  }

  void DeleteSpawner(short n) {
    this->spawners->Remove(this->spawners[n]);
  }

  void AddNeighbour(Direction direction, Scene^ scene) {
    neighbours->Add(direction, scene);
  }

  void SetDoors(Direction direction, bool value) {
    if (value && !doors->Contains(direction))
      doors->Add(direction);
    else if (!value && doors->Contains(direction))
      doors->Remove(direction);
    this->ImageSelector();
  }

  void SetUp(bool value) {
    SetDoors(Direction::Up, value);
  }

  void SetDown(bool value) {
    SetDoors(Direction::Down, value);
  }

  void SetLeft(bool value) {
    SetDoors(Direction::Left, value);
  }

  void SetRight(bool value) {
    SetDoors(Direction::Right, value);
  }

  bool GetDoors(Direction direction) {
    return doors->Contains(direction);
  }

  bool GetUp() {
    return GetDoors(Direction::Up);
  }

  bool GetDown() {
    return GetDoors(Direction::Down);
  }

  bool GetLeft() {
    return GetDoors(Direction::Left);
  }

  bool GetRight() {
    return GetDoors(Direction::Right);
  }

  Rectangle GetDrawingArea() {
    return this->drawingArea;
  }

  Point GetPos() {
    return this->drawingArea.Location;
  }

  List<SceneSpawner^>^ GetSpawners() {
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


