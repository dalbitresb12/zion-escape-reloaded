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
    // Initialize points List (only used to detect collisions)
    Dictionary<Point, int>^ points = gcnew Dictionary<Point, int>;
    // Initialize the first scene
    currentScene = gcnew Scene(DoorLocations(true), position);
    // Add the first Point
    points->Add(position, currentScene->GetHashCode());
    // Create the default spawners
    currentScene->CreateSpawners(points);
    // Start recursive generation
    GenerateScene(currentScene, points);
    // Finish generation
    generated = true;
  }

  void Draw(Graphics^ world) {
    List<int>^ drawnNodes = gcnew List<int>;

    DrawScene(world, currentScene, drawnNodes);

    drawnNodes->Clear();
    delete drawnNodes;
  }

  bool IsGenerated() {
    return this->generated;
  }

  int GetSeed() {
    return this->seed;
  }

private:
  void GenerateScene(Scene^ scene, Dictionary<Point, int>^ points) {
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

      // Detect possible collisions. Continues to the next
      // loop and deletes the current spawner if a collision
      // was found.
      if (points->ContainsKey(position)) {
        int hashCode;
        points->TryGetValue(position, hashCode);

        if (hashCode != currentSpawner->GetHashCode()) {
          scene->SetDoorValue(EnumUtilities::GetInverseDirection(doorNeeded), false);
          delete currentSpawner;
          continue;
        }
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
      // Create the spawners and add them to the points List
      generatedScene->CreateSpawners(points, scene, doorNeeded);
      // Remove the spawner from the Dictionary
      points->Remove(position);
      // Save the Point to the Dictionary with the scene hash
      points->Add(position, generatedScene->GetHashCode());
      // Add as a neighbour to the current scene
      scene->AddNeighbour(element.Key, generatedScene);
      // Delete the spawner because the scene has been created
      delete currentSpawner;
      // Continue generating more scenes using recursion
      GenerateScene(generatedScene, points);
    }

    // Clear the List since all the spawners have been used
    scene->GetSpawners()->Clear();
  }

  void DrawScene(Graphics^ world, Scene^ scene, List<int>^ drawnNodes) {
    if (drawnNodes->Contains(scene->GetHashCode()))
      return;

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
    drawnNodes->Add(scene->GetHashCode());

    for each (KeyValuePair<Direction, Scene^> element in scene->GetNeighbours()) {
      Scene^ neighbour = element.Value;
      DrawScene(world, neighbour, drawnNodes);
    }
  }
};

#endif // !_MAP_H_
