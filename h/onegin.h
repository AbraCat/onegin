#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct String
{
    char *s;
    int len;
};

struct TextData
{
    char* buf;
    struct String* text;
    int n_lines, maxlen;
};

typedef int (*voidcmp_t)(const void*, const void*);
typedef int (*strcmp_t)(const struct String, const struct String);

int isletter(char c);
long fileSize(FILE *file, int* error);
int readFile(FILE* fp, struct TextData* td);
void getNLines(struct TextData *td);
void getLines(struct TextData *td);
void getText(struct TextData *td);
void myQsort(void* ptr, size_t count, size_t size, voidcmp_t cmp);
void writeLines(FILE* fp, struct TextData* td);
int myStrcmp(const struct String* s1, const struct String* s2);
int myStrcmpR(const struct String* s1, const struct String* s2);
void freeText(struct TextData* td);