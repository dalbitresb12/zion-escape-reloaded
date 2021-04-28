#pragma once

#include "Enums.h"
#include "UserInterface.h"

namespace ZionEscapeReloaded {
  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;
  using namespace System::Drawing::Drawing2D;
  using namespace System::Drawing::Text;

  /// <summary>
  /// Summary for MainActivity
  /// </summary>
  public ref class MainActivity : public System::Windows::Forms::Form {
  public:
    MainActivity(void) {
      InitializeComponent();
      Init();
      //
      //TODO: Add the constructor code here
      //
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~MainActivity() {
      if (components) {
        delete components;
      }
    }

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container^ components;

  #pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void) {
      this->SuspendLayout();
      // 
      // MainActivity
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(920, 585);
      this->DoubleBuffered = true;
      this->MaximizeBox = false;
      this->MaximumSize = System::Drawing::Size(936, 624);
      this->MinimumSize = System::Drawing::Size(936, 624);
      this->Name = L"MainActivity";
      this->Text = L"MainActivity";
      this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainActivity::MainActivity_Paint);
      this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainActivity::MainActivity_MouseClick);
      this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainActivity::MainActivity_MouseMove);
      this->ResumeLayout(false);

    }
  #pragma endregion

  private:
    UserInterface currentUI;
    Point prevMouseLoc;
    Point mouseLoc;

  private: void Init() {
    currentUI = UserInterface::MainMenu;
  }

  private: void MainActivity_Paint(Object^ sender, PaintEventArgs^ e) {
    Graphics^ world = e->Graphics;
    world->SmoothingMode = SmoothingMode::AntiAlias;
    world->TextRenderingHint = TextRenderingHint::AntiAlias;

    switch (currentUI) {
      case UserInterface::MainMenu:
      {
        UI::DrawMenu(world, ClientSize, mouseLoc);
      }
    }
  }
  private: void MainActivity_MouseMove(Object^ sender, MouseEventArgs^ e) {
    if (currentUI == UserInterface::InGame)
      return;

    // Prevent the event from firing twice for the same mouse location
    // See https://stackoverflow.com/a/23048201
    Point mousePos = e->Location;
    if (mousePos == prevMouseLoc)
      return;

    prevMouseLoc = mouseLoc;
    mouseLoc = mousePos;

    if (UI::HasPendingRendering(mousePos)) {
      // Re-render the form
      Invalidate();
    }
  }

  private: void MainActivity_MouseClick(Object^ sender, MouseEventArgs^ e) {
    // Fire any event needed in the UI
    UserInterface newUI = UI::ClickEvent(e->Location, currentUI);
    if (currentUI != newUI) {
      currentUI = newUI;
      // Re-render the form
      Invalidate();
    }
  }
  };
}