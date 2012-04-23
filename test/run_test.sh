#!/bin/bash
#//==========================================================================//
#// Copyright (c) 2012 Wen-Han Gu. All rights reserved.
#//
#// Author: Nowar Gu (Wen-Han Gu)
#// E-mail: wenhan.gu <at> gmail.com
#//
#// Any advice is welcome. Thank you!
#//==========================================================================//
filename="`cd $1/src/ && ls *.java`"

rm -rf /tmp/jptest_$$
mkdir -p /tmp/jptest_$$
cp $1/src/$filename /tmp/jptest_$$
javac /tmp/jptest_$$/$filename
filename="`ls /tmp/jptest_$$/*.class`"
./out/jp $filename
if [ $? -ne 0 ]; then
  echo "Failed!"
fi
echo "Success!"
