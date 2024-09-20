#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

#include <onegin.h>

int fileSize(FILE *file, long *siz)
{
    assert(file != NULL && siz != NULL);

    if (fseek(file, 0L, SEEK_END))
    {
        *siz = -1L;
        return errno;
    }

    *siz = ftell(file);

    if (*siz == -1L)
        return errno;
    if (fseek(file, 0L, SEEK_SET))
        return errno;

    return 0;
}

int readFile(FILE* file, char** bufptr)
{
    assert(file != NULL && bufptr != NULL);

    long siz = 0;
    int error = fileSize(file, &siz);

    if (error != 0)
    {
        *bufptr = NULL;
        return error;
    }

    char* buf = *bufptr = (char*)calloc(siz + 2, sizeof(char));
    if (buf == NULL)
        return errno;

    int n_read = fread(buf, sizeof(char), siz, file);
    if (ferror(file))
    {
        free(buf);
        *bufptr = NULL;
        return EIO;
    }

    if (buf[n_read - 1] != '\n')
        buf[n_read++] = '\n';
    buf[n_read] = '\0';
    return 0;
}

void getNLines(char* buf, int* n_lines)
{
    assert(buf != NULL);

    *n_lines = 0;
    for (; *buf != '\0'; ++buf)
    {
        if (*buf == '\n')
            (*n_lines)++;
    }
}

int getLines(char* buf, int* n_lines, struct String** textptr, int* maxlen)
{
    assert(buf != NULL &&n_lines != NULL && textptr != NULL && maxlen != NULL);

    getNLines(buf, n_lines);

    struct String* text = *textptr = (struct String*)calloc(*n_lines, sizeof(struct String));
    if (text == NULL)
        return errno;

    struct String str = {buf, 0};
    *maxlen = 0;

    for (int i = 0; i != *n_lines; ++buf)
    {
        if (*buf == '\n')
        {
            str.len = buf - str.str;
            *maxlen = str.len > *maxlen ? str.len : *maxlen;
            text[i++] = str;
            str.str = buf + 1;
        }
    }
    return 0;
}

void writeLines(FILE* file, struct String* text, int n_lines)
{
    assert(file != NULL && text != NULL);

    for (int i = 0; i < n_lines; ++i)
    {
        if (text[i].len == 0) continue;
        fprintf(file, "%.*s\n", text[i].len, text[i].str);
    }
}

void sortAndWrite(FILE* file, struct TextData* data, voidcmp_f cmp)
{
    assert(file != NULL && data != NULL);

    myQsort(data->text, data->n_lines, sizeof(struct String), cmp);
    writeLines(file, data->text, data->n_lines);
    fprintf(file, "\n%s\n\n", "--------------------------------");
}

int myStrcmp(const struct String* str1, const struct String* str2)
{
    assert(str1 != NULL && str2 != NULL);

    char *lft = str1->str, *rgt = str2->str;

    while (*lft != '\n' && !isalnum(*lft)) ++lft;
    while (*rgt != '\n' && !isalnum(*rgt)) ++rgt;

    while (*lft != '\n' && *lft == *rgt)
    {
        ++lft;
        ++rgt;
        
        while (*lft != '\n' && !isalnum(*lft)) ++lft;
        while (*rgt != '\n' && !isalnum(*rgt)) ++rgt;
    }
    return (*lft == '\n' ? '\0' : *lft) - (*rgt == '\n' ? '\0' : *rgt);
}

int myStrcmpR(const struct String* str1, const struct String* str2)
{
    assert(str1 != NULL && str2 != NULL);

    char *lftend = str1->str - 1, *rghend = str2->str - 1;
    char *lft = lftend + str1->len, *rgt = rghend + str2->len;

    while (lft != lftend && !isalnum(*lft)) --lft;
    while (rgt != rghend && !isalnum(*rgt)) --rgt;

    while (lft != lftend && rgt != rghend && *lft == *rgt)
    {
        --lft;
        --rgt;

        while (lft != lftend && !isalnum(*lft)) --lft;
        while (rgt != rghend && !isalnum(*rgt)) --rgt;
    }
    
    return (lft == lftend ? '\0' : *lft) - (rgt == rghend ? '\0' : *rgt);
}

void freeText(struct TextData* data)
{
    assert(data != NULL);
    
    free(data->buf);
    free(data->text);
}