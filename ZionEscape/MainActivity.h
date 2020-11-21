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
  using namespace System::Diagnostics;

  // Main Activity Form
  public ref class MainActivity : public Form {
    System::ComponentModel::IContainer^ components;
    System::Windows::Forms::Timer^ MovementTimer;
    // User-defined properties.
    Game^ game;

  public:
    MainActivity() {
      // Important call. Do not delete.
      InitializeComponent();

      // User-defined code.
      this->game = gcnew Game();
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

    if (game != nullptr) {
      if (!game->HasInitialized()) {
        game->Init(ClientRectangle.Size);
        // POSSIBLE CAUSE OF DEBUGGER ERROR
        // game->ResetPathfinders();
      }

      game->StartGeneration(world);
      game->Paint(world);
    }
  }

  private: void MainActivity_KeyDown(Object^ sender, KeyEventArgs^ e) {
    if (game != nullptr)
      game->KeyDown(e);
  }

  private: void MainActivity_KeyUp(Object^ sender, KeyEventArgs^ e) {
    if (game != nullptr)
      game->KeyUp(e);
  }

  private: void MovementTimer_Tick(Object^ sender, EventArgs^ e) {
    if (game != nullptr) {
      game->MovementTick();
      Refresh();
    }
  }
};
}
