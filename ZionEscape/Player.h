#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BitmapManager.h"
#include "Entity.h"
#include "Bullet.h"

using namespace System::Collections::Generic;

ref class Player : public Entity {
  List<Bullet^>^ bullets;
public:
  Player(Point pos)
    : Entity(EntityType::Player, pos, 3U, 10.f, 1.f) {
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    Bitmap^ image = bmpManager->GetImage("assets\\sprites\\principal\\principal_m.png");
    this->bullets = gcnew List<Bullet^>;
    this->SetImage(image, 4, 4);
  }

  Player(Bitmap^ image, short nCols, short nRows, Point pos)
    : Entity(EntityType::Player, pos, 3U, 10.f, 2.f) {
    this->SetImage(image, nCols, nRows);
  }

  ~Player() {
    for each (Bullet ^ bullet in this->bullets)
      delete bullet;
    this->bullets->Clear();
    delete this->bullets;
  }

  void SetSpriteDirection(Direction direction) override {
    switch (direction) {
      case Direction::Up:
        SetRow(3);
        break;
      case Direction::Down:
        SetRow(0);
        break;
      case Direction::Left:
        SetRow(1);
        break;
      case Direction::Right:
        SetRow(2);
        break;
      default:
        SetRow(0);
        break;
    }
  }

  void DrawHearts(Graphics^ g) {
    Point heartPos = Point(20, 15);
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    //Draw all the healthPoints of the player
    for (float i = 0.f; i < this->healthPoints; i++) {
      g->DrawImage(bmpManager->GetImage("assets\\sprites\\misc\\heart.png"), heartPos);
      //Add the X position of where the next heart will be drawn
      heartPos.X += 46;
    }
  }

  //Create a Bullet
  void Shoot(float posX, float posY) {
    this->bullets->Add(gcnew Bullet(Size(20,20), this->drawingArea.X+10, this->drawingArea.Y+20, posX, posY, 10));
  }

  void ActionBullets(Graphics^ g, Rectangle area, List<NPC^>^ npcs) {
    //Check if there are bullets
    if (this->bullets->Count > 0)
      //Check all the Bullets in the List
      for (unsigned currentBullet = this->bullets->Count; currentBullet > 0; currentBullet--) {

        bool isDeleted = false;
        //Move the bullet
        bullets[currentBullet-1]->Move();
        //Draw the bullet
        bullets[currentBullet-1]->Draw(g);

        //Check if the bullet collides with any Corrupt or Assasin
        for each (NPC ^ npc in npcs)
          if (npc->GetEntityType() == EntityType::Corrupt || npc->GetEntityType() == EntityType::Assassin)
            if (bullets[currentBullet - 1]->Collides(npc->GetDrawingArea())) {
              npc->SetHealth(npc->GetHealth() - this->GetDamagePoints());
              isDeleted = true;
              break;
            }
       
        //Detects if the Bullet is out of the screen
        if (bullets[currentBullet-1]->OutScreen(area))
          isDeleted = true;

        if (isDeleted) {
          //Delete ptr
          delete this->bullets[currentBullet - 1];
          //Delete form the list
          bullets->Remove(bullets[currentBullet - 1]);
        }
      }
  }
};

#endif // !_PLAYER_H_

