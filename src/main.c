#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <onegin.h>

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

    getText(&data);

    if (data.text == NULL)
    {
        perror("Error");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    qsort(data.text, data.n_lines, sizeof(char*), myStrcmp);
    writeLines(fout, data.text, data.n_lines);

    free(data.buf);
    free(data.text);
    fclose(fin);
    fclose(fout);
    return 0;
}