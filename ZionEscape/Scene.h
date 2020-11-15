#pragma once

#include "SceneSpawner.h"

using namespace System::Drawing;
using namespace System::Collections::Generic;

ref class Scene {
  Bitmap^ image;
  bool up, down, right, left;
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;
public:
  Scene(bool up, bool down, bool right, bool left, Point pos) {
    this->spawners = gcnew List<SceneSpawner^>;
    this->up = up;
    this->down = down;
    this->right = right;
    this->left = left;
    this->ImageSelector();
    this->drawingArea = Rectangle(pos.X, pos.Y, this->image->Width, this->image->Height);
  }

  ~Scene() {
    for (short i = 0; i < this->spawners->Count; i++)
      delete this->spawners[i];
    this->spawners->Clear();
    delete this->spawners;
  }

  //Temporal Image Selector -> Works as  a reference
  void ImageSelector() {
      if (!this->up && this->down && !this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\D.png");

      else if (!this->up && this->down && this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\DR.png");

      else if (this->up && this->down && this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\G.png");

      else if (!this->up && !this->down && !this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\L.png");

      else if (!this->up && !this->down && this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\RL.png");

      else if (!this->up && !this->down && this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\R.png");

      else if (this->up && !this->down && !this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\T.png");

      else if (this->up && this->down && !this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TD.png");

      else if (this->up && !this->down && !this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TL.png");

      else if (this->up && !this->down && this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TR.png");

      else if (!this->up && this->down && !this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\DL.png");

      else if (!this->up && this->down && this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\DRL.png");

      else if (this->up && this->down && !this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDL.png");

      else if (this->up && this->down && this->right && !this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDR.png");

      else if (this->up && !this->down && this->right && this->left)
        this->image = gcnew Bitmap("assets\\sprites\\colliders\\TRL.png");
  }

  void Draw(Graphics^ g) {
    g->DrawImage(this->image, this->drawingArea);
  }

  void CreateSpawner(Point pos) {
    Point location;
    Size size = Size(this->image->Width, this->image->Height);
    if (this->up) {
      location = Point(pos.X, pos.Y - size.Height);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Down, Rectangle(location, size)));
    }
    if (this->down) {
      location = Point(pos.X, pos.Y + size.Height);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Up, Rectangle(location, size)));
    }
    if (this->right) {
      location = Point(pos.X + size.Width, pos.Y);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Left, Rectangle(location, size)));
    }
    if (this->left) {
      location = Point(pos.X - size.Width, pos.Y);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Right, Rectangle(location, size)));
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
