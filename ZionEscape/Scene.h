#pragma once

#include "SceneSpawner.h"

using namespace System::Drawing;
using namespace System::Collections::Generic;

enum class ImageDirection {
  Down, DownRight, All, Left, RightLeft, Right, Up, UpDown, UpLeft,
  UpRight, DownLeft, DownRightLeft, UpDownLeft, UpDownRight, UpRightLeft
};

ref class Scene {
  Bitmap^ image;
  bool up, down, right, left;
  ImageDirection id;
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;
public:
  Scene(bool up, bool down, bool right, bool left, ImageDirection id) {
    this->spawners = gcnew List<SceneSpawner^>;
    this->up = up;
    this->down = down;
    this->right = right;
    this->left = left;
    this->id = id;
    this->ImageSelector();
  }

  ~Scene() {
    for (short i = 0; i < this->spawners->Count; i++)
      delete this->spawners[i];
    this->spawners->Clear();
    delete this->spawners;
  }

  void ImageSelector() {
    switch (this->id)
    {
    case ImageDirection::Down:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\D.png");
      break;
    case ImageDirection::DownRight:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DR.png");
      break;
    case ImageDirection::All:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\G.png");
      break;
    case ImageDirection::Left:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\L.png");
      break;
    case ImageDirection::RightLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\RL.png");
      break;
    case ImageDirection::Right:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\R.png");
      break;
    case ImageDirection::Up:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\T.png");
      break;
    case ImageDirection::UpDown:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TD.png");
      break;
    case ImageDirection::UpLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TL.png");
      break;
    case ImageDirection::UpRight:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TR.png");
      break;
    case ImageDirection::DownLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DL.png");
      break;
    case ImageDirection::DownRightLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DRL.png");
      break;
    case ImageDirection::UpDownLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDL.png");
      break;
    case ImageDirection::UpDownRight:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDR.png");
      break;
    case ImageDirection::UpRightLeft:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TRL.png");
      break;
    }
  }

  void Draw(Graphics^ g, Point pos) {
    this->drawingArea = Rectangle(pos.X, pos.Y, this->image->Width, this->image->Height);
    g->DrawImage(this->image, this->drawingArea);
  }

  void CreateSpawner(Graphics^ g, Point pos) {
    Point location;
    Size size = Size(this->image->Width, this->image->Height);
    if (this->up) {
      location = Point(pos.X, pos.Y - size.Height);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Down, g, Rectangle(location, size)));
    }
    if (this->down) {
      location = Point(pos.X, pos.Y + size.Height);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Up, g, Rectangle(location, size)));
    }
    if (this->right) {
      location = Point(pos.X, pos.Y + size.Width);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Left, g, Rectangle(location, size)));
    }
    if (this->left) {
      location = Point(pos.X, pos.Y - size.Width);
      this->spawners->Add(gcnew SceneSpawner(ParentDirection::Right, g, Rectangle(location, size)));
    }
  }

  void DeleteSpawner(short n) {
    this->spawners->Remove(this->spawners[n]);
  }

  bool Collides(Rectangle area) {
    return this->drawingArea.IntersectsWith(area);
  }

  ImageDirection GetImageDirection() {
    return this->id;
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
