#pragma once

#include "Grid.h"
#include "NPC.h"
#include "Pathfinder.h"
#include "Node.h"

using namespace System;
using namespace System::Collections::Generic;

ref class Pathfinder {
private:
  Pathfinder() {}

public:
  static void FindPath(Grid^ grid, Point startPos, Point targetPos, NPC^ npc) {
    Node^ startNode = grid->GetNodeFromWorldPoint(startPos);
    Node^ targetNode = grid->GetNodeFromWorldPoint(targetPos);

    List<Node^>^ openNodes = gcnew List<Node^>;
    HashSet<Node^>^ closedNodes = gcnew HashSet<Node^>;

    openNodes->Add(startNode);

    while (openNodes->Count > 0) {
      Node^ node = openNodes[0];
      for (int i = 1; i < openNodes->Count; ++i) {
        if (openNodes[i]->GetFCost() < node->GetFCost() || openNodes[i]->GetFCost() == node->GetFCost()) {
          if (openNodes[i]->hCost < node->hCost)
            node = openNodes[i];
        }
      }

      openNodes->Remove(node);
      closedNodes->Add(node);

      if (node == targetNode) {
        npc->path = RetracePath(startNode, targetNode);
        return;
      }

      for each (Node ^ neighbour in grid->GetNeighbours(node)) {
        if (!neighbour->walkable || closedNodes->Contains(neighbour))
          continue;

        int newCostToNeighbour = node->gCost + GetDistance(node, neighbour);
        if (newCostToNeighbour < neighbour->gCost || !openNodes->Contains(neighbour)) {
          neighbour->gCost = newCostToNeighbour;
          neighbour->hCost = GetDistance(neighbour, targetNode);
          neighbour->parent = node;

          if (!openNodes->Contains(neighbour))
            openNodes->Add(neighbour);
        }
      }
    }
  }

  static List<Node^>^ RetracePath(Node^ startNode, Node^ targetNode) {
    List<Node^>^ path = gcnew List<Node^>;
    Node^ currentNode = targetNode;

    while (currentNode != targetNode) {
      path->Add(currentNode);
      currentNode = currentNode->parent;
    }

    return path;
  }

  static int GetDistance(Node^ a, Node^ b) {
    int distanceX = Math::Abs(a->gridPos.X - b->gridPos.X);
    int distanceY = Math::Abs(a->gridPos.Y - b->gridPos.Y);

    if (distanceX > distanceY)
      return 14 * distanceY + 10 * (distanceX - distanceY);
    return 14 * distanceX + 10 * (distanceY - distanceX);
  }
};
