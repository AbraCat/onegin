#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <onegin.h>

/*
fdsfd
plkiuh
bbb
wwwcz
rerwe
;
ccc
zxcdws
aaa
kmkmn
qazxswedcxde
*/

int main(int argc, char* argv[])
{
    const char *in_name = ".\\txt\\input.txt", *out_name = ".\\txt\\output.txt";
    FILE *fin = fopen(in_name, "r"), *fout = fopen(out_name, "w");

    if (fin == NULL || fout == NULL)
    {
        perror("Error");
        if (fin != NULL)
            fclose(fin);
        if (fout != NULL)
            fclose(fout);
        return 1;
    }
    
    struct TextData data;

    printf("aaa\n");

    int error = readFile(fin, &data);
    if (error)
    {
        printf("Error: %s", strerror(error));
        fclose(fin);
        fclose(fout);
        return 1;
    }

    printf("bbb\n");

    for (int i = 0; i < 5; ++i)
    {
        printf("%.2X ", data.buf[i]);
    }
    putchar('\n');

    getText(&data);

    printf("n_lines: %d\n", data.n_lines);
    for (int i = 0; i < 5; ++i)
    {
        printf("%.2X ", data.buf[i]);
    }
    putchar('\n');
    printf("%p\n\n", data.buf);
    for (int i = 0; i < data.n_lines; ++i)
    {
        printf("%p ", data.text[i].s);
    }
    putchar('\n');
    printf("ccc\n");

    if (data.text == NULL)
    {
        perror("Error");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    //qsort(data.text, data.n_lines, sizeof(char*), myStrcmp);
    writeLines(fout, &data);
    printf("ddd\n");

    free(data.buf);
    printf("eee\n");
    free(data.text);
    printf("fff\n");
    fclose(fin);
    fclose(fout);
    return 0;
}