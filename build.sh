#!/bin/bash

# main build task
pwd

# build main executable
cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -not -path "./ui/*" -not -path "./tests/*" -print0)
# echo "List of .c files: $cFiles"

assembly="fabryka"
defines="-DDEBUG=0"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags=""

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags

# build separate ui executable
cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -not -path "./core/*" -not -path "./tests/*" -print0)
# echo "List of .c files: $cFiles"

assembly="ui_fabryka"
defines="-DDEBUG=0 -DUSE_SLEEP=1"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags="-lncurses"

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags
