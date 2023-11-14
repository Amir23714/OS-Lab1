#!/bin/bash

directory_path="$1"

echo "Hello world." > "${directory_path}/test_file.txt"
echo "Created file: test_file.txt"

mkdir "${directory_path}/test_folder"
echo "Created folder: test_folder"

echo "Updated content." > "${directory_path}/test_file.txt"
echo "Modified file: test_file.txt"

ln -s "${directory_path}/test_file.txt" "${directory_path}/test_link.txt"
echo "Created symbolic link: test_link.txt"

cat "${directory_path}/test_file.txt"
ls "${directory_path}/test_folder"

rm "${directory_path}/test_link.txt"
echo "Removed symbolic link: test_link.txt"

rm "${directory_path}/test_file.txt"
rmdir "${directory_path}/test_folder"
echo "Removed file and folder: test_file.txt, test_folder"
