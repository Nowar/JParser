//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#include <cassert>
#include "Parser.h"
#include "ReadFileHelper.h"
#include "UniquePtr.h"
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
  if (!ParseHeader(fp.Get()) ||
      !ParseConstPool(fp.Get()) ||
      !ParseFlagAndClass(fp.Get()) ||
      !ParseInterface(fp.Get()) ||
      !ParseField(fp.Get()) ||
      !ParseMethod(fp.Get()) ||
      !ParseAttr(fp.Get())) {
    return false;
  }
  long int pos = ftell(fp.Get());
  if (fseek(fp.Get(), 0, SEEK_END) == 0) {
    if (ftell(fp.Get()) != pos) {
      return false;
    }
  }
  return true;
}

bool Parser::ParseHeader(FILE* fp) {
  unsigned char buffer[4];
  if (fread(buffer, 1, 4, fp) != 4) {
    return false;
  }
  if (buffer[0] != 0xCA || buffer[1] != 0xFE ||
      buffer[2] != 0xBA || buffer[3] != 0xBE) {
    return false;
  }
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.minor_version_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.major_version_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  return true;
}

bool Parser::ParseConstPool(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.const_pool_count_ = ReadFileHelper<true>::GetU32FromByteStr(buffer,
                                                                     2);
  if (klass_.const_pool_count_ == 0) {
    return true;
  }
  klass_.const_pool_ = new ConstPoolInfo[klass_.const_pool_count_];
  for (size_t i = 1; i < klass_.const_pool_count_; ++i) { // no use index 0
    if (fread(buffer, 1, 1, fp) != 1) {
      return false;
    }
    uint8_t tag = ReadFileHelper<true>::GetU32FromByteStr(buffer, 1);
    if (!ParseConstPoolContents(klass_.const_pool_[i], fp, tag)) {
      return false;
    }
  }
  return true;
}

bool Parser::ParseConstPoolContents(ConstPoolInfo& info, FILE* fp,
                                    uint8_t tag) {
  info.tag_ = tag;
  UniquePtr<CPParseHelper> helper(CreateCPParseHelper(tag));
  if (!helper->Parse(info, fp)) {
    return false;
  }
  return true;
}

bool Parser::ParseFlagAndClass(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.access_flags_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.this_class_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.super_class_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  return true;
}

bool Parser::ParseInterface(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.interfaces_count_ = ReadFileHelper<true>::GetU32FromByteStr(buffer,
                                                                     2);
  if (klass_.interfaces_count_ == 0) {
    return true;
  }
  klass_.interfaces_ = new uint16_t[klass_.interfaces_count_];
  for (size_t i = 0; i < klass_.interfaces_count_; ++i) {
    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.interfaces_[i] = ReadFileHelper<true>::GetU32FromByteStr(buffer,
                                                                    2);
  }
  return true;
}

bool Parser::ParseField(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.fields_count_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (klass_.fields_count_ == 0) {
    return true;
  }
  klass_.fields_ = new FieldInfo[klass_.fields_count_];
  for (size_t i = 0; i < klass_.fields_count_; ++i) {
    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.fields_[i].access_flags_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.fields_[i].name_index_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.fields_[i].descriptor_index_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.fields_[i].attrs_count_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
    if (!ParseAttrInner(klass_.fields_[i].attrs_,
                        klass_.fields_[i].attrs_count_, fp)) {
      return false;
    }
  }
  return true;
}

bool Parser::ParseMethod(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.methods_count_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (klass_.methods_count_ == 0) {
    return true;
  }
  klass_.methods_ = new MethodInfo[klass_.methods_count_];
  for (size_t i = 0; i < klass_.methods_count_; ++i) {
    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.methods_[i].access_flags_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.methods_[i].name_index_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.methods_[i].descriptor_index_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);

    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    klass_.methods_[i].attrs_count_ =
        ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
    if (!ParseAttrInner(klass_.methods_[i].attrs_,
                        klass_.methods_[i].attrs_count_, fp)) {
      return false;
    }
  }
  return true;
}

bool Parser::ParseAttr(FILE* fp) {
  unsigned char buffer[2];
  if (fread(buffer, 1, 2, fp) != 2) {
    return false;
  }
  klass_.attrs_count_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
  if (!ParseAttrInner(klass_.attrs_, klass_.attrs_count_, fp)) {
    return false;
  }
  return true;
}

bool Parser::ParseAttrInner(AttrInfo*& info, size_t num, FILE* fp) {
  assert(info == NULL && "info should be uninitialized");
  if (num == 0) {
    return true;
  }
  info = new AttrInfo[num];
  unsigned char buffer[4];
  for (size_t i = 0; i < num; ++i) {
    if (fread(buffer, 1, 2, fp) != 2) {
      return false;
    }
    info[i].attr_name_index_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 2);
    if (fread(buffer, 1, 4, fp) != 4) {
      return false;
    }
    info[i].attr_len_ = ReadFileHelper<true>::GetU32FromByteStr(buffer, 4);
    if (!ParseAttrInnerInner(info[i].info_, info[i].attr_len_, fp)) {
      return false;
    }
  }
  return true;
}

bool Parser::ParseAttrInnerInner(uint8_t*& info, size_t num, FILE* fp) {
  assert(info == NULL && "info should be uninitialized");
  if (num == 0) {
    return true;
  }
  info = new uint8_t[num];
  for (size_t i = 0; i < num; ++i) {
    if (!ReadFileHelper<true>::Get(&info[i], fp, 1)) {
      return false;
    }
  }
  return true;
}
