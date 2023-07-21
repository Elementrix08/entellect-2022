#!/bin/bash


echo "Compiling code"
make

echo "Running test cases..."
for i in {1..5}
do
    inputFile="$i.in"
    outputFile="$i.out"  
    ./main < $inputFile > $outputFile
done

echo "DONE"