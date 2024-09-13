#include <errno.h>

#include <onegin.h>

int ispunc(char c)
{
    return (c < '0' || c > '9') && (c < 'A' || c > 'Z') && (c < 'a' || c > 'z');
}
long fileSize(FILE *file, int* error)
{
    assert(file != NULL && error != NULL);

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
    assert(td != NULL);

    td->n_lines = 0;
    char* s = td->buf;
    for (; *s != '\0'; ++s)
    {
        if (*s == '\n')
            td->n_lines++;
    }
}
int getLines(struct TextData *td)
{
    assert(td != NULL);

    td->text = calloc(td->n_lines, sizeof(struct String));
    if (td->text == NULL)
        return 1;
    char *s = td->buf;
    struct String str = {s, 0};
    td->maxlen = 0;
    for (int i = 0; i != td->n_lines; ++s)
    {
        if (*s == '\n')
        {
            str.len = s - str.s;
            td->maxlen = str.len > td->maxlen ? str.len : td->maxlen;
            td->text[i++] = str;
            str.s = s + 1;
        }
    }
    return 0;
}
int getText(struct TextData *td)
{
    assert(td != NULL);

    getNLines(td);
    return getLines(td);
}
void bubbleSort(void* ptr, size_t count, size_t size, voidcmp_t cmp)
{
    assert(ptr != NULL);

    char *a = ptr, tmp = 0;
    for (int i = count - 2; i >= 0; --i)
    {
        for (int j = 0; j <= i; ++j)
        {
            if (cmp(a + j * size, a + (j + 1) * size) > 0)
            {
                for (int k = 0; k < size; ++k)
                {
                    tmp = a[j * size + k];
                    a[j * size + k] = a[(j + 1) * size + k];
                    a[(j + 1) * size + k] = tmp;
                }
            }
        }
    }
}
void myQsort(void* ptr, size_t count, size_t size, voidcmp_t cmp)
{
    assert(ptr != NULL);

    if (count == 1 || count == 0) return;

    char tmp = 0;
    char *pt = ptr;
    char *l = ptr, *r = (char*)pt + (count - 1) * size, *piv = pt + (count - 1) * size;

    while (l < r && cmp(l, piv) < 0)
        l += size;
    while (l < r && cmp(r, piv) > 0)
        r -= size;

    while (l < r)
    {
        for (int i = 0; i < size; ++i)
        {
            tmp = l[i];
            l[i] = r[i];
            r[i] = tmp;
        }

        l += size;
        r -= size;

        while (l < r && cmp(l, piv) < 0)
            l += size;
        while (l < r && cmp(r, piv) > 0)
            r -= size;
    }
    if (l == pt + (count - 1) * size)
    {
        myQsort(pt, count - 1, size, cmp);
        myQsort(pt + (count - 1) * size, 1, size, cmp);
        return;
    }
    if (l > r)
    {
        myQsort(pt, (l - pt) / size, size, cmp);
        myQsort(l, count - (l - pt) / size, size, cmp);
        return;
    }
    int inc_l = cmp(l, piv) <= 0 ? 1 : 0;
    myQsort(pt, (l - pt) / size + inc_l, size, cmp);
    myQsort(r + inc_l * size, count - (r - pt) / size - inc_l, size, cmp);
}
void writeLines(FILE* fp, struct TextData* td)
{
    assert(fp != NULL && td != NULL);

    char* buf = calloc(td->maxlen, sizeof(char));
    for (int i = 0; i < td->n_lines; ++i)
    {
        if (td->text[i].len == 0) continue;
        sscanf(td->text[i].s, "%[^\n]s", buf);
        fprintf(fp, "%s\n", buf);
    }
    free(buf);
}
// int myStrcmp(const void* p1, const void* p2)
// {
//     char **s1 = (char**)p1, **s2 = (char**)p2;

//     assert(s1 != NULL && s2 != NULL && *s1 != NULL && *s2 != NULL);

//     return strcmp(*s1, *s2);
// }
int myStrcmp(const struct String* s1, const struct String* s2)
{
    assert(s1 != NULL && s2 != NULL);

    char *l = s1->s, *r = s2->s;
    while (*l != '\n' && ispunc(*l)) ++l;
    while (*r != '\n' && ispunc(*r)) ++r;
    while (*l != '\n' && *l == *r)
    {
        ++l;
        ++r;
        while (*l != '\n' && ispunc(*l)) ++l;
        while (*r != '\n' && ispunc(*r)) ++r;
    }
    return (*l == '\n' ? '\0' : *l) - (*r == '\n' ? '\0' : *r);
}
int myStrcmpR(const struct String* s1, const struct String* s2)
{
    assert(s1 != NULL && s2 != NULL);

    char *le = s1->s - 1, *re = s2->s - 1;
    char *l = le + s1->len, *r = re + s2->len;
    while (l != le && ispunc(*l)) --l;
    while (r != re && ispunc(*r)) --r;
    while (l != le && r != re && *l == *r)
    {
        --l;
        --r;
        while (l != le && ispunc(*l)) --l;
        while (r != re && ispunc(*r)) --r;
    }
    return (l == le ? '\0' : *l) - (r == re ? '\0' : *r);
}
void freeText(struct TextData* td)
{
    assert(td != NULL);
    
    free(td->buf);
    free(td->text);
}