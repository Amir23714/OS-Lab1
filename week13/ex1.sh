#!/bin/bash

gcc -o ex1 ex1.c -pthread

filename="input.txt"
if [ -e "$filename" ]; then
    # If the file exists, clear its content
    > "$filename"
else
    touch "$filename"
fi

# Example from the lab
lines=("thread resource" "1 2" "3 0" "1 0" "3 2" "1 1" "3 1")

for line in "${lines[@]}"; do
    echo "$line" >> "$filename"
done

./ex1 3 4
rm ex1


