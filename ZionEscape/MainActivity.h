#pragma once

#include "BitmapManager.h"
#include "Grid.h"
#include "Player.h"
#include "Ally.h"

namespace ZionEscape {
  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Drawing;
  using namespace System::Collections;
  using namespace System::Data;
  using namespace System::Windows::Forms;

  // Main Activity Form
  public ref class MainActivity : public Form {
    System::ComponentModel::IContainer^ components;
    System::Windows::Forms::Timer^ MovementTimer;
    // User-defined properties.
    Bitmap^ background;
    GraphicsPath^ unwalkableLayer;
    Grid^ mapGrid;
    Player^ player;
    List<Ally^>^ allies;

    List<Keys>^ keysPressed;
    List<Keys>^ validKeys;

  public:
    MainActivity() {
      // User-defined code.
      BitmapManager^ bmpManager = BitmapManager::GetInstance();
      background = bmpManager->GetImage("assets\\sprites\\scenes\\scene_1.png");

      unwalkableLayer = gcnew GraphicsPath();
      Point gridWorldSize = Point(background->Width, background->Height);
      PointF nodeRadius = PointF(18, 10);
      mapGrid = gcnew Grid(unwalkableLayer, gridWorldSize, nodeRadius);

      player = gcnew Player(Point(200, 200));

      allies = gcnew List<Ally^>;
      allies->Add(gcnew Ally(Point(200, 200)));
      allies->Add(gcnew Ally(Point(300, 200)));
      allies->Add(gcnew Ally(Point(400, 200)));

      keysPressed = gcnew List<Keys>;
      validKeys = gcnew List<Keys>;
      validKeys->Add(Keys::W);
      validKeys->Add(Keys::A);
      validKeys->Add(Keys::S);
      validKeys->Add(Keys::D);

      // Important call. Do not delete.
      InitializeComponent();
    }

  protected:
    ~MainActivity() {
      if (components) {
        delete components;
      }
    }

#pragma region Windows Form Designer generated code
    // Required method for Designer support - do not modify the contents of this method with the code editor.
    void InitializeComponent() {
      this->components = (gcnew System::ComponentModel::Container());
      this->MovementTimer = (gcnew System::Windows::Forms::Timer(this->components));
      this->SuspendLayout();
      // 
      // MovementTimer
      // 
      this->MovementTimer->Enabled = true;
      this->MovementTimer->Interval = 30;
      this->MovementTimer->Tick += gcnew System::EventHandler(this, &MainActivity::MovementTimer_Tick);
      // 
      // MainActivity
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(936, 624);
      this->DoubleBuffered = true;
      this->Name = L"MainActivity";
      this->Text = L"Zion Escape";
      this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainActivity::MainActivity_Paint);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainActivity::MainActivity_KeyDown);
      this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MainActivity::MainActivity_KeyUp);
      this->ResumeLayout(false);

    }
#pragma endregion

  private: void MainActivity_Paint(Object^ sender, PaintEventArgs^ e) {
    Graphics^ world = e->Graphics;
    world->DrawImage(this->background, Point(0, 0));
    this->mapGrid->DrawGizmos(world, Color::Blue);

    for each (Ally ^ ally in allies) {
      ally->Draw(world);
    }

    this->player->Draw(world);
  }

  private: void MainActivity_KeyDown(Object^ sender, KeyEventArgs^ e) {
    if (!validKeys->Contains(e->KeyCode)) return;

    if (!keysPressed->Contains(e->KeyCode))
      keysPressed->Add(e->KeyCode);
  }

  private: void MainActivity_KeyUp(Object^ sender, KeyEventArgs^ e) {
    if (keysPressed->Contains(e->KeyCode))
      keysPressed->Remove(e->KeyCode);
  }

  private: void MovementTimer_Tick(Object^ sender, EventArgs^ e) {
    for each (Ally ^ ally in allies) {
      ally->PathMovement(mapGrid, player);
      ally->Move(ally->GetDelta());
    }

    for each (Keys key in keysPressed) {
      if (!validKeys->Contains(key)) break;
      player->Move(key);
    }
    Refresh();
  }
  };
}
