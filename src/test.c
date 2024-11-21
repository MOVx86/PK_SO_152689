#include <stdio.h>

#include "test_defines.h"

int main(void) {
    s32 integer = 42;
    f32 floating = 3.14;
    
    printf("Hello World!\n");
    printf("Int:   %d\n", integer);
    printf("Float: %.2f\n", floating);
    
    return 0;
}