//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <stdint.h>
#include <cstdio>

class ConstPoolInfo;
class FieldInfo;
class MethodInfo;
class AttrInfo;

struct ClassFile {
  uint16_t minor_version_;
  uint16_t major_version_;
  uint16_t const_pool_count_;
  ConstPoolInfo* const_pool_; // 1 ~ const_pool_count-1
  uint16_t access_flags_;
  uint16_t this_class_;
  uint16_t super_class_;
  uint16_t interfaces_count_;
  uint16_t* interfaces_;
  uint16_t fields_count_;
  FieldInfo* fields_;
  uint16_t methods_count_;
  MethodInfo* methods_;
  uint16_t attrs_count_;
  AttrInfo* attrs_;

  ClassFile();
  ~ClassFile();
};


struct ConstPoolInfo {
  uint8_t tag_;
  uint8_t info_[8];  // sufficient for all CONSTANT_*
  uint8_t* bytes_;    // Only for CONSTANT_Utf8_info::bytes

  ConstPoolInfo();
  ~ConstPoolInfo();
};

#define CONSTANT_Class              7
#define CONSTANT_Fieldref           9
#define CONSTANT_Methodref          10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String             8
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_NameAndType        12
#define CONSTANT_Utf8               1
#define CONSTANT_MethodHandle       15
#define CONSTANT_MethodType         16
#define CONSTANT_InvokeDynamic      18

struct CPParseHelper {
  virtual bool Parse(ConstPoolInfo& info, FILE* fp) = 0;
};
CPParseHelper* CreateCPParseHelper(uint8_t tag);


struct FieldInfo {
  uint16_t access_flags_;
  uint16_t name_index_;
  uint16_t descriptor_index_;
  uint16_t attrs_count_;
  AttrInfo* attrs_;

  FieldInfo();
  ~FieldInfo();
};


struct MethodInfo : public FieldInfo {
};


struct AttrInfo {
  uint16_t attr_name_index_;
  uint32_t attr_len_;
  uint8_t* info_;

  AttrInfo();
  ~AttrInfo();
};

#endif
