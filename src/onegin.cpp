#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include <onegin.h>

int ispunc(char chr)
{
    return (chr < '0' || chr > '9') && (chr < 'A' || chr > 'Z') && (chr < 'a' || chr > 'z');
}
int fileSize(FILE *file, unsigned long *siz)
{
    assert(file != NULL && siz != NULL);

    if (fseek(file, 0L, SEEK_END))
    {
        *siz = -1L;
        return errno;
    }
    *siz = ftell(file);
    if (*siz == -1L)
    {
        return errno;
    }
    if (fseek(file, 0L, SEEK_SET))
    {
        return errno;
    }
    return 0;
}
int readFile(FILE* file, struct TextData* data)
{
    if (file == NULL)
    {
        return ENOENT;
    }
    assert(file != NULL && data != NULL);

    unsigned long siz = 0;
    int error = fileSize(file, &siz);
    if (error != 0)
    {
        data->buf = NULL;
        return error;
    }
    data->buf = (char*)calloc(siz + 2, sizeof(char));
    if (data->buf == NULL)
        return errno;
    int n_read = fread(data->buf, sizeof(char), siz, file);
    if (ferror(file))
    {
        data->buf = NULL;
        return EIO;
    }
    if (data->buf[n_read - 1] != '\n')
        data->buf[n_read++] = '\n';
    data->buf[n_read] = '\0';
    return 0;
}
void getNLines(struct TextData *data)
{
    assert(data != NULL);

    data->n_lines = 0;
    char* str = data->buf;
    for (; *str != '\0'; ++str)
    {
        if (*str == '\n')
            data->n_lines++;
    }
}
int getLines(struct TextData *data)
{
    assert(data != NULL);

    data->text = (struct String*)calloc(data->n_lines, sizeof(struct String));
    if (data->text == NULL)
        return errno;
    char *buf = data->buf;
    struct String str = {buf, 0};
    data->maxlen = 0;
    for (int i = 0; i != data->n_lines; ++buf)
    {
        if (*buf == '\n')
        {
            str.len = buf - str.str;
            data->maxlen = str.len > data->maxlen ? str.len : data->maxlen;
            data->text[i++] = str;
            str.str = buf + 1;
        }
    }
    return 0;
}
int getText(struct TextData *data)
{
    assert(data != NULL);

    getNLines(data);
    return getLines(data);
}
void writeLines(FILE* file, struct TextData* data)
{
    assert(file != NULL && data != NULL);

    for (int i = 0; i < data->n_lines; ++i)
    {
        if (data->text[i].len == 0) continue;
        fprintf(file, "%.*s\n", data->text[i].len, data->text[i].str);
    }
}
int myStrcmp(const struct String* str1, const struct String* str2)
{
    assert(str1 != NULL && str2 != NULL);

    char *lft = str1->str, *rgt = str2->str;

    while (*lft != '\n' && ispunc(*lft)) ++lft;
    while (*rgt != '\n' && ispunc(*rgt)) ++rgt;

    while (*lft != '\n' && *lft == *rgt)
    {
        ++lft;
        ++rgt;
        
        while (*lft != '\n' && ispunc(*lft)) ++lft;
        while (*rgt != '\n' && ispunc(*rgt)) ++rgt;
    }
    return (*lft == '\n' ? '\0' : *lft) - (*rgt == '\n' ? '\0' : *rgt);
}
int myStrcmpR(const struct String* str1, const struct String* str2)
{
    assert(str1 != NULL && str2 != NULL);

    char *lfte = str1->str - 1, *rghe = str2->str - 1;
    char *lft = lfte + str1->len, *rgt = rghe + str2->len;
    while (lft != lfte && ispunc(*lft)) --lft;
    while (rgt != rghe && ispunc(*rgt)) --rgt;
    while (lft != lfte && rgt != rghe && *lft == *rgt)
    {
        --lft;
        --rgt;
        while (lft != lfte && ispunc(*lft)) --lft;
        while (rgt != rghe && ispunc(*rgt)) --rgt;
    }
    return (lft == lfte ? '\0' : *lft) - (rgt == rghe ? '\0' : *rgt);
}
void freeText(struct TextData* data)
{
    assert(data != NULL);
    
    free(data->buf);
    free(data->text);
}