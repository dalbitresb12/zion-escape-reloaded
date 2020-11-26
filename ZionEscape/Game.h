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

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Threading;
using namespace System::Threading::Tasks;
// TO DO: Remove when UI to show the map seed is added
using namespace System::Diagnostics;

ref class Game {
  Map^ map;
  Grid^ mapGrid;
  Player^ player;
  // TO DO: Get this from the current scene on the map
  // This one should only be a List of Allies, since those
  // are the ones that should always be with the player.
  // All the other NPCs should only be tied to a single scene.
  List<NPC^>^ npcs;
  bool initialized;

public:
  Game() {
    map = gcnew Map();
    initialized = false;
  }

  void Init(Size ClientSize) {
    player = gcnew Player(Point(ClientSize.Width / 2, ClientSize.Height / 2));
    InitNPCs();
    InitGrid(ClientSize);
    initialized = true;
    ResetPathfinders();
  }

  bool HasInitialized() {
    return initialized;
  }
  ~Game() {
    delete map;
  }

  bool IsGenerated() {
    return this->map->IsGenerated();
  }

  int GetMapSeed() {
    return this->map->GetSeed();
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

  void Paint(Graphics^ world) {
    map->DrawCurrent(world);

    if (map != nullptr) {
      DrawMapGizmos(world);
    }

    if (npcs != nullptr) {
      for each (NPC ^ npc in npcs) {
        npc->Draw(world);
      }
    }

    if (player != nullptr) {
      // Add the player to the scene
      player->Draw(world);

      // Draw the player health using the UI controller
      UI::DrawHearts(world, player->GetHealth());
    }
  }

  void KeyDown(KeyEventArgs^ e) {
    // Temporary map seed viewer
    if (e->KeyCode == Keys::P) {
      Debug::WriteLine("Seed: {0}", GetMapSeed());
      return;
    }

    player->KeyDown(e);
  }

  void KeyUp(KeyEventArgs^ e) {
    player->KeyUp(e);
  }

  void MovementTick() {
    // Prevent execution if NPCs is nullptr
    // This will prevent the worst error in the debugger, ever:
    // 'System.NullReferenceException' occurred in Unknown Module
    // Don't suffer what I suffered. Don't forget to check this first.
    if (npcs != nullptr) {
      for each (NPC ^ npc in npcs) {
        Point deltas = npc->GetDelta();
        npc->Move(deltas.X, deltas.Y);
      }
    }

    if (player != nullptr) {
      if (player->MoveUsingKeysList(mapGrid->walkableLayer)) {
        BitmapManager^ bmpManager = BitmapManager::GetInstance();
        Point position = player->GetPosition();
        Size size = player->GetSize();

        // Up
        if (position.Y < 78) {
          map->ChangeScene(Direction::Up);
          player->SetPosition(position.X - size.Width / 2, 519 - size.Height);
        }
        // Down
        if (position.Y > 536) {
          map->ChangeScene(Direction::Down);
          player->SetPosition(position.X - size.Width / 2, 104);
        }
        // Left
        if (position.X < 78) {
          map->ChangeScene(Direction::Left);
          player->SetPosition(830 - size.Width, position.Y - size.Height / 2);
        }
        // Right
        if (position.X > 850) {
          map->ChangeScene(Direction::Right);
          player->SetPosition(104, position.Y - size.Height / 2);
        }

        // mapGrid->walkableLayer = GetWalkableLayer(map->GetCurrentScene());
        mapGrid->UpdateNodes(GetWalkableLayer(map->GetCurrentScene()));
      }
    }
  }

  void AnimationTick() {
    if (npcs != nullptr) {
      for each (NPC ^ npc in npcs) {
        npc->ShiftCol();
      }
    }

    if (player != nullptr) {
      player->ShiftCol();
    }
  }

  void ResetPathfinders() {
    // Prevent execution if NPCs is nullptr
    // This will prevent the worst error in the debugger, ever:
    // 'System.NullReferenceException' occurred in Unknown Module
    // Don't suffer what I suffered. Don't forget to check this first.
    if (npcs == nullptr)
      return;

    // Get a List with only Allies
    List<Ally^>^ allies = npcs->
      FindAll(gcnew Predicate<NPC^>(&Ally::CheckIfType))->
      ConvertAll<Ally^>(gcnew Converter<NPC^, Ally^>(&Ally::ConvertFromNPC));

    for each (NPC ^ npc in npcs) {
      if (npc->GetEntityType() == EntityType::Ally || npc->GetEntityType() == EntityType::Assassin) {
        Pathfinder::FindPath(mapGrid, npc->GetPosition(), player->GetPosition(), npc);
      } else if (npc->GetEntityType() == EntityType::Corrupt) {
        // Probably each Corrupt should save a reference to the Ally
        // that it is tracking, so that it can follow the same Ally
        // and not get crazy with the Pathfinder.
        // Maybe the Corrupt can have a property to save the tracked Ally?
        // If it isn't assigned (nullptr) we assign it one Ally randomly.
        // This property could be public (it needs to be accesible everywhere).
        Random r;
        Ally^ ally = allies[r.Next(0, allies->Count)];
        Pathfinder::FindPath(mapGrid, npc->GetPosition(), ally->GetPosition(), npc);
      }
    }
  }

private:
  static GraphicsPath^ GetWalkableLayer(Scene^ scene) {
    // TO DO: Get this from the current scene on the map
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

  void InitNPCs() {
    npcs = gcnew List<NPC^>;
    npcs->Add(gcnew Ally(Point(700, 200)));
    npcs->Add(gcnew Ally(Point(450, 200)));
    npcs->Add(gcnew Assassin(Point(300, 100)));
    npcs->Add(gcnew Corrupt(Point(300, 100)));
  }
};

#endif // !_GAME_H_


