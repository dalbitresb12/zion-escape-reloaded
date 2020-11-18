#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneSpawner.h"
#include "BitmapManager.h"

using namespace System::Drawing;
using namespace System::Collections::Generic;

ref class Scene {
  Bitmap^ background;
  BitmapManager^ bmpManager;
  bool up, down, right, left;
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;
public:
  Scene(bool up, bool down, bool right, bool left, Point pos) {
    this->bmpManager = BitmapManager::GetInstance();
    this->spawners = gcnew List<SceneSpawner^>;
    this->up = up;
    this->down = down;
    this->right = right;
    this->left = left;
    this->ImageSelector();
    this->drawingArea = Rectangle(pos, this->background->Size);
  }

  ~Scene() {
    for each (SceneSpawner ^ spawner in this->spawners)
      delete spawner;
    this->spawners->Clear();
    delete this->spawners;
  }

  //Temporal Image Selector -> Works as  a reference
  void ImageSelector() {
    if (!this->up && this->down && !this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\D.png");

    else if (!this->up && this->down && this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\DR.png");

    else if (this->up && this->down && this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\G.png");

    else if (!this->up && !this->down && !this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\L.png");

    else if (!this->up && !this->down && this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\RL.png");

    else if (!this->up && !this->down && this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\R.png");

    else if (this->up && !this->down && !this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\T.png");

    else if (this->up && this->down && !this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TD.png");

    else if (this->up && !this->down && !this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TL.png");

    else if (this->up && !this->down && this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TR.png");

    else if (!this->up && this->down && !this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\DL.png");

    else if (!this->up && this->down && this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\DRL.png");

    else if (this->up && this->down && !this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TDL.png");

    else if (this->up && this->down && this->right && !this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TDR.png");

    else if (this->up && !this->down && this->right && this->left)
      this->background = this->bmpManager->GetImage("assets\\sprites\\colliders\\TRL.png");
  }

  void Draw(Graphics^ g) {
    g->DrawImage(this->background, this->drawingArea);
  }

  void CreateSpawner(Point pos) {
    Point location;
    Size size = Size(this->background->Width, this->background->Height);
    if (this->up) {
      location = Point(pos.X, pos.Y - size.Height);
      this->spawners->Add(gcnew SceneSpawner(Direction::Down, Rectangle(location, size)));
    }
    if (this->down) {
      location = Point(pos.X, pos.Y + size.Height);
      this->spawners->Add(gcnew SceneSpawner(Direction::Up, Rectangle(location, size)));
    }
    if (this->right) {
      location = Point(pos.X + size.Width, pos.Y);
      this->spawners->Add(gcnew SceneSpawner(Direction::Left, Rectangle(location, size)));
    }
    if (this->left) {
      location = Point(pos.X - size.Width, pos.Y);
      this->spawners->Add(gcnew SceneSpawner(Direction::Right, Rectangle(location, size)));
    }
  }

  void DeleteSpawner(short n) {
    this->spawners->Remove(this->spawners[n]);
  }

  void SetUp(bool value) {
    this->up = value;
    this->ImageSelector();
  }

  void SetDown(bool value) {
    this->down = value;
    this->ImageSelector();
  }

  void SetRight(bool value) {
    this->right = value;
    this->ImageSelector();
  }

  void SetLeft(bool value) {
    this->left = value;
    this->ImageSelector();
  }

  bool GetUp() {
    return this->up;
  }

  bool GetDown() {
    return this->down;
  }

  bool GetRight() {
    return this->right;
  }

  bool GetLeft() {
    return this->left;
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
};

#endif // !_SCENE_H_


