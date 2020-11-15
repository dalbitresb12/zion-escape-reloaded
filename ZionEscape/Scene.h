#pragma once
#include "SceneSpawner.h"
using namespace System::Drawing;
using namespace System::Collections::Generic;

enum class ImageId {
  D, DR, G, L, RL, R, T, TD, TL, TR, DL, DRL, TDL, TDR, TRL
};

ref class Scene {
  Bitmap^ image;
  bool top, down, right, left;
  ImageId id;
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;
public:
  Scene(bool top, bool down, bool right, bool left, ImageId id, Graphics^ g) {
    this->spawners = gcnew List<SceneSpawner^>;
    this->top = top;
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
    case ImageId::D:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\D.png");
      break;
    case ImageId::DR:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DR.png");
      break;
    case ImageId::G:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\G.png");
      break;
    case ImageId::L:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\L.png");
      break;
    case ImageId::RL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\RL.png");
      break;
    case ImageId::R:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\R.png");
      break;
    case ImageId::T:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\T.png");
      break;
    case ImageId::TD:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TD.png");
      break;
    case ImageId::TL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TL.png");
      break;
    case ImageId::TR:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TR.png");
      break;
    case ImageId::DL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DL.png");
      break;
    case ImageId::DRL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DRL.png");
      break;
    case ImageId::TDL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDL.png");
      break;
    case ImageId::TDR:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDR.png");
      break;
    case ImageId::TRL:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TRL.png");
      break;
    }
  }
  void Draw(Graphics^ g, Point pos) {
    this->drawingArea = Rectangle(pos.X, pos.Y, this->image->Width, this->image->Height);
    g->DrawImage(this->image, this->drawingArea);
  }
  void CreateSpawner(Graphics^ g, Point pos) {
    if (this->top == true) {
      short x = pos.X;
      short y = pos.Y - this->image->Height;
      this->spawners->Add(gcnew SceneSpawner(OpDir::Down, g, Rectangle(x, y, this->image->Width, this->image->Height)));
    }
    if (this->down == true) {
      short x = pos.X;
      short y = pos.Y + this->image->Height;
      this->spawners->Add(gcnew SceneSpawner(OpDir::Up, g, Rectangle(x, y, this->image->Width, this->image->Height)));
    }
    if (this->right == true) {
      short x = pos.X + this->image->Width;
      short y = pos.Y;
      this->spawners->Add(gcnew SceneSpawner(OpDir::Left, g, Rectangle(x, y, this->image->Width, this->image->Height)));
    }
    if (this->left == true) {
      short x = pos.X - this->image->Width;
      short y = pos.Y;
      this->spawners->Add(gcnew SceneSpawner(OpDir::Right, g, Rectangle(x, y, this->image->Width, this->image->Height)));
    }
  }
  void DeleteSpawner(short n) {
    this->spawners->Remove(this->spawners[n]);
  }
  bool Collides(Rectangle area) {
    return this->drawingArea.IntersectsWith(area);
  }
  ImageId GetID() {
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
