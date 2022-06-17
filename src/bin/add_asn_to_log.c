// By Virgilio Sanz <virgilio.sanz@gmail.com> April 2017

#include <asn/asn.h>
#include <core/core.h>
#include <getopt.h>

#define ASNS_RAW_FNAME "misc/data-raw-table.txt"
#define ASNS_USED_FNAME "misc/data-used-autnums.txt"

bool search_ip(char* line, char* ip);

#define TO_MB(N) ((N) / 1048576)
void proccess(struct asn_table* table, int separator, uint16_t position)
{
    char* line = NULL;
    size_t len = 0;
    ssize_t slen;
    size_t lines = 0;
    size_t total_bytes = 0;
    struct asn* asn;
    char* c;
    char ip_s[16];

    clock_t begin = clock();
    while ((slen = getline(&line, &len, stdin)) != -1) {
        trim_eol(line);
        lines++;
        total_bytes += slen;

        if ((lines % 500000) == 0)
            info("%zu lines (%zu Mb)", lines, TO_MB(total_bytes));

        if (get_field(line, ip_s, 16, separator, position)) {
            asn = asn_table_find_by_ip_s(table, ip_s);
            if (NULL == asn) {
                warn("Cannot load at line %zu IP: %s", lines, ip_s);
                continue;
            }
            c = line;
            while (*c != '\0')
                putchar(*(c++));

            printf("\t%d\n", asn_number(asn));
        }
        else {
            warn("Cannot load at line %zu >%s<", lines, line);
        }
    }

    ALLOC_FREE(line);

    double time_spent = (clock() - begin) / CLOCKS_PER_SEC;

    info("%zu lines (%zu Mb) read in %.2f secs", lines, TO_MB(total_bytes),
         time_spent);
}

void print_help()
{
    fprintf(stderr,
            "\n"
            "Parse Akamai log lines to look for end user IP and add\n"
            "ASN at the end of the line\n\n"
            "Get ASN data files from:\n"
            "\thttp://thyme.apnic.net/current/data-raw-table\n"
            "\thttp://thyme.apnic.net/current/data-used-autnums\n\n"
            "Options:\n"
            "\t-h Print this info\n"
            "\t-d Print debug information to stderr, 1 or 2 levels\n"
            "\t-s Log character separator. Default = \" \"\n"
            "\t-p IP 'field' position. Mandatory! \n"
            "\t-r <filename> File path to raw data: misc/data-raw-table.txt\n"
            "\t-u <filename> File path to used autnums: "
            "misc/data-used-autnums.txt\n\n");
}

int main(int argc, char* argv[])
{
    int option = 0;

    char* raw_fname = ASNS_RAW_FNAME;
    char* used_fname = ASNS_USED_FNAME;
    enum debug_level debug_level = debug_level_error;
    int level = 0;
    int separator = ' ';
    uint16_t position = 1;

    while ((option = getopt(argc, argv, "hr:u:s:p:d:")) != -1) {
        switch (option) {
        case 'r':
            raw_fname = strdup(optarg);
            break;
        case 'u':
            used_fname = strdup(optarg);
            break;
        case 's':
            separator = atoi(optarg);
            break;
        case 'p':
            position = atoi(optarg);
            break;
        case 'd':
            level = atoi(optarg);
            if (1 == level) {
                debug_level = debug_level_warn;
            }
            else {
                debug_level = debug_level_trace;
            }




            break;
        default:
            print_help();
            goto error;
        }
    }

    if (0 == position) {
        print_help();
        goto error;
    }

    debug_level_set(debug_level);

    info("Loading ASN info");

    trace("Loading ASN tables from: %s & %s", raw_fname, used_fname);
    struct asn_table* table = asn_table_init(raw_fname, used_fname);
    error_if(NULL == table, error, "Cannot load from %s & %s", raw_fname,
             used_fname);

    info("Starting proccess from stdin");
    proccess(table, separator, position);

    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}

/*
162.158.146.30 - - [09/Jul/2017:06:45:48 +0200] "GET /mi-cocinario/HTTP/1.1" 302
128 "-" "Mozilla/5.0 (compatible; Yahoo! Slurp;
http://help.yahoo.com/help/us/ysearch/slurp)" IP - - [dd/MMM/yyyy:HH:MM:SS
+0200] "METHOD URL HTTP/1.1" Response bytes "User Agent"
*/
bool search_ip(char* line, char* ip) { return get_field(line, ip, 16, ' ', 1); }
