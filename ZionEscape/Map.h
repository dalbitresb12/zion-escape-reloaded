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
    this->CreateScene(1, 1, 1, 1, ImageId::G, g, Point(468, 312));
  }

  void CreateScene(bool t, bool d, bool r, bool l, ImageId id, Graphics^ g, Point pos) {
    this->scenes->Add(gcnew Scene(t, d, r, l, id));
    this->scenes[this->scenes->Count - 1]->Draw(g, pos);
    this->scenes[this->scenes->Count - 1]->CreateSpawner(g, pos);
  }

  void MapGeneration(Graphics^ g) {
    //Check if any of the scenes don't collide with any scene
    /*for (short curScene = 0; curScene < this->scenes->Count; curScene++) {
      for (short adScene = 0; adScene < this->scenes->Count; adScene++) {
        if (curScene != adScene)
          if (this->scenes[curScene]->Collides(this->scenes[adScene]->GetDrawingArea())) {
            this->Reboot(g);
          }
      }
    }*/
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
          OpDir doorNeeded = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetOpDir();
          //Get position where the Scene will be created
          Point pos = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos();

          bool shouldBreak = false;
          for each (Scene ^ scene in this->scenes) {
            for each (SceneSpawner ^ spawner in scene->GetSpawners()) {
              if (this->scenes[curScene]->GetSpawners()[curSpawner - 1]->Equals(spawner))
                continue;

              if (spawner->GetPos().Equals(pos)) {
                shouldBreak = true;
                break;
              }
            }

            if (shouldBreak)
              break;

            if (scene->GetPos().Equals(pos)) {
              shouldBreak = true;
              break;
            }
          }

          if (shouldBreak) {
            this->scenes[curScene]->DeleteSpawner(curSpawner - 1);
            break;
          }

          //Get Values to Create the new Scene

          bool up = false, down = false, left = false, right = false;

          if (doorNeeded != OpDir::Up)
            up = rnd->Next(0, 2);
          if (doorNeeded != OpDir::Down)
            down = rnd->Next(0, 2);
          if (doorNeeded != OpDir::Left)
            left = rnd->Next(0, 2);
          if (doorNeeded != OpDir::Right)
            right = rnd->Next(0, 2);

          ImageId selectedId = this->GetImageIdFromValues(up, down, right, left, doorNeeded);

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

  ImageId GetImageIdFromValues(bool top, bool down, bool right, bool left, OpDir dorNeeded) {
    if (top && down && right && left)
      return ImageId::G;
    else if (!top && down && right && left)
      return ImageId::DRL;
    else if (top && !down && right && left)
      return ImageId::TRL;
    else if (top && down && !right && left)
      return ImageId::TDL;
    else if (top && down && right && !left)
      return ImageId::TDR;
    else if (!top && !down && right && left)
      return ImageId::RL;
    else if (!top && down && !right && left)
      return ImageId::DL;
    else if (!top && down && right && !left)
      return ImageId::DR;
    else if (!top && !down && right && left)
      return ImageId::RL;
    else if (top && down && !right && !left)
      return ImageId::TD;
    else if (top && !down && !right && left)
      return ImageId::TL;
    else if (top && !down && right && !left)
      return ImageId::TR;
    else if (top && !down && !right && !left)
      return ImageId::T;
    else if (!top && down && !right && !left)
      return ImageId::D;
    else if (!top && !down && right && !left)
      return ImageId::R;
    else if (!top && !down && !right && left)
      return ImageId::L;
    else if (!top && !down && !right && !left)
      switch (dorNeeded) {
      case OpDir::Down:
        return ImageId::D;
        break;
      case OpDir::Up:
        return ImageId::T;
        break;
      case OpDir::Left:
        return ImageId::L;
        break;
      case OpDir::Right:
        return ImageId::R;
        break;
      }

  }

  bool GetGenerated() {
    return this->generated;
  }
};
