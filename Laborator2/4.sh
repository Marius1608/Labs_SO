#!/bin/bash

variabila=$(find . -type f -name "*.c")
for Laborator_ex in $variabila; 
do
    nr=$(wc -l "$Laborator_ex" | cut -d " " -f 1)
    if test "$nr" -gt 20;
    then
        echo "$Laborator_ex"
    fi
done

