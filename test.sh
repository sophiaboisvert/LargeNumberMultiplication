#!/bin/bash

for number in {1..1000}

do
./a.out
output_file=$(cat output.txt)

if [[ "$output_file" == "152415765279684" ]]; then
 echo "Test Passed"
else
 echo "Test Failed"
fi

done
exit 0
