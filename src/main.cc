//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#include <cstdlib>
#include <cstring>
#include "Parser.h"

int main(int argc, char** argv) {
  Parser p;
  for (size_t i = 1; i < (unsigned) argc; ++i) {
    p.SetClassFile(argv[1]);
    if (!p.Parse()) {
      return EXIT_FAILURE;
    }
    p.DumpClassInfo();
  }
  return EXIT_SUCCESS;
}
