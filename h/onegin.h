#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

long fileSize(FILE *file, int* error);
int readFile(FILE* fp, char** s);
int getNLines(const char* s);
char** getLines(char* text, int n_lines);
void writeLines(FILE* fp, const char** lines, int n_lines);
int myStrcmp(const void* p1, const void* p2);