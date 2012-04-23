//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#include <cassert>
#include <cstdio>
#include <cstring>
#include "ClassFile.h"
#include "ReadFileHelper.h"

ClassFile::ClassFile()
: minor_version_(0), major_version_(0),
  const_pool_count_(0), const_pool_(NULL),
  access_flags_(0), this_class_(0), super_class_(0),
  interfaces_count_(0), interfaces_(NULL),
  fields_count_(0), fields_(NULL),
  methods_count_(0), methods_(NULL),
  attrs_count_(0), attrs_(NULL) {
}

ClassFile::~ClassFile() {
  delete [] const_pool_;
  delete [] interfaces_;
  delete [] fields_;
  delete [] methods_;
  delete [] attrs_;
}

ConstPoolInfo::ConstPoolInfo()
: tag_(0), bytes_(NULL) {
#ifndef NDEBUG
  memset(info_, 0xfe, 8);
#endif
}

ConstPoolInfo::~ConstPoolInfo() {
  delete [] bytes_;
}


struct CPParseHelperClass : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 2)) {
      return false;
    }
    return true;
  }
};

struct CPParseHelperMethodref : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 2)) {
      return false;
    }
    if (!ReadFileHelper<true>::Get(&info.info_[2], fp, 2)) {
      return false;
    }
    return true;
  }
};

struct CPParseHelperInteger : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 4)) {
      return false;
    }
    return true;
  }
};

struct CPParseHelperLong : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 4)) {
      return false;
    }
    if (!ReadFileHelper<true>::Get(&info.info_[4], fp, 4)) {
      return false;
    }
    return true;
  }
};

struct CPParseHelperUtf8 : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 2)) {
      return false;
    }
    uint16_t len = ReadFileHelper<true>::GetU32FromByteStr(info.info_, 2);
    info.bytes_ = new uint8_t[len];
    if (!ReadFileHelper<true>::Get(info.bytes_, fp, len)) {
      return false;
    }
    return true;
  }
};

struct CPParseHelperMethodHandle : public CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) {
    if (!ReadFileHelper<true>::Get(info.info_, fp, 1)) {
      return false;
    }
    if (!ReadFileHelper<true>::Get(&info.info_[1], fp, 2)) {
      return false;
    }
    return true;
  }
};


CPParseHelper* CreateCPParseHelper(uint8_t tag) {
  switch (tag) {
  case CONSTANT_Class: {
    return new CPParseHelperClass();
  }
  case CONSTANT_Fieldref: {
    return new CPParseHelperMethodref();
  }
  case CONSTANT_Methodref: {
    return new CPParseHelperMethodref();
  }
  case CONSTANT_InterfaceMethodref: {
    return new CPParseHelperMethodref();
  }
  case CONSTANT_String: {
    return new CPParseHelperClass();
  }
  case CONSTANT_Integer: {
    return new CPParseHelperInteger();
  }
  case CONSTANT_Float: {
    return new CPParseHelperInteger();
  }
  case CONSTANT_Long: {
    return new CPParseHelperLong();
  }
  case CONSTANT_Double: {
    return new CPParseHelperLong();
  }
  case CONSTANT_NameAndType: {
    return new CPParseHelperMethodref();
  }
  case CONSTANT_Utf8: {
    return new CPParseHelperUtf8();
  }
  case CONSTANT_MethodHandle: {
    return new CPParseHelperMethodHandle();
  }
  case CONSTANT_MethodType: {
    return new CPParseHelperClass();
  }
  case CONSTANT_InvokeDynamic: {
    return new CPParseHelperMethodref();
  }
  default: {
    assert(false && "Unsupport constant pool tag");
    return NULL;
  }
  }
}


FieldInfo::FieldInfo()
: access_flags_(0), name_index_(0),
  descriptor_index_(0), attrs_count_(0),
  attrs_(NULL) {
}

FieldInfo::~FieldInfo() {
  delete [] attrs_;
}

AttrInfo::AttrInfo()
: attr_name_index_(0), attr_len_(0),
  info_(NULL) {
}

AttrInfo::~AttrInfo() {
  delete [] info_;
}
