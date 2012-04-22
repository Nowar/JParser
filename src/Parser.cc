//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#include <cassert>
#include <sstream>
#include "Parser.h"
#include "UniqueFilePtr.h"

Parser::Parser() {
}

Parser::~Parser() {
}

bool Parser::Parse() {
  assert(file_name_);
  UniqueFilePtr fp;
  fp.OpenFile(file_name_);
  if (fp.Get() == NULL) {
    return false;
  }
  if (!ParseHeader(fp.Get())) {
    return false;
  }
  return true;
}

void Parser::DumpClassInfo() const {
}

bool Parser::ParseHeader(FILE* fp) {
  char buffer[4];
  if (fread(buffer, 1, 4, fp) == 4) {
    return false;
  }
  if (buffer[0] != 0xCA || buffer[1] != 0xFE ||
      buffer[2] != 0xBA || buffer[3] != 0xBE) {
    return false;
  }
  std::stringstream ss;
  if (fread(buffer, 1, 2, fp) == 2) {
    return false;
  }
  ss.read(buffer, 2);
  ss >> info_.minor_version_;
  if (fread(buffer, 1, 2, fp) == 2) {
    return false;
  }
  ss.read(buffer, 2);
  ss >> info_.major_version_;
  return true;
}
