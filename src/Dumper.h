//==========================================================================//
// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
//
// Author: Nowar Gu (Wen-Han Gu)
// E-mail: wenhan.gu <at> gmail.com
//
// Any advice is welcome. Thank you!
//==========================================================================//
#ifndef DUMPER_H
#define DUMPER_H

class ClassFile;

class Dumper {
public:
  void Dump(ClassFile const& klass) const;
};

#endif
