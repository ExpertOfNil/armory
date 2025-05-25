#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_ALIGNMENT 8
#define StackAlloc(arena, type, n) \
    (type*)StackAllocAlign(arena, sizeof(type) * n, DEFAULT_ALIGNMENT)

bool is_power_of_two(uintptr_t x);

size_t calc_padding_with_header(
    uintptr_t ptr, uintptr_t alignment, size_t header_size
);

typedef struct StackHeader {
    size_t prev_offset;
    size_t padding;
} StackHeader;

typedef struct Stack {
    uint8_t* buffer;
    size_t offset;
    size_t capacity;
} Stack;

void Stack_Init(Stack* arena, void* buf, size_t capacity);
void* Stack_AllocAlign(Stack* arena, size_t size, size_t alignment);
void Stack_Pop(Stack* arena);
