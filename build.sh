#!/bin/bash

# (DEBUG) build task
pwd

# build main executable
cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -not -path "./ui/*" -print0)
# echo "List of .c files: $cFiles"

assembly="fabryka"
defines="-D_DEBUG"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags=""

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags

# build separate ui executable
cFiles="./ui/ui_terminal.c"

assembly="ui_fabryka"
defines="-D_DEBUG"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags="-lncurses"

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags
