#!/bin/bash

INPUT_FILES_DIR="inputs"
OUTPUT_FILES_DIR="outputs"

echo "Compiling code"
make

echo "Running test cases..."
for i in {0..5}
do
    inputFile="$INPUT_FILES_DIR/$i.in"
    outputFile="$OUTPUT_FILES_DIR/$i.out"  
    ./main < $inputFile > $outputFile
done

echo "DONE"