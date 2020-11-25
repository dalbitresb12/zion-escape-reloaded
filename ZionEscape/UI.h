#pragma once

#ifndef _UI_H_
#define _UI_H_

#include "BitmapManager.h"
#include "Enums.h"

using namespace System::Drawing;

ref class UI {
  UI() {}
  ~UI() {}

public:
  static void DrawMenu(Graphics^ world, Size ClientSize) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ background = bmpManager->GetImage("assets\\sprites\\misc\\menu-bg.png");

    world->DrawImage(background, Point(0, 0));

    Font^ font = gcnew Font(FontFamily::GenericSansSerif, 32);
    PointF position = PointF(Math::Round((float)ClientSize.Width / 2 - 140), Math::Round((double)ClientSize.Height / 2 - 50));
    world->DrawString("Zion Escape", font, Brushes::White, position);
  }

  static void DrawHearts(Graphics^ world, float healthPoints) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ heartImage = bmpManager->GetImage("assets\\sprites\\misc\\heart.png");
    Rectangle drawingArea = Rectangle(Point(20, 15), Size(30, 30));
    // Draw all the healthPoints of the Player
    for (int i = 0; i < Math::Round(healthPoints); i++) {
      world->DrawImage(heartImage, drawingArea);
      // Add the X position of where the next heart will be drawn
      drawingArea.X += 33;
    }
  }
};

#endif // !_UI_H_
