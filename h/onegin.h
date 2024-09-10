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
    int n_lines;
    int* lens;
};

long fileSize(FILE *file, int* error);
int readFile(FILE* fp, struct TextData* td);
void getNLines(struct TextData *td);
void getLines(struct TextData *td);
void getText(struct TextData *td);
void writeLines(FILE* fp, struct TextData* td);
int myStrcmp(const void* p1, const void* p2);