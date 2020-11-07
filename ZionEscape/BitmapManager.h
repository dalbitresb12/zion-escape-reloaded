#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;

ref class BitmapManager {
  Dictionary<String^, Bitmap^>^ images;

public:
  BitmapManager() {
    this->images = gcnew Dictionary<String^, Bitmap^>();
  }
  ~BitmapManager() {
    for each (KeyValuePair<String^, Bitmap^> img in this->images) {
      delete img.Value;
    }
    delete this->images;
  }

  bool HasImage(String^ path) {
    return images->ContainsKey(path);
  }

  Bitmap^ GetImage(String^ path) {
    Bitmap^ bmp;
    if (!images->TryGetValue(path, bmp)) {
      bmp = this->LoadImage(path);
      images->Add(path, bmp);
    }
    return bmp;
  }

private:
  Bitmap^ LoadImage(String^ path) {
    Bitmap^ bmp = gcnew Bitmap(path);
    return bmp;
  }
};
