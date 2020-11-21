#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "Map.h"
#include "Grid.h"
#include "Pathfinder.h"
#include "NPC.h"
#include "Player.h"
#include "Ally.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
// TO DO: Remove when UI to show the map seed is added
using namespace System::Diagnostics;

ref class Game {
  Map^ map;
  GraphicsPath^ unwalkableLayer;
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
    // POSSIBLE CAUSE OF DEBUGGER ERROR
    map = gcnew Map();
    initialized = false;
  }

  void Init(Size ClientSize) {
    player = gcnew Player(Point(ClientSize.Width / 2, ClientSize.Height / 2));
    InitGrid(ClientSize);
    initialized = true;
  }

  bool HasInitialized() {
    return initialized;
  }

  void StartGeneration(Graphics^ g) {
    this->map->StartGeneration(g);
    
  }

  bool IsGenerated() {
    return this->map->IsGenerated();
  }

  int GetMapSeed() {
    return this->map->GetSeed();
  }

  void Paint(Graphics^ world) {
    // TO DO: Add Draw for current scene when #19 is merged
    // map->DrawCurrent(world);
    for each (NPC ^ npc in npcs) {
      npc->Draw(world);
    }

    // Add the player to the scene
    player->Draw(world);
    // TO DO: Move this to future UI controller
    player->DrawHearts(world);
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
    for each (NPC ^ npc in npcs) {
      Point deltas = npc->GetDelta();
      npc->Move(deltas.X, deltas.Y);
    }

    player->MoveUsingKeysList();
  }

  void ResetPathfinders() {
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
        List<Ally^>^ allies = gcnew List<Ally^>;
        for each (NPC ^ possibleAlly in npcs) {
          if (possibleAlly->GetEntityType() == EntityType::Ally)
            allies->Add((Ally^)possibleAlly);
        }
        Ally^ ally = allies[r.Next(0, allies->Count)];
        Pathfinder::FindPath(mapGrid, npc->GetPosition(), ally->GetPosition(), npc);
      }
    }
  }

private:
  void InitGrid(Size ClientSize) {
    Point gridWorldSize = Point(ClientSize);
    PointF nodeRadius = PointF(18, 10);

    // TO DO: Get this from the current scene on the map
    unwalkableLayer = gcnew GraphicsPath();

    mapGrid = gcnew Grid(unwalkableLayer, gridWorldSize, nodeRadius);
  }

  void InitNPCs() {
    npcs = gcnew List<NPC^>;
    npcs->Add(gcnew Ally(Point(700, 200)));
    //npcs->Add(gcnew Ally(Point(450, 200)));
    //npcs->Add(gcnew Assassin(Point(300, 100)));
    //npcs->Add(gcnew Corrupt(Point(300, 100)));
  }
};

#endif // !_GAME_H_


