#!/bin/bash

for a in *.tar.gz
do
    a_dir=`expr $a : '\(.*\).tar.gz'`
    mkdir $a_dir 2>/dev/null
    tar -xvzf $a -C $a_dir
done