#!/bin/bash
#MT2020013 Apoorv Panse
#Shell Script to execute and check process information and execution status for 02.c
gcc 02.c
./a.out &
PID=$!
echo "PID of the process is $PID"
echo "The process related information in /proc/$PID :"
cat /proc/$PID/status