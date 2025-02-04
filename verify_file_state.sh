#!/bin/bash

finished_files=$(grep -rl "// FINISHED" --include=\*.{c,h} .)
unfinished_files=$(grep -rl "// UNFINISHED" --include=\*.{c,h} .)

{
    echo "Finished files:"
    echo "$finished_files"
    echo ""
    echo "Unfinished files:"
    echo "$unfinished_files"
} > ./doc/file_state.txt