#!/bin/bash

gcc ex1.c -o ex1
./ex1 &
sleep 1

pid=$(cat /tmp/ex1.pid)
heap=$(cat "/proc/$pid/maps" | grep -E "heap" | awk '{print $1}')
start=0x$(echo "$heap" | cut -d'-' -f1)
end=0x$(echo "$heap" | cut -d'-' -f2)

pass=$(sudo xxd -s $start -l $(($end - $start)) /proc/$pid/mem | grep -oE "pass:.{8}")
rm ex1

echo "$pass"
