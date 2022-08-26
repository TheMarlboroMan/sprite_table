#!/bin/bash

if [[ $# -ne 2 ]]
then
	echo "usage: make_appimage.sh path_to_executable path_to_linuxdeploy"
	exit 1
fi;

#export SIGN=1
#export SIGN_KEY=0xLAST8CHARSOFGPGKEYID

MAJOR_VERSION=`grep "set(MAJOR_VERSION" CMakeLists.txt | awk '{print $2}' | tr -d ')'`
MINOR_VERSION=`grep "set(MINOR_VERSION" CMakeLists.txt | awk '{print $2}' | tr -d ')'`
PATCH_VERSION=`grep "set(PATCH_VERSION" CMakeLists.txt | awk '{print $2}' | tr -d ')'`

export VERSION="$MAJOR_VERSION.$MINOR_VERSION.$PATCH_VERSION"
echo "will build $VERSION"

rm -rf AppDir
$2 --appdir AppDir
cp -r resources/data AppDir/usr/share
mkdir -p AppDir/usr/share/metainfo
cp linuxdeploy/*.xml AppDir/usr/share/metainfo/
$2 --executable $1 --appdir AppDir -i ./linuxdeploy/sprite_table.png -d ./linuxdeploy/sprite_table.desktop --output appimage
exit 0
