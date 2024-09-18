#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sort.h>
#include <onegin.h>
#include <options.h>

static int N_OPTS = 4;
static const char TEST_NAME[] = ".\\txt\\test-input.txt", IN_NAME[] = ".\\txt\\input.txt", OUT_NAME[] = ".\\txt\\output.txt";

int main(int argc, const char* argv[])
{
    struct Option opts[] = {{"-h", "--help"}, {"-i", "--input"}, {"-o", "--output"}, {"-r", "--reverse"}};

    handleError(parseOpts(argc, argv, opts, N_OPTS), NULL, NULL);

    const char *test_in_name = ".\\txt\\test-input.txt", *in_name = ".\\txt\\input.txt", *out_name = ".\\txt\\output.txt";
    FILE *fin = fopen(test_in_name, "r"), *fout = fopen(out_name, "w");

    if (fin == NULL || fout == NULL)
        handleError(errno, fin, fout);
    
    struct TextData data = {0};

    handleError(readFile(fin, &data), fin, fout);
    handleError(getText(&data), fin, fout);

    myQsort(data.text, data.n_lines, sizeof(struct String), (voidcmp_f)myStrcmp);
    writeLines(fout, &data);
    fprintf(fout, "\n%s\n\n", "--------------------------------");

    myQsort(data.text, data.n_lines, sizeof(struct String), (voidcmp_f)myStrcmpR);
    writeLines(fout, &data);
    fprintf(fout, "\n%s\n\n", "--------------------------------");

    fprintf(fout, "%s", data.buf);

    freeText(&data);
    fclose(fin);
    fclose(fout);
    return 0;
}