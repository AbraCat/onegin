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
int readFile(FILE* fp, char** s)
{
    assert(fp != NULL && s != NULL);

    int error = 0;
    long siz = fileSize(fp, &error);
    if (error == 1)
    {
        *s = NULL;
        return error;
    }
    *s = calloc(siz, sizeof(char));
    if (*s == NULL)
        return errno;
    fread(*s, sizeof(char), siz, fp);
    if (ferror(fp))
    {
        *s = NULL;
        return EIO;
    }
    return 0;
}
int getNLines(const char* s)
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

    assert(s != NULL);

    int n_lines = 0;
    for (; *s != '\0'; ++s)
    {
        if (*s == '\n')
            ++n_lines;
    }
    if (s[-1] != '\n')
        ++n_lines;
    return n_lines;
}
char** getLines(char* text, int n_lines)
{
    assert(text != NULL);

    char** lines = calloc(n_lines, sizeof(char*));
    if (lines == NULL)
        return NULL;
    lines[0] = text++;
    for (int i = 1; i != n_lines; ++text)
    {
        if (text[-1] == '\n')
        {
            text[-1] = '\0';
            lines[i++] = text;
        }
    }
    return lines;
}
void writeLines(FILE* fp, const char** lines, int n_lines)
{
    assert(fp != NULL && lines != NULL);
    
    for (int i = 0; i != n_lines; ++i)
        fprintf(fp, "%s\n", lines[i]);
}
int myStrcmp(const void* p1, const void* p2)
{
    char **s1 = (char**)p1, **s2 = (char**)p2;

    assert(s1 != NULL && s2 != NULL && *s1 != NULL && *s2 != NULL);

    return strcmp(*s1, *s2);
}