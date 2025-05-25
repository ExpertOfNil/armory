#include "tests.h"

int main(int argc, char** argv) {
    String output_str = {
        .begin = malloc(sizeof(char) * 8),
        .size = 0,
        .capacity = 8,
    };

    Test_Vec4IsEqual();
    fprintf(stdout, "Passed: Test_Vec4Equal\n");

    Test_Mat4IsEqual();
    fprintf(stdout, "Passed: Test_Mat4Equal\n");

    Test_Mat4Transpose();
    fprintf(stdout, "Passed: Test_Mat4Transpose\n");

    return SUCCESS;
}

