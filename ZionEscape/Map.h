#pragma once
#include "Scene.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Map {
  List<Scene^>^ scenes;
  Random^ rnd;
  bool isGenerating;
  bool generated;

public:
  Map(Graphics^ g) {
    this->rnd = gcnew Random();
    this->Reboot(g);
  }

  ~Map() {
    for (short i = 0; i < this->scenes->Count; i++)
      delete this->scenes[i];
    this->scenes->Clear();
    delete this->scenes;
    delete this->rnd;
  }

  void Reboot(Graphics^ g) {
    if (this->scenes != nullptr) {
      for (short i = 0; i < this->scenes->Count; i++)
        delete this->scenes[i];
      this->scenes->Clear();
      delete this->scenes;
    }

    //Select Background color
    g->Clear(Color::FromArgb(255, 37, 37, 37));
    this->isGenerating = false;
    this->generated = false;

    //Draw and select Start point color
    g->FillRectangle(Brushes::Crimson, Rectangle(468, 312, 27, 27));
    this->scenes = gcnew List<Scene^>;
    this->CreateScene(1, 1, 1, 1, ImageDirection::All, g, Point(468, 312));
  }

  void CreateScene(bool t, bool d, bool r, bool l, ImageDirection id, Graphics^ g, Point pos) {
    this->scenes->Add(gcnew Scene(t, d, r, l, id));
    this->scenes[this->scenes->Count - 1]->Draw(g, pos);
    this->scenes[this->scenes->Count - 1]->CreateSpawner(g, pos);
  }

  void MapGeneration(Graphics^ g) {
    //Generate new Scenes
    if (isGenerating) {
      //Save the current scenes to count
      short totalScenes = this->scenes->Count;

      //Check all the scenes
      for (short curScene = 0; curScene < totalScenes; curScene++) {
        //Number of spawners in the current Scene
        short countSpawners = this->scenes[curScene]->GetSpawners()->Count;

        for (short curSpawner = countSpawners; curSpawner > 0; curSpawner--) {
          //Get the Scene to create
          ParentDirection doorNeeded = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetParentDirection();
          //Get position where the Scene will be created
          Point pos = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos();

          bool up = false, down = false, left = false, right = false;

          if (doorNeeded != ParentDirection::Up)
            up = rnd->Next(0, 2);
          if (doorNeeded != ParentDirection::Down)
            down = rnd->Next(0, 2);
          if (doorNeeded != ParentDirection::Left)
            left = rnd->Next(0, 2);
          if (doorNeeded != ParentDirection::Right)
            right = rnd->Next(0, 2);

          ImageDirection selectedId = this->GetImageIdFromValues(up, down, right, left, doorNeeded);

          this->CreateScene(up, down, right, left, selectedId, g, pos);

          //Then the Spawner has to be deleted
          this->scenes[curScene]->DeleteSpawner(curSpawner - 1);
        }
      }
    } else if (this->scenes->Count == 1) {
      this->isGenerating = true;
    } else if (this->scenes->Count > 1 && this->scenes->Count < 25 && this->isGenerating == false) {
      this->Reboot(g);
    } else {
      if (this->scenes->Count != 1) {
        this->generated = true;
        //Paint the last scene
        g->FillRectangle(Brushes::BlueViolet, this->scenes[scenes->Count - 1]->GetDrawingArea());
        this->scenes[scenes->Count - 1]->Draw(g, this->scenes[scenes->Count - 1]->GetPos());
      }
    }
  }

  ImageDirection GetImageIdFromValues(bool top, bool down, bool right, bool left, ParentDirection dorNeeded) {
    if (top && down && right && left)
      return ImageDirection::All;
    else if (!top && down && right && left)
      return ImageDirection::DownRightLeft;
    else if (top && !down && right && left)
      return ImageDirection::UpRightLeft;
    else if (top && down && !right && left)
      return ImageDirection::UpDownLeft;
    else if (top && down && right && !left)
      return ImageDirection::UpDownRight;
    else if (!top && !down && right && left)
      return ImageDirection::RightLeft;
    else if (!top && down && !right && left)
      return ImageDirection::DownLeft;
    else if (!top && down && right && !left)
      return ImageDirection::DownRight;
    else if (!top && !down && right && left)
      return ImageDirection::RightLeft;
    else if (top && down && !right && !left)
      return ImageDirection::UpDown;
    else if (top && !down && !right && left)
      return ImageDirection::UpLeft;
    else if (top && !down && right && !left)
      return ImageDirection::UpRight;
    else if (top && !down && !right && !left)
      return ImageDirection::Up;
    else if (!top && down && !right && !left)
      return ImageDirection::Down;
    else if (!top && !down && right && !left)
      return ImageDirection::Right;
    else if (!top && !down && !right && left)
      return ImageDirection::Left;
    else if (!top && !down && !right && !left)
      switch (dorNeeded) {
      case ParentDirection::Down:
        return ImageDirection::Down;
        break;
      case ParentDirection::Up:
        return ImageDirection::Up;
        break;
      case ParentDirection::Left:
        return ImageDirection::Left;
        break;
      case ParentDirection::Right:
        return ImageDirection::Right;
        break;
      }

  }

  bool GetGenerated() {
    return this->generated;
  }
};
