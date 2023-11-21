#!/bin/bash

FILE_SIZE=50M

# Create a file lofs.img with a size of at least 50 MiB using fallocate
sudo fallocate -l $FILE_SIZE lofs.img

# Set up a loop device using losetup
loop_device=$(sudo losetup -f lofs.img)

# Create an ext4 file system on the created device using mkfs
sudo mkfs.ext4 $loop_device

# Create a new empty directory ./lofsdisk
mkdir -p ./lofsdisk

# Compile ex1.c statically with GCC
gcc ex1.c --static -o ex1
sudo chmod +x ./lofsdisk/ex1

# Copy the compiled executable to ./lofsdisk/ex1
cp ex1 ./lofsdisk/ex1

# Create necessary directories in ./lofsdisk
sudo mkdir -p ./lofsdisk/{bin,lib,lib64}

# Create two files with content in ./lofsdisk
echo "Amir" | sudo tee ./lofsdisk/file1
echo "Gubaidullin" | sudo tee ./lofsdisk/file2

# Function to get the library dependencies of an executable
get_libs() {
    sudo ldd "$1" | awk '{print $3}'
}

# Copy required system binaries to ./lofsdisk/bin/
sudo cp /bin/bash ./lofsdisk/bin/
sudo cp /bin/cat ./lofsdisk/bin/
sudo cp /bin/echo ./lofsdisk/bin/
sudo cp /bin/ls ./lofsdisk/bin/

# Run ex1 within the chroot environment and redirect the output to /ex1.txt
sudo sh -c 'chroot lofsdisk /ex1 > /ex1.txt'
./ex1 >> ex1.txt 

# Mount the created file system to the mount point ./lofsdisk
sudo mount $loop_device ./lofsdisk

# Unmount the file system
sudo umount ./lofsdisk

# Release the loop device
sudo losetup -d $loop_device
