#include "pch.h"
#include "MainActivity.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace ZionEscape;

int main() {
  // See why: https://stackoverflow.com/a/6764245
  Application::EnableVisualStyles();
  // See why: https://stackoverflow.com/a/18115466
  Application::SetCompatibleTextRenderingDefault(false);
  // Initialize MainActivity
  Application::Run(gcnew MainActivity);

  return 0;
}
