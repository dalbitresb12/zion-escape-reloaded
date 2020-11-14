#pragma once
#include <list>
#include "Scene.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Map {
  List<Scene^>^ scenes;
  Random^ rnd = gcnew Random();
  short counter;
  bool isGenerating;
  bool generated;
public:
  Map(Graphics^ g) {
    this->Reboot(g);
  }
  ~Map() {
    for (short i = 0; i < this->counter; i++)
      delete this->scenes[i];
    this->scenes->Clear();
    delete this->scenes;
    delete rnd;
  }
  void Reboot(Graphics^ g) {
    if (this->counter > 0) {
      for (short i = 0; i < this->counter; i++)
        delete this->scenes[i];
      this->scenes->Clear();
      delete this->scenes;
    }
    //Select Background color
    g->Clear(Color::FromArgb(255, 37, 37, 37));
    this->isGenerating = false;
    //Draw and select Start point color
    g->FillRectangle(Brushes::Crimson, Rectangle(468, 312, 27, 27));
    this->scenes = gcnew List<Scene^>;
    this->counter = this->scenes->Count;
    this->CreateScene(1, 1, 1, 1, 3, g, Point(468, 312));
  }
  void CreateScene(bool t, bool d, bool r, bool l, short id, Graphics^ g, Point pos) {
    this->scenes->Add(gcnew Scene(t, d, r, l, id, g));
    this->counter++;
    this->scenes[this->counter - 1]->Draw(g, pos);
    this->scenes[this->counter - 1]->CreateSpawner(g, pos);
  }
  void MapGeneration(Graphics^ g) {
    //Check if any of the scenes doesn't collide with any scene
    for (short curScene = 0; curScene < this->counter; curScene++) {
      for (short adScene = 0; adScene < this->counter; adScene++) {
        if (curScene != adScene)
          if (this->scenes[curScene]->Collides(this->scenes[adScene]->GetDrawingArea())) {
            this->Reboot(g);
          }
      }
    }
    //Generate new Scenes
    if (isGenerating == true) {
      this->isGenerating = false;
      //Current scenes to count
      short totalScenes = this->counter;
      //We will check all the scenes	
      for (short curScene = 0; curScene < totalScenes; curScene++) {
        //How many spawners have the current Scene
        short countSpawners = this->scenes[curScene]->GetSpawners()->Count;
        //Know if there are enough countSpawners
        if (countSpawners > 0) {
          //For each Spawner of the Scene we need to know if they collides or not
          for (short curSpawner = countSpawners; curSpawner > 0; curSpawner--) {
            for (short k = 0; k < totalScenes; k++) {
              //Generates a scene, If the Spawner doesn't collide with other scene
              this->isGenerating = true;
              //Get the Scene to create
              short doorNeeded = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetOpDir();
              //Makes a Random number to a random Scene
              short id = 0;
              //Generates a List of ID
              List<short>^ ids = gcnew List<short>();
              //Get position of where the Scene will be created
              Point pos = this->scenes[curScene]->GetSpawners()[curSpawner - 1]->GetPos();
              //Get Values to Create the new Scene
              switch (doorNeeded)
              {
                //TOP
              case 1: {
                ids->Add(7); ids->Add(8); ids->Add(9); ids->Add(10);
                ids->Add(8); ids->Add(13); ids->Add(14); ids->Add(15);
                //Select a Random
                id = ids[rnd->Next(0, 7)];
                if (id == 7) {
                  this->CreateScene(0, 0, 0, 0, id, g, pos);
                }
                else if (id == 8) {
                  this->CreateScene(0, 1, 0, 0, id, g, pos);
                }
                else if (id == 9) {
                  this->CreateScene(0, 0, 0, 1, id, g, pos);
                }
                else if (id == 10) {
                  this->CreateScene(0, 0, 1, 0, id, g, pos);
                }
                else if (id == 13) {
                  this->CreateScene(0, 1, 0, 1, id, g, pos);
                }
                else if (id == 14) {
                  this->CreateScene(0, 1, 1, 0, id, g, pos);
                }
                else {
                  this->CreateScene(0, 0, 1, 1, id, g, pos);
                }
                break;
              }
                    //DOWN
              case 2: {
                ids->Add(1); ids->Add(2); ids->Add(11);  ids->Add(8);
                ids->Add(8); ids->Add(13); ids->Add(14); ids->Add(12);
                //Select a Random
                id = ids[rnd->Next(0, 7)];
                if (id == 1) {
                  this->CreateScene(0, 0, 0, 0, id, g, pos);
                }
                else if (id == 2) {
                  this->CreateScene(0, 0, 1, 0, id, g, pos);
                }
                else if (id == 11) {
                  this->CreateScene(0, 0, 0, 1, id, g, pos);
                }
                else if (id == 8) {
                  this->CreateScene(1, 0, 0, 0, id, g, pos);
                }
                else if (id == 13) {
                  this->CreateScene(1, 0, 0, 1, id, g, pos);
                }
                else if (id == 14) {
                  this->CreateScene(1, 0, 1, 0, id, g, pos);
                }
                else {
                  this->CreateScene(0, 0, 1, 1, id, g, pos);
                }
                break;
              }
                    //RIGHT
              case 3: {
                ids->Add(2); ids->Add(5); ids->Add(6); ids->Add(10);
                //Select a Random
                id = ids[rnd->Next(0, 3)];
                if (id == 2) {
                  this->CreateScene(0, 1, 0, 0, id, g, pos);
                }
                else if (id == 5) {
                  this->CreateScene(0, 0, 0, 1, id, g, pos);
                }
                else if (id == 6) {
                  this->CreateScene(0, 0, 0, 0, id, g, pos);
                }
                else {
                  this->CreateScene(1, 0, 0, 0, id, g, pos);
                }
                break;
              }
                    //LEFT
              case 4: {
                ids->Add(4); ids->Add(5); ids->Add(9); ids->Add(11);
                //Select a Random
                id = ids[rnd->Next(0, 3)];
                if (id == 4) {
                  this->CreateScene(0, 0, 0, 0, id, g, pos);
                }
                else if (id == 5) {
                  this->CreateScene(0, 0, 1, 0, id, g, pos);
                }
                else if (id == 11) {
                  this->CreateScene(0, 1, 0, 0, id, g, pos);
                }
                else {
                  this->CreateScene(1, 0, 0, 0, id, g, pos);
                }
                break;
              }
              }
              //Delete ids and rnd
              ids->Clear();
              delete ids;
              //Then the Spawner has to be deleted
              this->scenes[curScene]->DeleteSpawner(curSpawner - 1);
              break;
            }
          }
        }
      }
    }
    else if (this->counter == 1) {
      this->isGenerating = true;
    }
    else if (this->counter > 1 && this->counter < 25 && this->isGenerating == false) {
      this->Reboot(g);
    }
    else {
      if (this->counter != 1) {
        this->generated = true;
        //Paint the last scene
        g->FillRectangle(Brushes::BlueViolet, this->scenes[counter - 1]->GetDrawingArea());
        this->scenes[counter - 1]->Draw(g, this->scenes[counter - 1]->GetPos());
      }
    }
  }
  bool GetGenerated() {
    return this->generated;
  }
};
