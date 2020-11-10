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

  MainActivity^ mainActivity = gcnew MainActivity();
  Application::Run(mainActivity);

  return 0;
}
