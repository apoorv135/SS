#!/bin/bash
#MT2020013 Apoorv Panse
#Shell Script to open the file with od and check the empty spaces in between the data.

gcc 10.c
./a.out
echo "Output of od : \n"
od -c test10.txt
rm test* a.out