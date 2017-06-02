#!/bin/bash

#自定义编译输出目录
CUSTOM_BUILD_DIR=$(cd "$(dirname "$0")"; pwd)
CUSTOM_BUILD_OUT_DIR=$CUSTOM_BUILD_DIR/outputs

rm -r $CUSTOM_BUILD_OUT_DIR
mkdir $CUSTOM_BUILD_OUT_DIR

sudo rm -r $CUSTOM_BUILD_DIR/android-toolchain

$NDK_HOME/build/tools/make-standalone-toolchain.sh \
--arch=arm --platform=android-14 --install-dir=$CUSTOM_BUILD_DIR/android-toolchain

TOOLCHAIN_DIR=$CUSTOM_BUILD_DIR/android-toolchain/bin
echo "toolchain目录：$TOOLCHAIN_DIR"

export PATH=$PATH:$TOOLCHAIN_DIR
echo "PATH：$PATH"


cd ./poco

sudo make clean
./configure --config=Android --no-samples --no-tests --omit="Util JSON XML Crypto NetSSL_OpenSSL Zip Data Data/SQLite Data/ODBC Data/MySQL MongoDB Redis PDF CppParser PageCompiler"

#make -s -j4
make -s -j4 ANDROID_ABI=armeabi-v7a

#Copy lib files
cp -r ./lib/Android/*  $CUSTOM_BUILD_OUT_DIR

#copy include files
mkdir -p $CUSTOM_BUILD_OUT_DIR/include/Poco
for i in {Foundation,Util,XML,Net,NetSSL_OpenSSL,Crypto,Data,Data/SQLite}
do
echo "Copy $i include files......."
cp -r ./$i/include/Poco/*  $CUSTOM_BUILD_OUT_DIR/include/Poco
done
