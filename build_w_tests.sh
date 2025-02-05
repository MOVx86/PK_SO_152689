#!/bin/bash
set -eu

# main build task
pwd

## -------------------------- ##
## BUILDING AND RUNNING TESTS ##
## -------------------------- ##

cFiles=""
while IFS= read -r -d $'\0' file; do
    if [[ "$file" == *"run_tests.c" || "$file" == *"ipc_utils.c" || "$file" == *"pid_array.c" || "$file" == *"manager.c" ]]; then
        cFiles="$cFiles $file"
    fi
done < <(find . -type f \( -name "run_tests.c" -o -name "ipc_utils.c" -o -name "pid_array.c" -o -name "manager.c" \) -print0)
# echo "List of .c files: $cFiles"

assembly="test_fabryka"
defines="-DDEBUG=1"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags="-lncurses"

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags

echo "Running test..."
../bin/$assembly

## ------------------------ ##
## BUILDING MAIN EXECUTABLE ##
## ------------------------ ##

cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -not -path "./ui/*" -not -path "./tests/*" -print0)
# echo "List of .c files: $cFiles"

assembly="fabryka"
defines="-DDEBUG=0 -DUSE_SLEEP=1"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags=""

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags

## ------------------------------- ##
## BUILDING STRESS-TEST EXECUTABLE ##
## ------------------------------- ##

cFiles=""
while IFS= read -r -d $'\0' file; do
    cFiles="$cFiles $file"
done < <(find . -type f -name "*.c" -not -path "./ui/*" -not -path "./tests/*" -print0)
# echo "List of .c files: $cFiles"

assembly="nosleep_fabryka"
defines="-DDEBUG=0 -DUSE_SLEEP=0"
includeFlags="-I./"
compileFlags="-g -O0"
linkerFlags=""

echo "Building $assembly..."
gcc $defines $includeFlags $compileFlags $cFiles -o ../bin/$assembly $linkerFlags

## ------------------------------- ##
## BUILDING SEPARATE UI EXECUTABLE ##
## ------------------------------- ##

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