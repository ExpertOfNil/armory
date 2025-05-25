#ifndef CSV_H
#define CSV_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {
    EXIT_OK,
    EXIT_ERR,
} EXIT_STATUS;

typedef struct String String;
typedef struct StringView StringView;
typedef struct VecString VecString;

/* A non-null terminated dynamic array of char */
struct String {
    char* items;
    size_t size;
    size_t capacity;
};
EXIT_STATUS String_Resize(String* str, size_t capacity);
EXIT_STATUS String_Append(String* str, const char* other, size_t other_size);
EXIT_STATUS String_ReadFile(String* str, const char* filepath);
EXIT_STATUS String_Split(String* str, VecString* pieces, const char delim);
void String_Reset(String* str);
bool String_IsEmpty(String* str);
void String_Free(String* str);

struct StringView {
    char* start;
    size_t length;
};

/* A dynamic array of String */
struct VecString {
    String* items;
    size_t size;
    size_t capacity;
};
String* VecString_Alloc(VecString* vec);
void VecString_Reset(VecString* vec);
bool VecString_IsEmpty(VecString* vec);
void VecString_Free(VecString* vec);

//EXIT_STATUS get_poses(std::string* content, std::vector<Pose>* poses);

#endif /* CSV_H */
