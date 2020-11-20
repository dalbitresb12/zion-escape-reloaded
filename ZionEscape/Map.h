#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "Scene.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;

ref class Map {
  Scene^ currentScene;
  Random^ rnd;
  int seed;
  int maxScenes;
  bool isGenerating;
  bool generated;

public:
  Map() : Map(40, 50, Environment::TickCount) {}

  Map(int seed) : Map(40, 50, seed) {}

  Map(int min, int max) : Map(min, max, Environment::TickCount) {}

  Map(int min, int max, int seed) {
    this->seed = seed;
    this->rnd = gcnew Random(seed);
    this->maxScenes = rnd->Next(min, max);
    this->generated = false;
  }

  ~Map() {
    delete currentScene;
    delete rnd;
  }

  void StartGeneration() {
    // Prevent execution if the map has already been generated
    if (generated)
      return;

    // Set the position of the first scene in the virtual grid
    Point position = Point(0, 0);
    // Initialize the first scene
    currentScene = gcnew Scene(DoorLocations(true), position);
    // Initialize points List (only used to detect collisions)
    List<Point>^ points = gcnew List<Point>;
    // Add the first Point
    points->Add(position);
    // Start recursive generation
    GenerateScenes(currentScene, points);
    generated = true;
  }

  void Draw(Graphics^ world) {
    DrawScene(world, currentScene);
  }

  bool IsGenerated() {
    return this->generated;
  }

  int GetSeed() {
    return this->seed;
  }

private:
  void GenerateScenes(Scene^ scene, List<Point>^ points) {
    for each (KeyValuePair<Direction, SceneSpawner^> element in scene->GetSpawners()) {
      // Save the reference to the current spawner as a local
      SceneSpawner^ currentSpawner = element.Value;

      // Skip the current spawner since it is nullptr
      if (currentSpawner == nullptr)
        continue;

      // Get the possible position of this scene
      Point position = currentSpawner->GetPos();
      // Get the door direction that the next scene will need
      Direction doorNeeded = currentSpawner->GetParentDirection();
      bool spawnerCollides = false;

      // Check all scenes for possible collisions
      for each (Point point in points) {
        if (position.Equals(point)) {
          spawnerCollides = true;
          break;
        }
      }

      // Continue to next loop and delete the current spawner
      if (spawnerCollides) {
        delete currentSpawner;
        continue;
      }

      // Initialize values for the new scene
      DoorLocations doorLocations;

      if (points->Count < maxScenes) {
        // Set default values
        doorLocations.SetAll(true);
        // Get a random open or closed door
        do {
          if (doorNeeded != Direction::Up)
            doorLocations.Up = rnd->Next(0, 2);
          if (doorNeeded != Direction::Down)
            doorLocations.Down = rnd->Next(0, 2);
          if (doorNeeded != Direction::Left)
            doorLocations.Left = rnd->Next(0, 2);
          if (doorNeeded != Direction::Right)
            doorLocations.Right = rnd->Next(0, 2);
        } while (doorLocations.IsAllTrue());
      } else {
        // If the number of scenes reaches the max scenes,
        // the current scene needs to be closed
        switch (doorNeeded) {
          case Direction::Up:
            doorLocations.SetAll(true, false, false, false);
            break;
          case Direction::Down:
            doorLocations.SetAll(false, true, false, false);
            break;
          case Direction::Left:
            doorLocations.SetAll(false, false, true, false);
            break;
          case Direction::Right:
            doorLocations.SetAll(false, false, false, true);
            break;
        }
      }

      // Create the new scene
      Scene^ generatedScene = gcnew Scene(doorLocations, position);
      // Save the Point to the List
      points->Add(position);
      // Add as a neighbour to the current scene
      scene->AddNeighbour(EnumUtilities::GetInverseDirection(doorNeeded), generatedScene);
      // Delete the spawner because the scene has been created
      delete currentSpawner;
      // Continue generating more scenes using recursion
      GenerateScenes(generatedScene, points);
    }

    // Clear the List since all the spawners have been used
    scene->GetSpawners()->Clear();
  }

  void DrawScene(Graphics^ world, Scene^ scene) {
    Point position = scene->GetPos();
    Size size = scene->GetBackgroundSize();
    Point worldPos = Point((position.X + 10) * size.Width, (position.Y + 10) * size.Height);
    Rectangle rect = Rectangle(worldPos, size);

    if (position.Equals(Point(0, 0))) {
      world->FillRectangle(Brushes::Crimson, rect);
    } else {
      world->FillRectangle(Brushes::CornflowerBlue, rect);
    }

    scene->Draw(world);

    for each (KeyValuePair<Direction, Scene^> element in scene->GetNeighbours()) {
      Scene^ neighbour = element.Value;
      DrawScene(world, neighbour);
    }
  }

  static void CloseDoor(Direction doorNeeded, Scene^ scene) {
    scene->SetDoorValue(EnumUtilities::GetInverseDirection(doorNeeded), false);
  }

  static void CloseDoorScene(Direction doorNeeded, Scene^ sceneA, Scene^ sceneB) {
    if (!sceneB->GetDoorValue(doorNeeded)) {
      sceneA->SetDoorValue(EnumUtilities::GetInverseDirection(doorNeeded), false);
    }
  }
};

#endif // !_MAP_H_
