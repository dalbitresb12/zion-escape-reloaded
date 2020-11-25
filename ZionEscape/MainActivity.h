#pragma once

#include "BitmapManager.h"
#include "Pathfinder.h"
#include "Game.h"
#include "Grid.h"
#include "Player.h"
#include "Ally.h"
#include "Assassin.h"
#include "Corrupt.h"

namespace ZionEscape {
  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Drawing;
  using namespace System::Collections;
  using namespace System::Data;
  using namespace System::Windows::Forms;
  using namespace System::Drawing::Text;
  using namespace System::Diagnostics;

  // Main Activity Form
  public ref class MainActivity : public Form {
    System::ComponentModel::IContainer^ components;
    System::Windows::Forms::Timer^ MovementTimer;
    System::Windows::Forms::Timer^ AnimationTimer;
    // User-defined properties.
    UserInterface currentUI;
    Point prevMouseLoc;
    Point mouseLoc;
    Game^ game;

  public:
    MainActivity() {
      // Important call. Do not delete.
      InitializeComponent();

      // User-defined code.
      currentUI = UserInterface::MainMenu;
    }

  protected:
    ~MainActivity() {
      if (components) {
        delete components;
      }
      delete game;
    }

#pragma region Windows Form Designer generated code
    // Required method for Designer support - do not modify the contents of this method with the code editor.
    void InitializeComponent() {
      this->components = (gcnew System::ComponentModel::Container());
      this->MovementTimer = (gcnew System::Windows::Forms::Timer(this->components));
      this->AnimationTimer = (gcnew System::Windows::Forms::Timer(this->components));
      this->SuspendLayout();
      // 
      // MovementTimer
      // 
      this->MovementTimer->Enabled = true;
      this->MovementTimer->Interval = 20;
      this->MovementTimer->Tick += gcnew System::EventHandler(this, &MainActivity::MovementTimer_Tick);
      // 
      // AnimationTimer
      // 
      this->AnimationTimer->Enabled = true;
      this->AnimationTimer->Interval = 80;
      this->AnimationTimer->Tick += gcnew System::EventHandler(this, &MainActivity::AnimationTimer_Tick);
      // 
      // MainActivity
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(936, 624);
      this->DoubleBuffered = true;
      this->Margin = System::Windows::Forms::Padding(4);
      this->Name = L"MainActivity";
      this->Text = L"Zion Escape";
      this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainActivity::MainActivity_Paint);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainActivity::MainActivity_KeyDown);
      this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MainActivity::MainActivity_KeyUp);
      this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainActivity::MainActivity_MouseClick);
      this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainActivity::MainActivity_MouseMove);
      this->ResumeLayout(false);

    }
#pragma endregion
  private: void MainActivity_Paint(Object^ sender, PaintEventArgs^ e) {
    Graphics^ world = e->Graphics;
    world->SmoothingMode = SmoothingMode::AntiAlias;
    world->TextRenderingHint = TextRenderingHint::AntiAlias;

    if (currentUI == UserInterface::InGame) {
      if (game == nullptr) {
        game = gcnew Game();
        game->MapGeneration();
      }

      if (!game->HasInitialized()) {
        game->Init(ClientSize);
      }

      game->Paint(world);
    } else if (currentUI == UserInterface::Pause) {
      UI::DrawPause(world);
    } else if (currentUI == UserInterface::Credits) {
      UI::DrawCredits(world);
    } else if (currentUI == UserInterface::MainMenu) {
      UI::DrawMenu(world, ClientSize, mouseLoc);
    }
  }

  private: void MainActivity_KeyDown(Object^ sender, KeyEventArgs^ e) {
    if (currentUI == UserInterface::Credits && e->KeyCode == Keys::Escape) {
      currentUI = UserInterface::MainMenu;
      Invalidate();
      return;
    }

    if (currentUI == UserInterface::Pause && e->KeyCode == Keys::Escape) {
      currentUI = UserInterface::InGame;
      MovementTimer->Start();
      AnimationTimer->Start();
      Invalidate();
      return;
    }

    if (currentUI == UserInterface::InGame && game != nullptr) {
      if (e->KeyCode == Keys::Escape) {
        currentUI = UserInterface::Pause;
        MovementTimer->Stop();
        AnimationTimer->Stop();
        Invalidate();
        return;
      }

      game->KeyDown(e);
    }
  }

  private: void MainActivity_KeyUp(Object^ sender, KeyEventArgs^ e) {
    if (game != nullptr)
      game->KeyUp(e);
  }

  private: void MovementTimer_Tick(Object^ sender, EventArgs^ e) {
    if (game != nullptr) {
      game->MovementTick();
      Invalidate();
    }
  }
  private: void AnimationTimer_Tick(Object^ sender, EventArgs^ e) {
    if (game != nullptr) {
      game->AnimationTick();
    }
  }

  private: void MainActivity_MouseMove(Object^ sender, MouseEventArgs^ e) {
    if (currentUI == UserInterface::InGame)
      return;

    // Prevent the event to fire twice for the same mouse location
    // See https://stackoverflow.com/a/23048201
    Point mousePos = e->Location;
    if (mousePos == prevMouseLoc)
      return;

    prevMouseLoc = mouseLoc;
    mouseLoc = mousePos;

    Invalidate();
  }
  private: void MainActivity_MouseClick(Object^ sender, MouseEventArgs^ e) {
    currentUI = UI::ClickEvent(e->Location, currentUI);
    Invalidate();
  }
};
}
