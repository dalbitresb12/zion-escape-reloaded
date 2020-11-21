#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "Scene.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Map {
  List<Scene^>^ scenes;
  Random^ rnd;
  int seed;
  int maxScenes;
  bool isGenerating;
  bool generated;

public:
  Map(): Map(40, 50, Environment::TickCount) {}

  Map(int seed): Map(40, 50, seed) {}

  Map(int min, int max): Map(min, max, Environment::TickCount) {}

  Map(int min, int max, int seed) {
    this->seed = seed;
    this->rnd = gcnew Random(seed);
    this->maxScenes = rnd->Next(min, max);
    // POSSIBLE CAUSE OF DEBUGGER ERROR
    // this->Reboot();
  }

  ~Map() {
    for each (Scene ^ scene in this->scenes)
      delete scene;
    this->scenes->Clear();
    delete this->scenes;
    delete this->rnd;
  }

  void Reboot() {
    if (this->scenes != nullptr) {
      for each (Scene^ scene in this->scenes)
        delete scene;
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

  void CreateScene(bool up, bool down, bool left, bool right, Point pos) {
    this->scenes->Add(gcnew Scene(up, down, left, right, pos));
    this->scenes[this->scenes->Count - 1]->CreateSpawner(pos);
  }

  void StartGeneration(Graphics^ g) {
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
            Direction doorNeeded = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetParentDirection();
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
                if (doorNeeded != Direction::Up)
                  up = rnd->Next(0, 2);
                if (doorNeeded != Direction::Down)
                  down = rnd->Next(0, 2);
                if (doorNeeded != Direction::Right)
                  right = rnd->Next(0, 2);
                if (doorNeeded != Direction::Left)
                  left = rnd->Next(0, 2);
              } while (up == down == right == left == true);
            }
            //If number of scenes reaches the maxScenes, the current scene needs to be closed
            else {
              switch (doorNeeded)
              {
              case Direction::Up:
                down = right = left = false;
                break;
              case Direction::Down:
                up = right = left = false;
                break;
              case Direction::Right:
                up = down = left = false;
                break;
              case Direction::Left:
                up = down = right = false;
                break;
              }
            }
            //Create a new scene
            this->CreateScene(up, down, left, right, pos);
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
      //g->Clear(Color::FromArgb(255, 37, 37, 37));

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

  void CloseDoor(Direction doorNeeded, Scene^ scene) {
    switch (doorNeeded)
    {
    case Direction::Up:
      scene->SetDown(false);
      break;
    case Direction::Down:
      scene->SetUp(false);
      break;
    case Direction::Right:
      scene->SetLeft(false);
      break;
    case Direction::Left:
      scene->SetRight(false);
      break;
    }
  }

  void CloseDoorScene(Direction doorNeeded, Scene^ scene, Scene^ otherScene) {
    switch (doorNeeded)
    {
    case Direction::Up:
      if (!otherScene->GetUp())
        scene->SetDown(false);
      break;
    case Direction::Down:
      if (!otherScene->GetDown())
        scene->SetUp(false);
      break;
    case Direction::Right:
      if (!otherScene->GetRight())
        scene->SetLeft(false);
      break;
    case Direction::Left:
      if (!otherScene->GetLeft())
        scene->SetRight(false);
      break;
    }
  }

  bool IsGenerated() {
    return this->generated;
  }

  int GetSeed() {
    return this->seed;
  }
};

#endif // !_MAP_H_
