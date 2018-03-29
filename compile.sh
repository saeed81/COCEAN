#!/bin/bash

iobs=0
isvg=1
issh=1


if [ $issh -eq 1 ];then
    gcc -o sshval -O2 -std=c99 -pedantic -Wall sshval.c -lm
fi

if [ $iobs -eq 1 ];then
    gcc -o getobs -O2 -march=native -std=c99 -Wall -D_GNU_SOURCE getobs.c
fi

if [ $isvg -eq 1 ];then
    gcc -o plottid -O2 -march=native -std=c99 -Wall svgtimeseries.c -lm
fi


