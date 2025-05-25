#include "csv.h"

#include <assert.h>

EXIT_STATUS String_Resize(String* str, size_t capacity) {
    if (str->capacity < capacity) {
        str->items = (char*)realloc(str->items, capacity);
        if (str->items == NULL) {
            fprintf(stderr, "Out of memory\n");
            return EXIT_ERR;
        }
        memset(&str->items[str->size], 0, capacity - str->capacity);
        str->capacity = capacity;
    }
    return EXIT_OK;
}

EXIT_STATUS String_Append(String* str, const char* other, size_t other_size) {
    // Handle uninitialized
    if (str->capacity <= 0) {
        if (String_Resize(str, 64) != EXIT_OK) {
            return EXIT_ERR;
        }
        str->size = 0;
    }
    while (str->capacity <= str->size + other_size) {
        if (String_Resize(str, str->capacity * 2) != EXIT_OK) {
            return EXIT_ERR;
        }
    }
    memcpy(&str->items[str->size], other, other_size);
    str->size += other_size;
    return EXIT_OK;
}

EXIT_STATUS String_ReadFile(String* content, const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_ERR;
    }
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0) {
        fprintf(stderr, "Failed to read file.");
        return EXIT_ERR;
    }
    if (file_size == 0) {
        fprintf(stderr, "File was empty.");
        return EXIT_ERR;
    }
    lseek(fd, 0, SEEK_SET);

    String_Resize(content, file_size);
    int bytes_read = read(fd, content->items, file_size);
    if(close(fd) != 0) {
        perror("close");
        return EXIT_ERR;
    }
    content->size = file_size;
    if (bytes_read != file_size) {
        fprintf(stderr, "Warning: did not read entire file");
        return EXIT_ERR;
    }
    return EXIT_OK;
}

EXIT_STATUS String_Split(String* str, VecString* pieces, const char delim) {
    assert(str != NULL && "Cannot split a NULL String");
    String* piece = NULL;
    for (size_t i = 0; i < str->size; ++i) {
        char c = str->items[i];
        if (c == delim) {
            // We don't care about empty lines
            if (piece != NULL && !String_IsEmpty(piece)) {
                // Allocate a new string
                piece = VecString_Alloc(pieces);
                if (piece == NULL) return EXIT_ERR;
            }
        } else {
            // Only allocate when needed
            if (piece == NULL) {
                piece = VecString_Alloc(pieces);
                if (piece == NULL) return EXIT_ERR;
            }
            String_Append(piece, &c, sizeof(char));
        }
    }
    return EXIT_OK;
}

void String_Reset(String* str) {
    if (str == NULL || str->items == NULL) {
        return;
    }
    str->size = 0;
    memset(str->items, 0, str->capacity);
    return;
}

bool String_IsEmpty(String* str) {
    return str->items == NULL || str->size == 0;
}

void String_Free(String* str) {
    if (str == NULL || str->items == NULL) {
        return;
    }
    free(str->items);
    str->items = NULL;
    str->capacity = 0;
    str->size = 0;
    return;
}

String* VecString_Alloc(VecString* vec) {
    assert(vec != 0 && "Cannot allocate from NULL VecString");
    // Handle uninitialized
    if (vec->capacity <= 0) {
        vec->capacity = 64;
        vec->size = 0;
        vec->items = (String*)calloc(1, vec->capacity * sizeof(String));
        if (vec->items == NULL) {
            fprintf(stderr, "Out of memory\n");
            return NULL;
        }
    }
    if (vec->capacity <= vec->size + 1) {
        vec->capacity *= 2;
        vec->items =
            (String*)realloc(vec->items, vec->capacity * sizeof(String));
        if (vec->items == NULL) {
            fprintf(stderr, "Out of memory\n");
            return NULL;
        }
    }
    String* new_str = &vec->items[vec->size];
    new_str->items = NULL;
    new_str->capacity = 0;
    new_str->size = 0;
    vec->size += 1;
    return new_str;
}

void VecString_Reset(VecString* vec) {
    if (vec == NULL || vec->items == NULL) {
        return;
    }
    for (size_t i = 0; i < vec->size; ++i) {
        String_Reset(&vec->items[i]);
    }
    vec->size = 0;
    return;
}

bool VecString_IsEmpty(VecString* vec) {
    return vec->items == NULL || vec->size == 0;
}

void VecString_Free(VecString* vec) {
    if (vec == NULL || vec->items == NULL) {
        return;
    }
    for (size_t i = 0; i < vec->size; ++i) {
        String_Free(&vec->items[i]);
    }
    free(vec->items);
    vec->items = NULL;
    vec->capacity = 0;
    vec->size = 0;
    return;
}

/*
EXIT_STATUS get_poses(std::string* content, std::vector<Pose>* poses) {
    std::vector<std::string> lines;
    get_lines(content, &lines);
    if (lines.size() == 0) {
        fprintf(stderr, "Got 0 lines\n");
        return EXIT_ERR;
    }
    printf("Got %ld lines\n", lines.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        std::vector<std::string> values;
        get_values(&lines[i], &values);
        if (values.size() != 8) {
            fprintf(stderr, "Expected 8 values.  Got %ld.\n", values.size());
            return EXIT_ERR;
        }
        Pose pose = {.id = (size_t)atoi(values[0].c_str()),
                     .replicate_id = (size_t)atoi(values[1].c_str()),
                     .rvec =
                         {
                             .x = (float)atof(values[2].c_str()),
                             .y = (float)atof(values[3].c_str()),
                             .z = (float)atof(values[4].c_str()),
                         },
                     .tvec = {
                         .x = (float)atof(values[5].c_str()),
                         .y = (float)atof(values[6].c_str()),
                         .z = (float)atof(values[7].c_str()),
                     }};
        poses->push_back(pose);
    }
    return EXIT_OK;
}
*/
