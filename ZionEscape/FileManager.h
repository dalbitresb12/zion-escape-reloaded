#pragma once

#include "pch.h"
#include "DataTypes.h"

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

ref class FileManager {
public:
  static void SaveSeed(const int seed) {
    String^ path = GetFilePath("saved_seed.txt");
    File::WriteAllText(path, seed.ToString());
  }

  static int LoadSeed() {
    String^ path = GetFilePath("saved_seed.txt");
    int seed = 0;
    if (File::Exists(path)) {
      String^ text = File::ReadAllText(path);
      Int32::TryParse(text, seed);
    }
    return seed;
  }

  static void OpenSeedFile() {
    String^ path = GetFilePath("saved_seed.txt");
    if (!File::Exists(path)) {
      File::WriteAllText(path, "");
    }
    Process^ fileopener = gcnew Process();
    fileopener->StartInfo->FileName = "explorer.exe";
    fileopener->StartInfo->Arguments = path;
    fileopener->Start();
  }

private:
  static String^ GetFilePath(String^ str) {
    if (String::IsNullOrEmpty(str)) {
      return String::Empty;
    }
    return Application::StartupPath + "\\" + str;
  }
};
