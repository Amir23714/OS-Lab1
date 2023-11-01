#!/bin/bash
gcc pager.c -o pager  
gcc mmu.c -o mmu 

./pager 4 2 &  

PAGER_PID=%1
./mmu 4 "R0 R1 W1 R0 R2 W2 R0 R3 W2" $PAGER_PID

rm pager mmu