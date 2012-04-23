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
#include "ClassFile.h"
#include "Dumper.h"

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
  void DumpClassInfo() const {
    dumper_.Dump(klass_);
  }

private:
  char const* file_name_;
  ClassFile klass_;
  Dumper dumper_;

private:
  bool ParseHeader(FILE* fp);
  bool ParseConstPool(FILE* fp);
  bool ParseConstPoolContents(ConstPoolInfo& info, FILE* fp,
                              uint8_t tag);
  bool ParseFlagAndClass(FILE* fp);
  bool ParseInterface(FILE* fp);
  bool ParseField(FILE* fp);
  bool ParseMethod(FILE* fp);
  bool ParseAttr(FILE* fp);
  bool ParseAttrInner(AttrInfo*&, size_t num, FILE* fp);
  bool ParseAttrInnerInner(uint8_t*&, size_t num, FILE* fp);
};

#endif
