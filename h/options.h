#ifndef OPTIONS_H
#define OPTIONS_H

struct Option
{
    char *sh_name, *l_name;
    const char *str_arg;
    int trig, int_arg, l_name_len;
};

void handleError(int error, FILE* fin, FILE* fout);
int optcmp(struct Option* opt1, struct Option* opt2);
int parseOpts(int argc, const char* argv[], struct Option* opts, int n_opts);
struct Option* optByName(struct Option* opts, int n_opts, const char* sh_name);

#endif //OPTIONS_H