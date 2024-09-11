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

    int error = readFile(fin, &data);
    if (error)
    {
        printf("Error: %s", strerror(error));
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if (data.text == NULL)
    {
        perror("Error");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    getText(&data);

    qsort(data.text, data.n_lines, sizeof(struct String), (int (*)(const void*, const void*))myStrcmp);
    writeLines(fout, &data);
    fprintf(fout, "\n%s\n\n", "--------------------------------");
    qsort(data.text, data.n_lines, sizeof(struct String), (int (*)(const void*, const void*))myStrcmpR);
    writeLines(fout, &data);
    fprintf(fout, "\n%s\n\n", "--------------------------------");
    fprintf(fout, "%s", data.buf);

    //freeText(&data);
    free(data.text);
    free(data.buf);
    fclose(fin);
    fclose(fout);
    return 0;
}