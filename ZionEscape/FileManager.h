#pragma once

#include "pch.h"
#include "DataTypes.h"

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

ref class FileManager {
public:
  static void SaveSeed(const int seed) {
    std::string path = GetFilePath(std::string("saved_seed.txt"));
    std::fstream file(path, std::ios::out);
    if (file.is_open()) {
      file << seed << "\n";
      file.close();
    }
  }

  static int LoadSeed() {
    std::string path = GetFilePath(std::string("saved_seed.txt"));
    std::fstream file(path, std::ios::in);
    int num = 0;
    if (file.is_open()) {
      std::string seed;
      file >> seed;
      try {
        num = std::stoi(seed);
      } catch (const std::out_of_range&) {}
      file.close();
    }
    return num;
  }

  static void OpenSeedFile() {
    String^ path = GetFilePath("saved_seed.txt");
    std::string nativePath = StringExtensions::ToStdString(path);
    if (!File::Exists(path)) {
      std::fstream file(nativePath, std::ios::out);
      file.close();
    }
    Process^ fileopener = gcnew Process();
    fileopener->StartInfo->FileName = "explorer.exe";
    fileopener->StartInfo->Arguments = path;
    fileopener->Start();
  }

private:
  static std::string GetFilePath(std::string str) {
    if (str.empty()) {
      return "";
    }
    return StringExtensions::ToStdString(Application::StartupPath) + "\\" + str;
  }

  static String^ GetFilePath(String^ str) {
    if (String::IsNullOrEmpty(str)) {
      return String::Empty;
    }
    return Application::StartupPath + "\\" + str;
  }
};
