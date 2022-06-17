// By Virgilio Sanz <virgilio.sanz@gmail.com> April 2017

#include <asn/asn.h>
#include <core/core.h>
#include <getopt.h>

#define ASNS_RAW_FNAME "misc/data-raw-table.txt"
#define ASNS_USED_FNAME "misc/data-used-autnums.txt"

void proccess(struct asn_table* table)
{
    char* line = NULL;
    size_t len = 0;
    ssize_t slen;
    char* c;
    char* name;
    uint32_t number;

    printf("AS NUM;BYTES;AS NAME\n");
    while ((slen = getline(&line, &len, stdin)) != -1) {
        trim_eol(line);

        number = atoi(line);
        name = asn_table_find_name_by_asnum(table, number);

        c = line;
        while (*c != '\0')
            putchar(*(c++));

        printf(";\"%s\"\n", name);
    }

    ALLOC_FREE(line);
}

void print_help()
{
    fprintf(
        stderr,
        "\n"
        "ASN Name at the end of the line. Program expects to have the\n"
        "number at the beginning of each line.\n\n"
        "Get ASN data files from:\n"
        "\thttp://thyme.apnic.net/current/data-raw-tablei\n"
        "\thttp://thyme.apnic.net/current/data-used-autnums\n\n"
        "Options:\n"
        "\t-h Print this info\n"
        "\t-d Print debug information to stderr 0 = erro, 1 = warn, 2 = info\n"
        "\t-r <filename> File path to raw data: misc/data-raw-table.txt\n"
        "\t-s <filename> File path to used autnums:"
        "\t-f log format 0 = apache, 1 = ..."
        "misc/data-used-autnums.txt\n\n");
}

int main(int argc, char* argv[])
{
    int option = 0;

    char* raw_fname = ASNS_RAW_FNAME;
    char* used_fname = ASNS_USED_FNAME;
    enum debug_level debug_level;

    int level = 0;

    while ((option = getopt(argc, argv, "hd:r:s:f:")) != -1) {
        switch (option) {
        case 'r':
            raw_fname = strdup(optarg);
            break;
        case 's':
            used_fname = strdup(optarg);
            break;
        case 'd':
            level = atoi(optarg);

            if (0 == level)
                debug_level = debug_level_error;
            else if (1 == level)
                debug_level = debug_level_warn;
            else
                debug_level = debug_level_trace;
            break;

        case 'f':

            break;

        default:
            print_help();
            goto error;
        }
    }

    debug_level_set(debug_level);

    info("Loading ASN info");

    trace("Loading ASN tables from: %s & %s", raw_fname, used_fname);
    struct asn_table* table = asn_table_init(raw_fname, used_fname);
    error_if(NULL == table, error, "Cannot load from %s & %s", raw_fname,
             used_fname);

    info("Starting proccess from stdin");
    proccess(table);

    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}
