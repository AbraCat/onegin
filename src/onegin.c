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
    printf("readFile(): siz = %d, n_read = %d\n", siz, n_read);
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
    char* s = td->buf;
    struct String str = {s, 0};
    td->text[0] = str;
    int j = -1, new_str = 1;
    for (int i = 0; i != td->n_lines; ++s)
    {
        if (new_str)
        {
            str.s = s;
            new_str = 0;
        }
        if (*s == '\n')
        {
            *s = '\0'; // remove
            str.len = i - j;
            j = i;
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
    
    printf("writeLines():\n");
    printf("%p\n", td->text[0].s);
    for (int i = 0; i < td->n_lines; ++i)
        fprintf(fp, "%s\n", td->text[i].s);
}
int myStrcmp(const void* p1, const void* p2)
{
    char **s1 = (char**)p1, **s2 = (char**)p2;

    assert(s1 != NULL && s2 != NULL && *s1 != NULL && *s2 != NULL);

    return strcmp(*s1, *s2);
}