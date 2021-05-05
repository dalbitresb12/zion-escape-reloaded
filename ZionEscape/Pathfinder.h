#pragma once

#ifndef _PATHFINDER_H_
#define _PATHFINDER_H_

#include "Grid.h"
#include "Node.h"
#include "NPC.h"

using namespace System;
using namespace System::Collections::Generic;

ref class Pathfinder {
private:
  Pathfinder() {}
  ~Pathfinder() {}

public:
  static void FindPath(Grid^ grid, Point startPos, Point targetPos, NPC^ npc) { // 2n^3 + 23n^2 + 5n + 9t = O(2n^3)
    Node^ startNode = grid->GetNodeFromWorldPoint(startPos); // 2t
    Node^ targetNode = grid->GetNodeFromWorldPoint(targetPos); // 2t

    List<Node^>^ openNodes = gcnew List<Node^>; // 2t
    HashSet<Node^>^ closedNodes = gcnew HashSet<Node^>; // 2t

    openNodes->Add(startNode); // O(1)

    while (openNodes->Count > 0) { // n * (2t + 1t + n(9t) + n + 1t + 2t + 2n^2 + 13n) = 2n^3 + 23n^2 + 5n
      Node^ node = openNodes[0]; // 2t
      for (int i = 1; i < openNodes->Count; ++i) { // 1t + n(9t)
        if (openNodes[i]->fCost < node->fCost || openNodes[i]->fCost == node->fCost) { // 5
          if (openNodes[i]->hCost < node->hCost) // 2t
            node = openNodes[i]; // 2t
        }
      }

      openNodes->Remove(node); // O(n)
      closedNodes->Add(node); // O(1)

      if (node == targetNode) { // 2t
        npc->path = RetracePath(startNode, targetNode);
        return;
      }

      for each (Node ^ neighbour in grid->GetNeighbours(node)) { // n * (2t + 3t + 2n + 8t) = 2n^2 + 13n
        if (!neighbour->walkable || closedNodes->Contains(neighbour)) // 2t
          continue;

        int newCostToNeighbour = node->gCost + GetDistance(node, neighbour); // 3t
        if (newCostToNeighbour < neighbour->gCost || !openNodes->Contains(neighbour)) { // 4t + (1t + 2t + 1t + 2n) = 2n + 8t
          neighbour->gCost = newCostToNeighbour; // 1t
          neighbour->hCost = GetDistance(neighbour, targetNode); // 2t
          neighbour->parent = node; // 1t

          if (!openNodes->Contains(neighbour)) // O(n) + O(n)
            openNodes->Add(neighbour); // O(n)
        }
      }
    }
  }

  static List<Node^>^ RetracePath(Node^ startNode, Node^ targetNode) {
    List<Node^>^ path = gcnew List<Node^>;
    Node^ currentNode = targetNode;

    while (currentNode != startNode) {
      path->Add(currentNode);
      currentNode = currentNode->parent;
    }

    return path;
  }

  static int GetDistance(Node^ a, Node^ b) {
    return GetDistance(a->gridPos, b->gridPos);
  }

  static int GetDistance(Point a, Point b) {
    int distanceX = Math::Abs(a.X - b.X);
    int distanceY = Math::Abs(a.Y - b.Y);

    if (distanceX > distanceY)
      return 14 * distanceY + 10 * (distanceX - distanceY);
    return 14 * distanceX + 10 * (distanceY - distanceX);
  }
};


#endif // !_PATHFINDER_H_
