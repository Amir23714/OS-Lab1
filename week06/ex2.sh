#!/bin/bash

gcc -o scheduler scheduler.c
gcc -o worker worker.c

./scheduler data.txt
rm ./scheduler
rm ./worker
