#pragma once
#include "SceneSpawner.h"
using namespace System::Drawing;
using namespace System::Collections::Generic;
ref class Scene {
  Bitmap^ image;
  bool top, down, right, left;
  short id;
  List<SceneSpawner^>^ spawners;
  Rectangle drawingArea;
public:
  Scene(bool top, bool down, bool right, bool left, short id, Graphics^ g) {
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
    case 1:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\D.png");
      break;
    case 2:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DR.png");
      break;
    case 3:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\G.png");
      break;
    case 4:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\L.png");
      break;
    case 5:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\RL.png");
      break;
    case 6:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\R.png");
      break;
    case 7:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\T.png");
      break;
    case 8:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TD.png");
      break;
    case 9:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TL.png");
      break;
    case 10:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TR.png");
      break;
    case 11:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DL.png");
      break;
    case 12:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\DRL.png");
      break;
    case 13:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDL.png");
      break;
    case 14:
      this->image = gcnew Bitmap("assets\\sprites\\colliders\\TDR.png");
      break;
    case 15:
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
      this->spawners->Add(gcnew SceneSpawner(2, g, Rectangle(x, y, this->image->Width, this->image->Height)));
      this->top = false;
    }
    if (this->down == true) {
      short x = pos.X;
      short y = pos.Y + this->image->Height;
      this->spawners->Add(gcnew SceneSpawner(1, g, Rectangle(x, y, this->image->Width, this->image->Height)));
      this->down = false;
    }
    if (this->right == true) {
      short x = pos.X + this->image->Width;
      short y = pos.Y;
      this->spawners->Add(gcnew SceneSpawner(4, g, Rectangle(x, y, this->image->Width, this->image->Height)));
      this->right = false;
    }
    if (this->left == true) {
      short x = pos.X - this->image->Width;
      short y = pos.Y;
      this->spawners->Add(gcnew SceneSpawner(3, g, Rectangle(x, y, this->image->Width, this->image->Height)));
      this->left = false;
    }
  }
  void DeleteSpawner(short n) {
    this->spawners->Remove(this->spawners[n]);
  }
  bool Collides(Rectangle area) {
    return this->drawingArea.IntersectsWith(area);
  }
  short GetID() {
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
