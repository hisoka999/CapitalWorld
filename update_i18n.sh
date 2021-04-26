#!/bin/bash

for file in $(find src/ -name '*.cpp')
do
    echo $file
    xgettext --keyword=_ --language=C++ --add-comments -j --sort-output -o locale/capitalworld.pot $file
done
