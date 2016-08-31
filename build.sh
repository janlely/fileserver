#!/bin/bash

BASEDIR=`pwd`
UNAME=`uname -s`
# check libevent version
if [ $UNAME == "Darwin" ];then
    ls /usr/local/lib/libevent.dylib || ls /usr/lib/libevent.dylib
    if [ $? -ne 0 ];then
        echo "please install libevent2 first!"
        exit 0;
    fi
elif [ $UNAME == "Linux" ];then
    ldconfig -p|grep libevent-2*.so
    if [ $? -ne 0 ];then
        echo "please install libevent2 first!"
        exit 0;
    fi
fi

echo "building dependencies......"

echo "building libjson-c"
cd deps
tar xvf json-c.tar.gz
cd json-c
./autogen.sh
if [ $? -ne 0 ];then
    echo "failed building libjson-c, exiting...."
    cd .. && rm -rf json-c
    exit 0;
fi
./configure && make
if [ $? -ne 0 ];then
    echo "failed building libjson-c, exiting...."
    cd .. && rm -rf json-c
    exit 0;
fi

cd $BASEDIR
cd deps
tar xvf ossp.tar.gz
cd ossp
echo "building ossp-uuid libaray..."
./configure && make
if [ $? -ne 0 ];then
    echo "failed building ossp-uuid, exiting...."
    cd .. && rm -rf ossp
    exit 0;
fi

cd $BASEDIR
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ];then
    echo "error: cmake failed"
    cd ../deps && rm -rf ossp json-c
    exit 0;
fi
make

if [ $? -ne 0 ];then
    echo "error: make failed"
    cd ../deps && rm -rf ossp json-c
    exit 0;
fi
cp fileserver ../
cd ../deps && rm -rf ossp json-c
exit 0



