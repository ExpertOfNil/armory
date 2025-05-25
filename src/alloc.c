#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "alloc.h"

bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

size_t calc_padding_with_header(
    uintptr_t ptr, uintptr_t alignment, size_t header_size
) {
    assert(is_power_of_two(alignment));
    uintptr_t modulo = ptr & (alignment - 1);
    uintptr_t padding = 0;
    if (modulo != 0) {
        padding = alignment - modulo;
    }
    uintptr_t needed_space = (uintptr_t)header_size;
    if (padding < needed_space) {
        needed_space -= padding;
        if ((needed_space & (alignment - 1)) != 0) {
            padding += alignment * (1 + (needed_space / alignment));
        } else {
            padding += alignment * (needed_space / alignment);
        }
    }
    return (size_t)padding;
}

void Stack_Init(Stack* arena, void* buf, size_t capacity) {
    arena->buffer = (uint8_t*)buf;
    arena->capacity = capacity;
    arena->offset = 0;
}

void* Stack_AllocAlign(Stack* arena, size_t size, size_t alignment) {
    assert(is_power_of_two(alignment));
    if (alignment > 128) {
        alignment = 128;
    }
    size_t header_size = sizeof(StackHeader);
    uintptr_t curr_addr = (uintptr_t)arena->buffer + (uintptr_t)arena->offset;
    size_t padding =
        calc_padding_with_header(curr_addr, alignment, header_size);
    if (arena->offset + size + padding >= arena->capacity) {
        fprintf(stderr, "Out of memory.");
        return NULL;
    }
    uintptr_t next_addr = curr_addr + (uintptr_t)padding + (uintptr_t)size;
    StackHeader* header = (StackHeader*)(next_addr - header_size);
    header->prev_offset = arena->offset;
    header->padding = padding - header_size;
    arena->offset += padding + size;
    return memset((void*)curr_addr, 0, size);
}

void Stack_Pop(Stack* arena) {
    if (arena->offset == 0) {
        return;
    }
    uintptr_t curr_addr = (uintptr_t)arena->buffer + (uintptr_t)arena->offset;
    StackHeader* header = (StackHeader*)(curr_addr - sizeof(StackHeader));
    arena->offset = header->prev_offset;
    uintptr_t prev_addr =
        (uintptr_t)arena->buffer + (uintptr_t)header->prev_offset;
    memset((void*)prev_addr, 0, curr_addr - prev_addr);
}

