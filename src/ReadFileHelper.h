//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#ifndef READ_FILE_HELPER_H
#define READ_FILE_HELPER_H

#include <stdint.h>
#include <string>

template <bool IsLittleEndian>
class ReadFileHelper;

template <>
class ReadFileHelper<true> {
public:
  static uint32_t GetU32FromByteStr(unsigned char const* str, size_t num) {
    uint32_t ret = 0;
    for (size_t i = 0; i < num; ++i) {
      ret <<= 8;   // shift one byte
      ret += static_cast<uint32_t>(*(str+i));
    }
    return ret;
  }

  static bool Get(uint8_t* ret, FILE* fp, size_t num_byte) {
    if (fread(ret, 1, num_byte, fp) != num_byte) {
      return false;
    }
    return true;
  }
};

#endif
