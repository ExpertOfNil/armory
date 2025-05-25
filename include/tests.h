#ifndef TESTS_H
#define TESTS_H

#include <assert.h>

#include "types.h"

#define TEST_F32_ERR 1e-7

static void Test_Vec4IsEqual(void);
static void Test_Mat4IsEqual(void);
static void Test_Mat4Transpose(void);

void Test_Vec4IsEqual(void) {
    Vec4 vec = {0.0, 1.0, 2.0, 3.0};
    Vec4 vecEq = {0.0, 1.0, 2.0, 3.0};
    Vec4 vecNotEq = {0.0, 1.0 + TEST_F32_ERR, 2.0, 3.0};

    assert(Vec4_IsEqual(vec, vecEq));
    assert(!Vec4_IsEqual(vec, vecNotEq));
}

void Test_Mat4Transpose(void) {
    Mat4 mat = {
        .x_row = {0.0, 0.1, 0.2, 0.3},
        .y_row = {1.0, 1.1, 1.2, 1.3},
        .z_row = {2.0, 2.1, 2.2, 2.3},
        .w_row = {3.0, 3.1, 3.2, 3.3},
    };
    Mat4 matT = {
        .x_row = {0.0, 1.0, 2.0, 3.0},
        .y_row = {0.1, 1.1, 2.1, 3.1},
        .z_row = {0.2, 1.2, 2.2, 3.2},
        .w_row = {0.3, 1.3, 2.3, 3.3},
    };

    Mat4 result = Mat4_Transpose(mat);
    assert(Mat4_IsEqual(result, matT));
}

void Test_Mat4IsEqual(void) {
    Mat4 mat = {
        .x_row = {0.0, 0.1, 0.2, 0.3},
        .y_row = {1.0, 1.1, 1.2, 1.3},
        .z_row = {2.0, 2.1, 2.2, 2.3},
        .w_row = {3.0, 3.1, 3.2, 3.3},
    };
    Mat4 matEq = {
        .x_row = {0.0, 0.1, 0.2, 0.3},
        .y_row = {1.0, 1.1, 1.2, 1.3},
        .z_row = {2.0, 2.1, 2.2, 2.3},
        .w_row = {3.0, 3.1, 3.2, 3.3},
    };
    Mat4 matNotEq = {
        .x_row = {0.0, 0.1, 0.2, 0.3},
        .y_row = {1.0, 1.1 + TEST_F32_ERR, 1.2, 1.3},
        .z_row = {2.0, 2.1, 2.2, 2.3},
        .w_row = {3.0, 3.1, 3.2, 3.3},
    };

    assert(Mat4_IsEqual(mat, matEq));
    assert(!Mat4_IsEqual(mat, matNotEq));
}

#endif /* TESTS_H */
