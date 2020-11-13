#pragma once
#include "Entity.h"
ref class Player : public Entity {
public:
  Player(int width, int height) {
    this->width = width; this->height = height;
    dx = 3; dy = 3;
    x = 70; y = 70;
    idx = idy = 0;
  }
  void drawPlayer(Graphics^ g, Bitmap^ img) {
    Rectangle sectionShown = Rectangle(idx * width, idy * height, width, height);
    Rectangle zoom = Rectangle(x, y, width, height);
    g->DrawImage(img, zoom, sectionShown, GraphicsUnit::Pixel);
  }

  void movePlayer(Graphics^ g, char key) {
    switch (key) {
    case 'A':
      if (x > 0) {
        idy = 1;
        x -= dx;
      }
      break;

    case 'D':
      if (x + width < g->VisibleClipBounds.Width) {
        idy = 2;
        x += dx;
      }
      break;

    case 'W':
      if (y > 0) {
        idy = 3;
        y -= dy;
      }
      break;

    case 'S':
      if (y + height < g->VisibleClipBounds.Height) {
        idy = 0;
        y += dy;
      }
      break;
    }

    idx++;
    if (idx > 3) idx = 0;
  }

  int getX() { return x; }
  int getY() { return y; }
};

