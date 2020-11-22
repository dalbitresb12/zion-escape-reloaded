#pragma once

#ifndef _BULLET_H_
#define _BULLET_H_

ref class Bullet {
  Rectangle drawingArea;
  float floatX, floatY, speed;
  double angle;

public:
  Bullet(Size size, float posX, float posY, float posTargetX, float posTargetY, float speed) {
    this->drawingArea = Rectangle(Point(posX, posY), size);
    //Position of the bullet (float)
    this->floatX = this->drawingArea.X;
    this->floatY = this->drawingArea.Y;
    //Calculate the angle between the player and the mouse
    this->angle = Math::Atan2(this->floatY - posTargetY, posTargetX - this->floatX);
    //Set the speed
    this->speed = speed;
  }

  ~Bullet(){}

  void Move() {
    //Set a new position for the bullet
    this->floatX += Math::Cos(this->angle) * this->speed;
    this->floatY -= Math::Sin(this->angle) * this->speed;
    //Round the value of the float
    this->drawingArea.X = Math::Round(floatX);
    this->drawingArea.Y = Math::Round(floatY);
  }

  void Draw(Graphics^ g) {
    //Draw the bullet
    BitmapManager^ bmpManager = BitmapManager::GetInstance();
    g->DrawImage(bmpManager->GetImage("assets\\sprites\\misc\\bullet.png"), this->drawingArea);
  }

  bool OutScreen(Rectangle area) {
    //Detect if the bullet is out of the area
    if (this->drawingArea.X > area.Width || this->drawingArea.X + this->drawingArea.Width < 0) return true;
    else if (this->drawingArea.Y > area.Height || this->drawingArea.Y + this->drawingArea.Height < 0) return true;
    return false;
  }
};

#endif // !_BULLET_H_
