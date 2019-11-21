#!/bin/sh

exe=$1 #发布的程序名称
des=$2 #你的路径

deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des