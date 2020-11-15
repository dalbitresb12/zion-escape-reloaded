#pragma once
#include "Scene.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Map {
  List<Scene^>^ scenes;
  Random^ rnd;
  short maxScenes;
  bool isGenerating;
  bool generated;

public:
  Map() {
    this->rnd = gcnew Random();
    this->maxScenes = rnd->Next(40, 50);
    this->Reboot();
  }

  ~Map() {
    for (short i = 0; i < this->scenes->Count; i++)
      delete this->scenes[i];
    this->scenes->Clear();
    delete this->scenes;
    delete this->rnd;
  }

  void Reboot() {
    if (this->scenes != nullptr) {
      for (short i = 0; i < this->scenes->Count; i++)
        delete this->scenes[i];
      this->scenes->Clear();
      delete this->scenes;
    }

    //Turn the values of generation to false
    this->isGenerating = false;
    this->generated = false;

    //Create the list of scenes and select the start point of the first one
    this->scenes = gcnew List<Scene^>;
    this->CreateScene(1, 1, 1, 1, Point(468, 312));
  }

  void CreateScene(bool t, bool d, bool r, bool l, Point pos) {
    this->scenes->Add(gcnew Scene(t, d, r, l, pos));
    this->scenes[this->scenes->Count - 1]->CreateSpawner(pos);
  }

  void MapGeneration(Graphics^ g) {
    //Generate new Scenes
    if (this->isGenerating) {

      //Check all the scenes
      for (short curScene = 0; curScene < this->scenes->Count; curScene++) {
        //Number of spawners in the current Scene
        short countSpawners = this->scenes[curScene]->GetSpawners()->Count;

        if(countSpawners > 0) {

          this->isGenerating = true;

          //For each Spawner of the Scene we need to know if they collides or not
          for (short curSpawner = countSpawners; curSpawner > 0; curSpawner--) {
            //Get the door direction that the next scene will need
            ParentDirection doorNeeded = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetParentDirection();
            bool spawnerCollides = false;

            //Check all scenes
            for each (Scene ^ otherScene in this->scenes) {
              //Check all ScenesSpawners of the otherScene
              for each (SceneSpawner ^ otherSpawner in otherScene->GetSpawners()) {
                if (this->scenes[curScene]->GetSpawners()[curSpawner - 1] != otherSpawner) {
                  //If the curSpawner is in the same position of otherSpawner, a door of the curScene will be closed
                  if (this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos().Equals(otherSpawner->GetPos())) {
                    spawnerCollides = true;
                    this->CloseDoor(doorNeeded, this->scenes[curScene]);
                    break;
                  }
                }
              }
              if (spawnerCollides)
                break;
              //If the curSpawner is in the same position of otherScene, a door of the curScene will be closed
              if (this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos().Equals(otherScene->GetPos())) {
                spawnerCollides = true;
                this->CloseDoorScene(doorNeeded, this->scenes[curScene], otherScene);
                break;
              }
            }

            if (spawnerCollides) {
              this->scenes[curScene]->DeleteSpawner(curSpawner - 1);
              break;
            }

            //Values of the new scene
            bool up = true, down = true, right = true, left = true;
            Point pos = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos();
            
            if (this->scenes->Count < this->maxScenes) {
              //Get a random open or close door
              do {
                if (doorNeeded != ParentDirection::Up)
                  up = rnd->Next(0, 2);
                if (doorNeeded != ParentDirection::Down)
                  down = rnd->Next(0, 2);
                if (doorNeeded != ParentDirection::Right)
                  right = rnd->Next(0, 2);
                if (doorNeeded != ParentDirection::Left)
                  left = rnd->Next(0, 2);
              } while (up == down == right == left == true);
            }
            //If number of scenes reaches the maxScenes, the current scene needs to be closed
            else {
              switch (doorNeeded)
              {
              case ParentDirection::Up:
                down = right = left = false;
                break;
              case ParentDirection::Down:
                up = right = left = false;
                break;
              case ParentDirection::Right:
                up = down = left = false;
                break;
              case ParentDirection::Left:
                up = down = right = false;
                break;
              }
            }
            //Create a new scene
            this->CreateScene(up, down, right, left, pos);
            //After the creation of the scene, delete the curSpawner
            this->scenes[curScene]->DeleteSpawner(curSpawner - 1);
          }
        }
        else {
          this->isGenerating = false;
        }

      }
    }
    else if (this->scenes->Count == 1) {
      this->isGenerating = true;
    }
    else if (this->scenes->Count < this->maxScenes && this->isGenerating == false) {
      this->Reboot();
    } else {
      //The Map is now generated
      this->generated = true;

      //Put a color to the background
      g->Clear(Color::FromArgb(255, 37, 37, 37));

      //Draw all the scenes
      for each (Scene ^ curScene in this->scenes) {
        //Put a color to the maze
        g->FillRectangle(Brushes::CornflowerBlue, curScene->GetDrawingArea());

        //Put a different color to the background of the first and last scene
        if (curScene == this->scenes[0])
          g->FillRectangle(Brushes::Crimson, curScene->GetDrawingArea());
        else if (curScene == this->scenes[this->scenes->Count - 1])
          g->FillRectangle(Brushes::BlueViolet, curScene->GetDrawingArea());

        // Draw the scene
        curScene->Draw(g);
      }
    }
  }

  void CloseDoor(ParentDirection doorNeeded, Scene^ scene) {
    switch (doorNeeded)
    {
    case ParentDirection::Up:
      scene->SetDown(false);
      break;
    case ParentDirection::Down:
      scene->SetUp(false);
      break;
    case ParentDirection::Right:
      scene->SetLeft(false);
      break;
    case ParentDirection::Left:
      scene->SetRight(false);
      break;
    }
  }
  void CloseDoorScene(ParentDirection doorNeeded, Scene^ scene, Scene^ otherScene) {
    switch (doorNeeded)
    {
    case ParentDirection::Up:
      if (!otherScene->GetUp())
        scene->SetDown(false);
      break;
    case ParentDirection::Down:
      if (!otherScene->GetDown())
        scene->SetUp(false);
      break;
    case ParentDirection::Right:
      if (!otherScene->GetRight())
        scene->SetLeft(false);
      break;
    case ParentDirection::Left:
      if (!otherScene->GetLeft())
        scene->SetRight(false);
      break;
    }
  }
  bool GetGenerated() {
    return this->generated;
  }
};
