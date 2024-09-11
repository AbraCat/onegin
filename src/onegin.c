#include <errno.h>

#include <onegin.h>

long fileSize(FILE *file, int* error)
{
    assert(file != NULL);

    if (fseek(file, 0L, SEEK_END))
    {
        *error = errno;
        return 0;
    }
    long siz = ftell(file);
    if (siz == -1L)
    {
        *error = errno;
        return 0;
    }
    if (fseek(file, 0L, SEEK_SET))
    {
        *error = errno;
        return 0;
    }
    *error = 0;
    return siz;
}
int readFile(FILE* fp, struct TextData* td)
{
    assert(fp != NULL && td != NULL);

    int error = 0;
    long siz = fileSize(fp, &error);
    if (error == 1)
    {
        td->buf = NULL;
        return error;
    }
    td->buf = calloc(siz + 2, sizeof(char));
    if (td->buf == NULL)
        return errno;
    int n_read = fread(td->buf, sizeof(char), siz, fp);
    if (ferror(fp))
    {
        td->buf = NULL;
        return EIO;
    }
    if (td->buf[n_read - 1] != '\n')
        td->buf[n_read++] = '\n';
    td->buf[n_read] = '\0';
    return 0;
}
void getNLines(struct TextData *td)
{
    // assert(s != NULL && n_lines != NULL);

    // int i = 0;
    // for (int c = 0; (c = getc(fp)) != EOF; ++i)
    // {
    //     s[i] = c;
    //     if (s[i] == '\n')
    //     {
    //         ++*n_lines;
    //         s[i] = '\0';
    //     }
    // }
    // if (s[i - 1] != '\0')
    // {
    //     ++*n_lines;
    //     s[i] = '\0';
    // }
    // return s;

    assert(td != NULL);

    td->n_lines = 0;
    char* s = td->buf;
    for (; *s != '\0'; ++s)
    {
        if (*s == '\n')
            td->n_lines++;
    }
}
void getLines(struct TextData *td)
{
    assert(td != NULL);

    td->text = calloc(td->n_lines, sizeof(struct String));
    if (td->text == NULL)
        return;
    char *s = td->buf, *l = td->buf - 1;
    struct String str = {s, 0};
    td->text[0] = str;
    int new_str = 1;
    for (int i = 0; i != td->n_lines; ++s)
    {
        if (new_str)
        {
            //printf("new_str: i = %d, j = %d\n", i, j);
            str.s = s;
            new_str = 0;
        }
        if (*s == '\n')
        {
            //printf("new_str: i = %d, j = %d\n", i, j);
            str.len = s - l;
            l = s;
            td->text[i++] = str;
            new_str = 1;
        }
    }
}
void getText(struct TextData *td)
{
    getNLines(td);
    getLines(td);
}
void writeLines(FILE* fp, struct TextData* td)
{
    assert(fp != NULL && td != NULL);
    
    char* buf = calloc(1000, sizeof(char));
    for (int i = 0; i < td->n_lines; ++i)
    {
        sscanf(td->text[i].s, "%[^\n]s", buf);
        //fprintf(fp, "%s %d\n", td->text[i].s, td->text[i].len);
        fprintf(fp, "%s %d\n", buf, td->text[i].len);
    }
    free(buf);
}
// int myStrcmp(const void* p1, const void* p2)
// {
//     char **s1 = (char**)p1, **s2 = (char**)p2;

//     assert(s1 != NULL && s2 != NULL && *s1 != NULL && *s2 != NULL);

//     return strcmp(*s1, *s2);
// }
int myStrcmp(const void* p1, const void* p2)
{
    assert(p1 != NULL && p2 != NULL);

    struct String *sl = p1, *sr = p2;
    char *l = sl->s, *r = sr->s;
    for(; *l != '\n' && *l == *r; ++l, ++r);
    return *l - *r;
}
int myStrcmpR(const void* p1, const void* p2)
{
    assert(p1 != NULL && p2 != NULL);

    struct String *sl = p1, *sr = p2;
    char *l = sl->s + sl->len - 1, *r = sr->s + sr->len - 1;
    for(; l != sl->s - 1 && r != sr->s - 1 && *l == *r; --l, --r);
    if (l == sl->s - 1) l += sl->len + 1;
    if (r == sr->s - 1) r += sr->len + 1;
    return *l - *r;
}
void freeText(struct TextData* td)
{
    free(td->buf);
    free(td->text);
}