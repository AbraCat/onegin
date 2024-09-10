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
    
    char* text = NULL;

    int error = readFile(fin, &text);
    if (error)
    {
        printf("Error: %s", strerror(error));
        fclose(fin);
        fclose(fout);
        return 1;
    }

    int n_lines = getNLines(text);
    char** lines = getLines(text, n_lines);

    if (lines == NULL)
    {
        perror("Error");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    qsort(lines, n_lines, sizeof(char*), myStrcmp);
    writeLines(fout, lines, n_lines);

    free(text);
    free(lines);
    fclose(fin);
    fclose(fout);
    return 0;
}