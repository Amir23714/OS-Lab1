#!/bin/bash
gcc  -pthread ex3.c -o ex3

printf "m = 1:\n" >> ex3_res.txt
/bin/time -p -a -o ex3_res.txt ./ex3 10000000 1
printf "m = 2:\n" >> ex3_res.txt
/bin/time -p -a -o ex3_res.txt ./ex3 10000000 2
printf "m = 4:\n" >> ex3_res.txt
/bin/time -p -a -o ex3_res.txt ./ex3 10000000 4
printf "m = 10:\n" >> ex3_res.txt
/bin/time -p -a -o ex3_res.txt ./ex3 10000000 10
printf "m = 100:\n" >> ex3_res.txt
/bin/time -p -a -o ex3_res.txt ./ex3 10000000 100

rm ex3
