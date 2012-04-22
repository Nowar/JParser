//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#ifndef PARSER_H
#define PARSER_H

#include <cstdio>
#include "ClassFileInfo.h"

class Parser {
public:
  Parser();
  ~Parser();

  char const* SetClassFile(char const* file_name) {
    char const* ret = file_name_;
    file_name_ = file_name;
    return ret;
  }
  char const* GetClassFile() const {
    return file_name_;
  }
  bool Parse();
  void DumpClassInfo() const;

private:
  char const* file_name_;
  FILE* fp_;
  ClassFileInfo info_;

private:
  bool ParseHeader(FILE* fp);
};

#endif
