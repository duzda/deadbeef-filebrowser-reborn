#/bin/sh

PWD=`pwd`
cd `dirname $0`
cp "./build/src/libddb_misc_filebrowser_reborn.so" "/home/$USER/.local/lib/deadbeef/ddb_misc_filebrowser_reborn.so"
cd $PWD