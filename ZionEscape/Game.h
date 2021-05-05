#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "UI.h"
#include "Map.h"
#include "Grid.h"
#include "Pathfinder.h"
#include "NPC.h"
#include "Ally.h"
#include "Assassin.h"
#include "Corrupt.h"
#include "Player.h"
#include "Enums.h"
#include "Scene.h"
#include "BitmapManager.h"
#include "DataTypes.h"
#include "MessageBox.h"
#include "FileManager.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Diagnostics;

ref class Game {
  Map^ map;
  Grid^ mapGrid;
  Player^ player;
  List<Ally^>^ allies;
  Messagebox^ messagebox;
  bool initialized;

public:
  Game() {
    map = gcnew Map();
    initialized = false;
  }

  Game(int seed) {
    map = gcnew Map(seed);
    initialized = false;
  }

  void Init(Size ClientSize) {
    player = gcnew Player(Point(ClientSize.Width / 2, ClientSize.Height / 2));
    InitAllies();
    InitGrid(ClientSize);
    initialized = true;
    ResetPathfinders();
    SaveMapSeed();
  }

  bool HasInitialized() {
    return initialized;
  }
  ~Game() {
    delete map;
    delete messagebox;
  }

  bool IsGenerated() {
    return this->map->IsGenerated();
  }

  int GetMapSeed() {
    return this->map->GetSeed();
  }

  void SaveMapSeed() {
    FileManager::SaveSeed(map->GetSeed());
  }

  void DrawMapGizmos(Graphics^ world) {
    if (this->map->IsGenerated())
      this->map->DrawGizmos(world);
  }

  void MapGeneration() {
    if (!map->IsGenerating() && !map->IsGenerated()) {
      Action^ action = gcnew Action(map, &Map::StartGeneration);
      Task^ generator = Task::Factory->StartNew(action, TaskCreationOptions::LongRunning);
    }
  }

  void Paint(Graphics^ world, Rectangle ClientRectangle) {
    map->DrawCurrent(world);

    if (map != nullptr) {
      DrawMapGizmos(world);
    }

    if (allies != nullptr) {
      for each (Ally ^ ally in allies) {
        ally->Draw(world);

        //If the health of the NPC is 0, it's dead
        if (ally->GetHealth() <= 0) {
          //Delete form the list
          allies->Remove(ally);
          //The for must be break, beacuse it won't consider the same npc
          break;
        }
      }
    }

    if (player != nullptr) {
      // Add the player to the scene
      player->Draw(world);
      if (map->GetCurrentScene()->GetNPCList() != nullptr)
        player->ActionBullets(world, ClientRectangle, map->GetCurrentScene()->GetNPCList());
      else
        player->ActionBullets(world, ClientRectangle);

      // Draw the player health using the UI controller
      UI::DrawHearts(world, player->GetHealth());
    }

    if (messagebox != nullptr && messagebox->IsActivated) {
      messagebox->Draw(world);
    }
  }

  void KeyDown(KeyEventArgs^ e) {
    // If the message box has appeared, we stop this event
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    // Temporary map seed viewer
    if (e->KeyCode == Keys::P) {
      Debug::WriteLine("Seed: {0}", GetMapSeed());
      return;
    }

    if (e->KeyCode == Keys::M) {
      map->ActivateAsssasins();
      return;
    }

    if (messagebox == nullptr) {
      player->KeyDown(e);
      return;
    }

    if (!messagebox->IsActivated) {
      player->KeyDown(e);
      return;
    }
  }

  void KeyUp(KeyEventArgs^ e) {
    // If the message box has appeared, we stop this event
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    player->KeyUp(e);
  }

  void MouseClick(MouseEventArgs^ e) {
    // If the message box has appeared, we stop this event
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    player->Shoot((float)e->Location.X, (float)e->Location.Y);
  }

  void MovementTick(int tickInterval, Label^ label, System::Windows::Forms::Timer^ MessageTimer) {
    // If the message box has appeared, we stop this timer
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    map->GetCurrentScene()->MoveNPCS(mapGrid, player, tickInterval, allies);

    if (allies != nullptr) {
      for each (Ally ^ ally in allies) {
        Point deltas = ally->GetDelta();
        ally->Move(deltas.X, deltas.Y, mapGrid);
      }
    }

    if (player != nullptr) {
      if (player->MoveUsingKeysList(mapGrid->walkableLayer)) {
        Point position = player->GetPosition();
        Size size = player->GetSize();
        Point newPosition = position;

        if (position.Y < 78) {
          // Up
          map->ChangeScene(Direction::Up);
          newPosition = Point(position.X - size.Width / 2, 519 - size.Height);
        } else if (position.Y > 536) {
          // Down
          map->ChangeScene(Direction::Down);
          newPosition = Point(position.X - size.Width / 2, 104);
        } else if (position.X < 78) {
          // Left
          map->ChangeScene(Direction::Left);
          newPosition = Point(830 - size.Width, position.Y - size.Height / 2);
        } else if (position.X > 850) {
          // Right
          map->ChangeScene(Direction::Right);
          newPosition = Point(104, position.Y - size.Height / 2);
        }

        if (!position.Equals(newPosition)) {
          player->SetPosition(newPosition);
          for each (Ally ^ ally in allies) {
            ally->SetPosition(newPosition);
          }
          // O^2 = Too slow, too much CPU
          // Didn't have time to optimize it
          mapGrid->UpdateNodes(GetWalkableLayer(map->GetCurrentScene()));
          // This one is faster, but has bugs
          // mapGrid->walkableLayer = GetWalkableLayer(map->GetCurrentScene());
        }
      }
    }

    if (map->IsAssassinActivated() && messagebox == nullptr && label != nullptr) {
      messagebox = gcnew Messagebox(label);
      label->Visible = true;
      MessageTimer->Start();
    }
  }

  void PrintLetterTick(Label^ label, System::Windows::Forms::Timer^ MessageTimer) {
    messagebox->PrintLetter(label, MessageTimer);
  }

  void AnimationTick() {
    // If the message box has appeared, we stop this timer
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    map->GetCurrentScene()->AnimateNPCS();

    if (allies != nullptr) {
      for each (Ally ^ ally in allies) {
        ally->ShiftCol();
      }
    }

    if (player != nullptr) {
      player->ShiftCol();
    }
  }

  void ResetPathfinders() {
    // If the message box has appeared, we stop this timer
    if (messagebox != nullptr && messagebox->IsActivated) {
      return;
    }

    // Prevent execution if NPCs is nullptr
    // This will prevent the worst error in the debugger, ever:
    // 'System.NullReferenceException' occurred in Unknown Module
    // Don't suffer what I've suffered. Don't forget to check this first.
    if (allies == nullptr)
      return;

    Node^ playerNode = mapGrid->GetNodeFromWorldPoint(player->Position);
    List<Node^>^ playerNeighbours = mapGrid->GetNeighbours(playerNode);

    if (allies->Count > 0) {
      for each (Ally ^ ally in allies) {
        Node^ currentNode = mapGrid->GetNodeFromWorldPoint(ally->GetPosition());
        if (!playerNeighbours->Contains(currentNode)) {
          Pathfinder::FindPath(mapGrid, ally->GetPosition(), player->GetPosition(), ally);
        }
      }
    }

    // We must reset the pathfinders of the entities from the map
    map->GetCurrentScene()->ResetPathfinders(mapGrid, playerNeighbours, player->Position, allies);
  }

  property Messagebox^ MsgBox {
    Messagebox^ get() {
      return messagebox;
    }
  }

  property float PlayerHealth {
    float get() {
      return player->GetHealth();
    }
  }

private:
  static GraphicsPath^ GetWalkableLayer(Scene^ scene) {
    GraphicsPath^ walkableLayer = gcnew GraphicsPath();
    walkableLayer->AddRectangle(Rectangle(Point(103, 103), Size(728, 417)));

    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ doorImage = bmpManager->GetImage("assets\\sprites\\misc\\door.png");
    int width = doorImage->Width;
    int height = doorImage->Height;
 
    DoorLocations doorLocations = scene->GetDoorLocations();
    if (doorLocations.Up) {
      walkableLayer->AddRectangle(Rectangle(Point(422, 27), Size(width, height)));
    }
    if (doorLocations.Down) {
      walkableLayer->AddRectangle(Rectangle(Point(422, 520), Size(width, height)));
    }
    if (doorLocations.Left) {
      walkableLayer->AddRectangle(Rectangle(Point(27, 266), Size(height, width)));
    }
    if (doorLocations.Right) {
      walkableLayer->AddRectangle(Rectangle(Point(831, 266), Size(height, width)));
    }

    return walkableLayer;
  }

  void InitGrid(Size ClientSize) {
    Point gridWorldSize = Point(ClientSize);
    PointF nodeRadius = PointF(18, 10);

    mapGrid = gcnew Grid(GetWalkableLayer(map->GetCurrentScene()), gridWorldSize, nodeRadius);
  }

  void InitAllies() {
    allies = gcnew List<Ally^>;
    allies->Add(gcnew Ally(Point(700, 200)));
    allies->Add(gcnew Ally(Point(450, 200)));
  }
};

#endif // !_GAME_H_


