#!/bin/bash
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

gnome-terminal -- bash -c "./publisher 3 && sleep 3; exec bash"

gnome-terminal -- bash -c "./subscriber 1 && sleep 3; exec bash"
gnome-terminal -- bash -c "./subscriber 2 && sleep 3; exec bash"
gnome-terminal -- bash -c "./subscriber 3 && sleep 3; exec bash"

