#!/bin/bash

# (DEBUG) build task
pwd


cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -print0)
echo "List of .c files: $cFiles"

assembly="debug_app"
defines="-D_DEBUG"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags=""

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags