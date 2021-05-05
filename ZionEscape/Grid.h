#pragma once

#ifndef _GRID_H_
#define _GRID_H_

#include "MathUtils.h"
#include "Node.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;
using namespace MathUtils;

ref class Grid {
  Point gridWorldSize;
  array<Node^, 2>^ grid;

  PointF nodeRadius;
  PointF nodeDiameter;
  Point gridSize;

public:
  GraphicsPath^ walkableLayer;

  Grid(GraphicsPath^ walkableLayer, Point gridWorldSize, PointF nodeRadius) {
    this->walkableLayer = walkableLayer;
    this->gridWorldSize = gridWorldSize;
    this->nodeRadius = nodeRadius;

    nodeDiameter = PointF(nodeRadius.X * 2, nodeRadius.Y * 2);
    gridSize = Point::Round(PointF(gridWorldSize.X / nodeDiameter.X, gridWorldSize.Y / nodeDiameter.Y));
    CreateGrid(walkableLayer);
  }
  ~Grid() {}

private:
  void CreateGrid(GraphicsPath^ walkableLayer) { // total detallado : 14n^2 + 4n + 3 - total asintótico : O(n^2)
    grid = gcnew array<Node^, 2>(gridSize.X, gridSize.Y); // 2t

    for (int x = 0; x < gridSize.X; ++x) { // 1t + n( 1t + _ _ 2t ) = 1t + n(t + 14n + t + 2t) = 14n^2 + 4n + 1
      for (int y = 0; y < gridSize.Y; ++y) { // 1t + n( 1t + _ _ 2t ) = 1t + n(14t) = 14n + 1t
        Point gridPos = Point(x, y); // t 
        Point worldPos = Point::Round(PointF(x * nodeDiameter.X + nodeRadius.X, y * nodeDiameter.Y + nodeRadius.Y)); //6t
        bool walkable = walkableLayer->IsVisible(worldPos);// 2t
        grid[x, y] = gcnew Node(worldPos, gridPos, walkable);// 2t
      }
    }
  }

public:
  void UpdateNodes(GraphicsPath^ walkableLayer) {
    this->walkableLayer = walkableLayer;

    for (int x = 0; x < gridSize.X; ++x) {
      for (int y = 0; y < gridSize.Y; ++y) {
        Point worldPos = grid[x, y]->worldPos;
        grid[x, y]->walkable = walkableLayer->IsVisible(worldPos);
      }
    }
  }

  List<Node^>^ GetNeighbours(Node^ node) {
    List<Node^>^ neighbours = gcnew List<Node^>;

    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        if (x == 0 && y == 0)
          continue;

        int checkX = node->gridPos.X + x;
        int checkY = node->gridPos.Y + y;

        if (checkX >= 0 && checkX < gridSize.X && checkY >= 0 && checkY < gridSize.Y) {
          neighbours->Add(grid[checkX, checkY]);
        }
      }
    }

    return neighbours;
  }

  Node^ GetNodeFromWorldPoint(Point worldPos) {
    float percentX = (float)worldPos.X / gridWorldSize.X;
    float percentY = (float)worldPos.Y / gridWorldSize.Y;

    percentX = Mathf::Clamp01(percentX);
    percentY = Mathf::Clamp01(percentY);

    int x = Mathf::RoundToInt((gridSize.X - 1) * percentX);
    int y = Mathf::RoundToInt((gridSize.Y - 1) * percentY);
    return grid[x, y];
  }

  void DrawGizmos(Graphics^ world) {
    this->DrawGizmos(world, Color::Black);
  }

  void DrawGizmos(Graphics^ world, Color color) {
    Pen^ pen = gcnew Pen(color, 1);
    int nodeRadiusX = Mathf::RoundToInt(nodeRadius.X);
    int nodeRadiusY = Mathf::RoundToInt(nodeRadius.Y);

    for each (Node ^ node in grid) {
      Point location = Point(node->worldPos.X - nodeRadiusX, node->worldPos.Y - nodeRadiusY);
      Size size = Size(Point::Round(nodeDiameter));
      Rectangle rect = Rectangle(location, size);
      world->DrawRectangle(pen, rect);
    }
  }

  void DrawNodeOutline(Graphics^ world, Point worldPos) {
    Node^ node = GetNodeFromWorldPoint(worldPos);
    SolidBrush^ brush = gcnew SolidBrush(Color::Green);
    Point nodePos = Point(node->worldPos.X - Mathf::RoundToInt(nodeRadius.X), node->worldPos.Y - Mathf::RoundToInt(nodeRadius.Y));
    Size size = Size(Point::Round(nodeDiameter));
    Rectangle rect = Rectangle(nodePos, size);
    world->FillRectangle(brush, rect);
  }
};

#endif // !_GRID_H_

