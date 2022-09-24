#!/bin/bash

for i in `seq 1 1 12`
#                 ^^ numer of cpu cores you want to use
do
    ./calc > output.txt &
done

watch -gn 1 grep "tries" output.txt

echo "Correct path found!"
echo "Check output.txt"

killall ./calc
