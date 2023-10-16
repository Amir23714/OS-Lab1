#!/bin/bash

gcc -o scheduler_sjf scheduler_sjf.c
gcc -o worker worker.c

./scheduler_sjf data.txt
rm ./scheduler_sjf
rm ./worker

