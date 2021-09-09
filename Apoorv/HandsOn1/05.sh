#!/bin/bash
#MT2020013 Apoorv Panse
#Shell Script to execute and check file descriptor status for 05.c

gcc 5.c
./a.out &
PID=$!
echo "PID of the process : $PID"
ls -l /proc/$PID/fd/
echo "Killing process : $PID"
kill -9 $PID
rm file* a.out
echo "Fin."