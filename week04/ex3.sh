#!/bin/bash

gcc ex3.c -o ex3 -lm

./ex3 3 &
pstree
sleep 6
pstree
sleep 6
pstree

./ex3 5 &
pstree
sleep 6
pstree
sleep 12
pstree


