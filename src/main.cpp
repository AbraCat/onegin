#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sort.h>
#include <onegin.h>
#include <options.h>

static int n_opts = 4, n_cmps = 2;
static const char std_test_in_name[] = ".\\txt\\test-input.txt", std_in_name[] = ".\\txt\\input.txt", std_out_name[] = ".\\txt\\output.txt";

int main(int argc, const char* argv[])
{
    struct Resource resource = {2, 2, NULL, NULL};

    struct Option opts[] = {{"-h", "--help"}, {"-r", "--reverse"}, {"-i", "--input", std_test_in_name}, {"-o", "--output", std_out_name}};
    handleError(parseOpts(argc, argv, opts, n_opts), &resource);

    //testOpts(opts, n_opts);

    if (optByName(opts, n_opts, "-h")->trig)
    {
        printHelpMsg(stdout);
        return 0;
    }

    const char *in_name = optByName(opts, n_opts, "-i")->str_arg, *out_name = optByName(opts, n_opts, "-o")->str_arg;
    FILE *fin = fopen(in_name, "r"), *fout = fopen(out_name, "w");

    FILE* to_close[] = {fin, fout};
    resource.to_close = to_close;

    if (fin == NULL || fout == NULL)
        handleError(errno, &resource);
    
    struct TextData data = {0};

    handleError(readFile(fin, &data.buf), &resource);
    char* to_free[] = {data.buf, NULL};
    resource.to_free = to_free;

    handleError(getLines(data.buf, &data.n_lines, &data.text, &data.maxlen), &resource);
    to_free[1] = (char*)data.text;

    voidcmp_f cmps[] = {(voidcmp_f)myStrcmp, (voidcmp_f)myStrcmpR};

    if (optByName(opts, n_opts, "-r")->trig)
        for (int i = n_cmps - 1; i >= 0; --i)
            sortAndWrite(fout, &data, cmps[i]);
    else
        for (int i = 0; i < n_cmps; ++i)
            sortAndWrite(fout, &data, cmps[i]);

    fprintf(fout, "%s", data.buf);

    free_resource(&resource);
    return 0;
}