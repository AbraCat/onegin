#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sort.h>
#include <options.h>

void handleError(int error, FILE* file1, FILE* file2)
{
    if (error)
    {
        printf("Error: %s\n", strerror(error));

        if (error == EINVAL)
            printf("(use -h or --help for help message)\n");

        if (file1 != NULL)
            fclose(file1);
        if (file2 != NULL)
            fclose(file2);

        exit(error);
    }
}

int optcmp(struct Option* opt1, struct Option* opt2)
{
    return strcmp(opt1->sh_name, opt2->sh_name);
}

struct Option* optByName(struct Option* opts, int n_opts, const char* sh_name)
{
    int lft = -1, rgt = n_opts, mid = 0;

    while (rgt - lft > 1)
    {
        mid = (lft + rgt) / 2;
        if (strcmp(opts[mid].sh_name, sh_name) >= 0)
            rgt = mid;
        else
            lft = mid;
    }

    return (strcmp(opts[rgt].sh_name, sh_name) == 0) ? opts + rgt : NULL;
}

void printHelpMsg(FILE* file)
{
    fprintf(file, "%s\n\n%s\n%s\n%s\n%s\n%s\n", "Program for sorting text.", "Command line options:",
    "-h (--help): prints help message", "-r (--reverse): prints result of reverse sort first", 
    "-i (--input): input file name", "-o (--output): output file name");
}

int parseOpts(int argc, const char* argv[], struct Option* opts, int n_opts)
{
    myQsort(opts, n_opts, sizeof(struct Option), (voidcmp_f)optcmp);

    for (int j = 0; j < n_opts; ++j)
        opts[j].l_name_len = strlen(opts[j].l_name);

    int trig = 0;

    for (int i = 1; i < argc; ++i)
    {
        trig = 0;
        for (int j = 0; j < n_opts; ++j)
        {
            if (strcmp(argv[i], opts[j].sh_name) == 0)
            {
                if (i == argc - 1 || argv[i + 1][0] == '-')
                {
                    trig = opts[j].trig = 1;
                    opts[j].str_arg = NULL;
                    opts[j].int_arg = 0;
                    break;
                }

                trig = opts[j].trig = 1;
                opts[j].str_arg = argv[++i];
                opts[j].int_arg = strtol(opts[j].str_arg, NULL, 10);
                break;
            }

            if (strncmp(argv[i], opts[j].l_name, opts[j].l_name_len) != 0)
                continue;

            if (argv[i][opts[j].l_name_len] == '\0')
            {
                trig = opts[j].trig = 1;
                opts[j].str_arg = NULL;
                opts[j].int_arg = 0;
                break;
            }

            if (argv[i][opts[j].l_name_len] != '=' || argv[i][opts[j].l_name_len + 1] == '\0')
                return EINVAL;

            trig = opts[j].trig = 1;
            opts[j].str_arg = argv[i] + opts[j].l_name_len + 1;
            opts[j].int_arg = strtol(opts[j].str_arg, NULL, 10);
            break;
        }
        if (!trig)
            return EINVAL;
    }
    for (int j = 0; j < n_opts; ++j)
    {
        if (!opts[j].trig)
        {
            opts[j].str_arg = NULL;
            opts[j].int_arg = 0;
        }
    }
    return 0;
}