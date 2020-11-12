#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;

/**
 * @brief BitmapManager is a class used to work with Bitmaps and load
 *        them into memory, only when needed. Allows Bitmap preloading
 *        and caching.
*/
ref class BitmapManager {
  Dictionary<String^, Bitmap^>^ images;

public:
  /**
   * @brief BitmapManager is a class used to work with Bitmaps and load
   *        them into memory, only when needed. Allows Bitmap preloading
   *        and caching.
  */
  BitmapManager() {
    this->images = gcnew Dictionary<String^, Bitmap^>();
  }
  ~BitmapManager() {
    for each (KeyValuePair<String^, Bitmap^> img in this->images) {
      if (img.Value != nullptr)
        delete img.Value;
      delete img.Key;
    }
    this->images->Clear();
    delete this->images;
  }

  /**
   * @brief Checks if the BitmapManager has already loaded an image.
   * @param path URI to the file path, relative to the executable file or absolute.
   * @return True if the URI was found, false otherwise.
  */
  bool HasImage(String^ path) {
    return this->images->ContainsKey(path);
  }

  /**
   * @brief Checks if the BitmapManager has already loaded an image.
   * @param bmp Pointer to another Bitmap.
   * @return True if the Bitmap was found, false otherwise.
  */
  bool HasImage(Bitmap^ bmp) {
    return this->images->ContainsValue(bmp);
  }

  /**
   * @brief Allows the preloading of a Bitmap into memory.
   * @param path URI to the file path, relative to the executable file or absolute.
  */
  void PreloadImage(String^ path) {
    if (!this->images->ContainsKey(path)) {
      Bitmap^ bmp = this->LoadImage(path);
      this->images->Add(path, bmp);
    }
  }

  /**
   * @brief Delete from memory an specific Bitmap.
   * @param path URI to the file path, relative to the executable file or absolute.
  */
  void ReleaseImage(String^ path) {
    Bitmap^ bmp;
    if (this->images->TryGetValue(path, bmp)) {
      images->Remove(path);
      delete bmp;
    }
  }

  /**
   * @brief Get a Bitmap from the filesystem if it is not already loaded in the memory.
   * @param path URI to the file path, relative to the executable file or absolute.
   * @return Pointer to the Bitmap of the URI requested.
  */
  Bitmap^ GetImage(String^ path) {
    Bitmap^ bmp;
    if (!this->images->TryGetValue(path, bmp)) {
      bmp = this->LoadImage(path);
      this->images->Add(path, bmp);
    }
    return bmp;
  }

  /**
   * @brief Get a Bitmap from the filesystem if it is not already loaded in the memory.
   * @param path URI to the file path, relative to the executable file or absolute.
   * @return Pointer to the Bitmap of the URI requested.
  */
  Bitmap^ GetImage(Uri^ uri) {
    String^ path = uri->ToString();
    Bitmap^ bmp;
    if (!this->images->TryGetValue(path, bmp)) {
      bmp = this->LoadImage(path);
      this->images->Add(path, bmp);
    }
    return bmp;
  }

private:
  /**
   * @brief Utility function to create a Bitmap pointer from an URI.
   * @param path URI to the file path, relative to the executable file or absolute.
   * @return Pointer to the Bitmap of the URI requested.
  */
  Bitmap^ LoadImage(String^ path) {
    Bitmap^ bmp = gcnew Bitmap(path);
    return bmp;
  }
};
