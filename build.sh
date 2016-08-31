#!/bin/bash

BASEDIR=`pwd`
echo "building dependencies......"

echo "building libjson-c"
cd deps/json-c
./autogen.sh 
if [ $? -ne 0];then
    echo "failed building libjson-c, exiting...."
    exit 0;
fi 
./configure && make
if [ $? -ne 0];then
    echo "failed building libjson-c, exiting...."
    exit 0;
fi 

cd $BASEDIR
cd deps/ossp
echo "building ossp-uuid libaray..."
./configure && make
if [ $? -ne 0];then
    echo "failed building ossp-uuid, exiting...."
    exit 0;
fi 

cd $BASEDIR
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0];then
    echo "error: cmake failed"
    exit 0;
fi 
make

if [ $? -ne 0];then
    echo "error: make failed"
    exit 0;
fi 
cp fileserver ../
exit 0



