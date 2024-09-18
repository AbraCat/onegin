#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>

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

int ispunc(char chr);
int fileSize(FILE *file, unsigned long *siz);
int readFile(FILE* file, struct TextData* data);
void getNLines(struct TextData *data);
int getLines(struct TextData *data);
int getText(struct TextData *data);
void writeLines(FILE* file, struct TextData* data);
int myStrcmp(const struct String* str1, const struct String* str2);
int myStrcmpR(const struct String* str1, const struct String* str2);
void freeText(struct TextData* data);

#endif //ONEGIN_H