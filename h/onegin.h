#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>

#include <sort.h>

struct String
{
    char *str;
    int len;
};

struct TextData
{
    char* buf;
    struct String* text;
    int n_lines, maxlen;
};

int fileSize(FILE *file, long *siz);
int readFile(FILE* file, char** bufptr);
void getNLines(char* buf, int* n_lines);
int getLines(char* buf, int* n_lines, struct String** textptr, int* maxlen);
void writeLines(FILE* file, struct String* text, int n_lines);
void sortAndWrite(FILE* file, struct TextData* data, voidcmp_f cmp);
int myStrcmp(const struct String* str1, const struct String* str2);
int myStrcmpR(const struct String* str1, const struct String* str2);
void freeText(struct TextData* data);

#endif //ONEGIN_H