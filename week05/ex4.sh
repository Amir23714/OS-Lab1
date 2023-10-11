#!/bin/bash

gcc -pthread ex4.c -o ex4

printf "m = 1:\n" >> ex4_res.txt
/bin/time -p -a -o ex4_res.txt ./ex4 10000000 1

printf "m = 2:\n" >> ex4_res.txt
/bin/time -p -a -o ex4_res.txt ./ex4 10000000 2

printf "m = 4:\n" >> ex4_res.txt
/bin/time -p -a -o ex4_res.txt ./ex4 10000000 4

printf "m = 10:\n" >> ex4_res.txt
/bin/time -p -a -o ex4_res.txt ./ex4 10000000 10

printf "m = 100:\n" >> ex4_res.txt
/bin/time -p -a -o ex4_res.txt ./ex4 10000000 100

rm ex4
