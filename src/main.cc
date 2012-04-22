//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#include <cstring>
#include "Parser.h"

int main(int argc, char** argv) {
  Parser p;
  for (size_t i = 1; i < argc; ++i) {
    p.SetClassFile(argv[1]);
    p.Parse();
    p.DumpClassInfo();
  }
  return 0;
}
