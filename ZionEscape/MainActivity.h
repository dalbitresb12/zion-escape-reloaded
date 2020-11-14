#pragma once
#include "Game.h"

namespace ZionEscape {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for MainActivity
  /// </summary>
  public ref class MainActivity : public System::Windows::Forms::Form
  {
    Graphics^ graficador;
    BufferedGraphics^ buffer;
    Game^ game;
  public:
    MainActivity(void)
    {
      InitializeComponent();
      this->graficador = CreateGraphics();
      this->buffer = BufferedGraphicsManager::Current->Allocate(this->graficador, this->ClientRectangle);
      this->game = gcnew Game(this->buffer->Graphics);
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~MainActivity()
    {
      if (components)
      {
        delete components;
      }
      delete buffer;
      delete graficador;
      delete game;
    }
  private: System::Windows::Forms::Timer^ generator;
  protected:
  private: System::ComponentModel::IContainer^ components;

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>


#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->components = (gcnew System::ComponentModel::Container());
      this->generator = (gcnew System::Windows::Forms::Timer(this->components));
      this->SuspendLayout();
      // 
      // generator
      // 
      this->generator->Enabled = true;
      this->generator->Interval = 10;
      this->generator->Tick += gcnew System::EventHandler(this, &MainActivity::generate_world);
      // 
      // MainActivity
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(936, 624);
      this->Name = L"MainActivity";
      this->Text = L"Zion Escape";
      this->ResumeLayout(false);

    }
#pragma endregion
  private: System::Void generate_world(System::Object^ sender, System::EventArgs^ e) {
    this->game->Generation(this->buffer->Graphics);
    this->buffer->Render();
    //If the map is generated, the timer stops
    if (this->game->IsGenerated()) {
      this->generator->Stop();
    }
  }
  };
}
