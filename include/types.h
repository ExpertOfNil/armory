#ifndef TYPES_H
#define TYPES_H

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VEC_MAX_WRITE 64
#define EPSILON 1e-9

typedef enum {
    SUCCESS,
    FAILURE,
} RETURN_STATUS;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ssize_t isize;

typedef float f32;
typedef double f64;

typedef struct String String;
typedef struct Vec3 Vec3;
typedef struct Vec4 Vec4;
typedef struct Mat3 Mat3;
typedef struct Mat4 Mat4;

struct String {
    char* begin;
    size_t size;
    size_t capacity;
};

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct Mat3 {
    Vec3 x_row;
    Vec3 y_row;
    Vec3 z_row;
};

struct Mat4 {
    Vec4 x_row;
    Vec4 y_row;
    Vec4 z_row;
    Vec4 w_row;
};

RETURN_STATUS String_Append(String* str, char* start, size_t len);
RETURN_STATUS String_AppendStr(String* str, const char* input_str);
RETURN_STATUS String_AppendMany(String* str, ...);
RETURN_STATUS String_AppendVec3(String* str, Vec3 vec);
RETURN_STATUS String_AppendVec4(String* str, Vec4 vec);
RETURN_STATUS String_AppendMat4(String* str, Mat4 mat);
RETURN_STATUS String_CheckCapacity(String* str, size_t len);

f32 F32_Abs(f32 value);

bool Vec3_IsEqual(Vec3 a, Vec3 b);
Vec3 Vec3_Add(Vec3 a, Vec3 b);
Vec3 Vec3_Sub(Vec3 a, Vec3 b);
f32 Vec3_Dot(Vec3 a, Vec3 b);
Vec3 Vec3_Cross(Vec3 a, Vec3 b);
Vec3 Vec3_Scale(Vec3 vec, f32 scale);
f32 Vec3_Mag(Vec3 vec);
Vec3 Vec3_Normalize(Vec3 vec);
Vec3 Vec3_Rotate(Vec3 vec, Mat3 rot);
Vec3 Vec3_Project(Vec3 src, Vec3 dst);
Vec3 Vec3_Reject(Vec3 src, Vec3 dst);

bool Vec4_IsEqual(Vec4 a, Vec4 b);
f32 Vec4_Dot(Vec4 a, Vec4 b);
Vec4 Vec4_Scale(Vec4 vec, f32 scale);
f32 Vec4_Mag(Vec4 vec);
Vec4 Vec4_Normalize(Vec4 vec);

bool Mat3_IsEqual(Mat3 a, Mat3 b);
Mat3 Mat3_Orthonormalize(Mat3 mat);

bool Mat4_IsEqual(Mat4 a, Mat4 b);
Mat4 Mat4_Transpose(Mat4 mat);

RETURN_STATUS String_Append(String* str, char* start, size_t len) {
    if (String_CheckCapacity(str, len) != SUCCESS) {
        return FAILURE;
    }
    memcpy(&str->begin[str->size], start, len);
    str->size += len;
    return SUCCESS;
}

RETURN_STATUS String_AppendStr(String* str, const char* input_str) {
    int str_len = strlen(input_str);
    if (String_CheckCapacity(str, str_len) != SUCCESS) {
        return FAILURE;
    }
    memcpy(&str->begin[str->size], input_str, str_len);
    str->size += str_len;
    return SUCCESS;
}

RETURN_STATUS String_AppendMany(String* str, ...) {
    va_list args;
    va_start(args, str);
    const char* append_str;
    while ((append_str = va_arg(args, const char*)) != NULL) {
        if (String_AppendStr(str, append_str) != SUCCESS) {
            return FAILURE;
        }
    }
    return SUCCESS;
}

RETURN_STATUS String_AppendVec3(String* str, Vec3 vec) {
    char local_buf[VEC_MAX_WRITE] = {0};
    size_t written = snprintf(
        local_buf, VEC_MAX_WRITE, "[%9.4f, %9.4f, %9.4f]", vec.x, vec.y, vec.z
    );
    if (String_Append(str, local_buf, written) != SUCCESS) {
        return FAILURE;
    }
    return SUCCESS;
}

RETURN_STATUS String_AppendVec4(String* str, Vec4 vec) {
    char local_buf[VEC_MAX_WRITE] = {0};
    int written = snprintf(
        local_buf,
        VEC_MAX_WRITE,
        "[%9.4f, %9.4f, %9.4f, %9.4f]",
        vec.x,
        vec.y,
        vec.z,
        vec.w
    );
    if (String_Append(str, local_buf, written) != SUCCESS) {
        return FAILURE;
    }
    return SUCCESS;
}

RETURN_STATUS String_AppendMat4(String* str, Mat4 mat) {
    if (String_AppendStr(str, "[\n\t") != SUCCESS) {
        return FAILURE;
    }
    Vec4* row_ptr = (Vec4*)&mat;
    size_t n_rows = 4;
    for (size_t i = 0; i < n_rows; ++i) {
        if (String_AppendVec4(str, row_ptr[i]) != SUCCESS) {
            return FAILURE;
        }
        if (String_AppendStr(str, "\n") != SUCCESS) {
            return FAILURE;
        }
        if (i != n_rows - 1) {
            if (String_AppendStr(str, "\t") != SUCCESS) {
                return FAILURE;
            }
        }
    }
    if (String_AppendStr(str, "]\n") != SUCCESS) {
        return FAILURE;
    }
    return SUCCESS;
}

RETURN_STATUS String_CheckCapacity(String* str, size_t len) {
    while (str->size + len > str->capacity) {
        int new_capacity = str->capacity * 2;
        str->begin = (char*)realloc(str->begin, new_capacity);
        if (NULL == str->begin) {
            fprintf(stderr, "ERROR: failed to reallocate String");
            return FAILURE;
        }
        str->capacity = new_capacity;
    }
    return SUCCESS;
}

f32 F32_Abs(f32 value) {
    u32* ptr = (u32*)&value;
    return (*ptr & 0x7FFFFFFF);
}

bool Vec3_IsEqual(Vec3 a, Vec3 b) {
    f32* col_ptr_a = (f32*)&a;
    f32* col_ptr_b = (f32*)&b;
    for (size_t col = 0; col < 3; ++col) {
        f32 delta = col_ptr_a[col] - col_ptr_b[col];
        if (F32_Abs(delta) > EPSILON) {
            return false;
        }
    }
    return true;
}

Vec3 Vec3_Add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

Vec3 Vec3_Sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

f32 Vec3_Dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3 Vec3_Cross(Vec3 a, Vec3 b) {
    f32 i = (a.y * b.z) - (a.z * b.y);
    f32 j = (a.x * b.z) - (a.z * b.x);
    f32 k = (a.x * b.y) - (a.y * b.x);
    return (Vec3){.x = i, .y = -j, .z = k};
}

Vec3 Vec3_Scale(Vec3 vec, f32 scale) {
    return (Vec3){
        .x = vec.x * scale,
        .y = vec.y * scale,
        .z = vec.z * scale,
    };
}

f32 Vec3_Mag(Vec3 vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vec3 Vec3_Normalize(Vec3 vec) { return Vec3_Scale(vec, 1.0f / Vec3_Mag(vec)); }

Vec3 Vec3_Rotate(Vec3 vec, Mat3 rot) {
    return (Vec3){
        .x = Vec3_Dot(rot.x_row, vec),
        .y = Vec3_Dot(rot.y_row, vec),
        .z = Vec3_Dot(rot.z_row, vec),
    };
}

Vec3 Vec3_Project(Vec3 src, Vec3 dst) {
    f32 dst_mag = Vec3_Mag(dst);
    Mat3 mat = {
        .x_row =
            {
                .x = dst.x * dst.x,
                .y = dst.x * dst.y,
                .z = dst.x * dst.z,
            },
        .y_row =
            {
                .x = dst.y * dst.x,
                .y = dst.y * dst.y,
                .z = dst.y * dst.z,
            },
        .z_row =
            {
                .x = dst.z * dst.x,
                .y = dst.z * dst.y,
                .z = dst.z * dst.z,
            },
    };
    return Vec3_Scale(Vec3_Rotate(src, mat), 1.0f / (dst_mag * dst_mag));
}

Vec3 Vec3_Reject(Vec3 src, Vec3 dst) {
    return Vec3_Sub(src, Vec3_Project(src, dst));
}

bool Vec4_IsEqual(Vec4 a, Vec4 b) {
    f32* col_ptr_a = (f32*)&a;
    f32* col_ptr_b = (f32*)&b;
    for (size_t col = 0; col < 4; ++col) {
        f32 delta = col_ptr_a[col] - col_ptr_b[col];
        if (F32_Abs(delta) > EPSILON) {
            return false;
        }
    }
    return true;
}

f32 Vec4_Dot(Vec4 a, Vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec4 Vec4_Scale(Vec4 vec, f32 scale) {
    return (Vec4){
        .x = vec.x * scale,
        .y = vec.y * scale,
        .z = vec.z * scale,
        .w = vec.w * scale,
    };
}

f32 Vec4_Mag(Vec4 vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

Vec4 Vec4_Normalize(Vec4 vec) { return Vec4_Scale(vec, 1.0f / Vec4_Mag(vec)); }

bool Mat3_IsEqual(Mat3 a, Mat3 b) {
    Vec4* row_ptr_a = (Vec4*)&a;
    Vec4* row_ptr_b = (Vec4*)&b;
    for (size_t row = 0; row < 3; ++row) {
        f32* col_ptr_a = (f32*)&row_ptr_a[row];
        f32* col_ptr_b = (f32*)&row_ptr_b[row];
        for (size_t col = 0; col < 3; ++col) {
            f32 delta = col_ptr_a[col] - col_ptr_b[col];
            if (F32_Abs(delta) > EPSILON) {
                return false;
            }
        }
    }
    return true;
}

Mat3 Mat3_Orthonormalize(Mat3 mat) {
    Vec3 vy = {.x = mat.x_row.y, .y = mat.y_row.y, .z = mat.z_row.y};
    Vec3 vz = {.x = mat.x_row.z, .y = mat.y_row.z, .z = mat.z_row.z};

    Vec3 ux = {.x = mat.x_row.x, .y = mat.y_row.x, .z = mat.z_row.x};
    Vec3 uy = Vec3_Sub(vy, Vec3_Project(vy, ux));
    Vec3 uz =
        Vec3_Sub(vz, Vec3_Sub(Vec3_Project(vz, ux), Vec3_Project(vz, uy)));
    ux = Vec3_Normalize(ux);
    uy = Vec3_Normalize(uy);
    uz = Vec3_Normalize(uz);
    return (Mat3){
        .x_row = {.x = ux.x, .y = uy.x, .z = uz.x},
        .y_row = {.x = ux.y, .y = uy.y, .z = uz.y},
        .z_row = {.x = ux.z, .y = uy.z, .z = uz.z},
    };
}

bool Mat4_IsEqual(Mat4 a, Mat4 b) {
    Vec4* row_ptr_a = (Vec4*)&a;
    Vec4* row_ptr_b = (Vec4*)&b;
    for (size_t row = 0; row < 4; ++row) {
        f32* col_ptr_a = (f32*)&row_ptr_a[row];
        f32* col_ptr_b = (f32*)&row_ptr_b[row];
        for (size_t col = 0; col < 4; ++col) {
            f32 delta = col_ptr_a[col] - col_ptr_b[col];
            if (F32_Abs(delta) > EPSILON) {
                return false;
            }
        }
    }
    return true;
}

Mat4 Mat4_Transpose(Mat4 mat) {
    return (Mat4){
        .x_row =
            {
                .x = mat.x_row.x,
                .y = mat.y_row.x,
                .z = mat.z_row.x,
                .w = mat.w_row.x,
            },
        .y_row =
            {
                .x = mat.x_row.y,
                .y = mat.y_row.y,
                .z = mat.z_row.y,
                .w = mat.w_row.y,
            },
        .z_row =
            {
                .x = mat.x_row.z,
                .y = mat.y_row.z,
                .z = mat.z_row.z,
                .w = mat.w_row.z,
            },
        .w_row =
            {
                .x = mat.x_row.w,
                .y = mat.y_row.w,
                .z = mat.z_row.w,
                .w = mat.w_row.w,
            },
    };
}

#endif /* TYPES_H */
