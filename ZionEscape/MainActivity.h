#pragma once

namespace ZionEscape {
  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  // Main Activity Form
  public ref class MainActivity : public Form {
    // Important property. Do not delete.
    System::ComponentModel::Container^ components;
    // User-defined properties.

  public:
    MainActivity() {
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
      this->SuspendLayout();
      // 
      // MainActivity
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(556, 388);
      this->Name = L"MainActivity";
      this->Text = L"Zion Escape";
      this->ResumeLayout(false);

    }
#pragma endregion
  };
}
