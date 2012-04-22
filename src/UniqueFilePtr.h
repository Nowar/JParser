//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#ifndef UNIQUE_FILE_PTR_H
#define UNIQUE_FILE_PTR_H

#include <cstdio>

class UniqueFilePtr {
public:
  UniqueFilePtr(FILE* fp = NULL) {
    fp_ = fp;
  }
  ~UniqueFilePtr() {
    CloseFile();
  }

  bool OpenFile(char const* filename) {
    CloseFile();
    fp_ = fopen(filename, "rb");
    if (fp_) {
      return true;
    } else {
      return false;
    }
  }

  void CloseFile() {
    if (fp_) {
      fclose(fp_);
      fp_ = NULL;
    }
  }

  FILE* Get() const {
    return fp_;
  }

  FILE* Release() {
    FILE* tmp = fp_;
    fp_ = NULL;
    return tmp;
  }

  FILE& operator*() const {
    return *fp_;
  }

  FILE* operator->() const {
    return fp_;
  }

private:
  FILE* fp_;

private:
  UniqueFilePtr(UniqueFilePtr const*);
  void operator=(UniqueFilePtr const*);
};

#endif
