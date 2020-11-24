#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BitmapManager.h"
#include "Entity.h"

using namespace System;

namespace Defaults {
  namespace Player {
    const unsigned short Velocity = 3;
    const float InitialHealth = 10.f;
    const float DamagePoints = 2.f;
  }
}

ref class Player : public Entity {
  array<Keys>^ movementKeys;
  List<Keys>^ keysPressed;

public:
  Player(Point pos)
    : Entity(EntityType::Player, pos, Defaults::Player::Velocity, Defaults::Player::InitialHealth, Defaults::Player::DamagePoints) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\principal\\principal_m.png");
    this->SetImage(image, 4, 4);
    this->movementKeys = gcnew array<Keys> { Keys::A, Keys::D, Keys::S, Keys::W };
    this->keysPressed = gcnew List<Keys>;
  }

  Player(Bitmap^ image, short nCols, short nRows, Point pos)
    : Entity(EntityType::Player, pos, Defaults::Player::Velocity, Defaults::Player::InitialHealth, Defaults::Player::DamagePoints) {
    this->SetImage(image, nCols, nRows);
  }

  ~Player() {}

  void SetSpriteDirection(Direction direction) override {
    switch (direction) {
      case Direction::Up:
        SetRow(3);
        break;
      case Direction::Down:
        SetRow(0);
        break;
      case Direction::Left:
        SetRow(1);
        break;
      case Direction::Right:
        SetRow(2);
        break;
    }
  }

  // TO DO: Move this to UI controller
  void DrawHearts(Graphics^ g) {
    Point heartPos = Point(20, 15);
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    //Draw all the healthPoints of the player
    for (float i = 0.f; i < this->healthPoints; i++) {
      g->DrawImage(bmpManager->GetImage("assets\\sprites\\misc\\heart.png"), heartPos);
      //Add the X position of where the next heart will be drawn
      heartPos.X += 46;
    }
  }

  void KeyDown(KeyEventArgs^ e) {
    if (!IsValidKey(e->KeyCode))
      return;

    if (!keysPressed->Contains(e->KeyCode)) {
      keysPressed->Add(e->KeyCode);
      StartAnimation();
      // ResetPathfinders();
    }
  }

  void KeyUp(KeyEventArgs^ e) {
    if (keysPressed->Contains(e->KeyCode))
      keysPressed->Remove(e->KeyCode);

    if (keysPressed->Count == 0)
      StopAnimation();
  }

  void MoveUsingKeysList() {
    for each (Keys key in keysPressed) {
      if (!IsValidKey(key)) break;
      Move(key);
    }
  }

private:
  bool IsValidKey(Keys key) {
    // Array::BinarySeach needs a sorted array to work correctly
    // See more in the link below:
    // https://docs.microsoft.com/en-us/dotnet/api/system.array.binarysearch?view=netframework-4.8
    return !(Array::BinarySearch(movementKeys, key) < 0);
  }
};

#endif // !_PLAYER_H_

