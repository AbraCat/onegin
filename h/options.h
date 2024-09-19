#ifndef OPTIONS_H
#define OPTIONS_H

struct Option
{
    const char *sh_name, *l_name, *str_arg;
    int int_arg, trig, l_name_len;
};

struct Resource
{
    int n_close, n_free;
    FILE** to_close;
    char** to_free;
};

void free_resource(struct Resource* resource);
void handleError(int error, struct Resource* resource);

int optcmp(struct Option* opt1, struct Option* opt2);

int parseOpts(int argc, const char* argv[], struct Option* opts, int n_opts);
struct Option* optByName(struct Option* opts, int n_opts, const char* sh_name);

void printHelpMsg(FILE* file);

void testOpts(struct Option* opts, int n_opts);

#endif //OPTIONS_H