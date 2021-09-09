#!/bin/bash
#MT2020013 Apoorv Panse
#Shell Script to execute and check file descriptor status for 05.c

gcc 05.c
./a.out &
PID=$!
echo "PID of the process : $PID"
echo "List of FDs listed in /proc/PID/fd/"
ls -l /proc/$PID/fd/ | grep test | awk '{print $9 $10 $11}'
echo "Killing process : $PID"
kill -9 $PID
rm test* a.out
echo "Done"