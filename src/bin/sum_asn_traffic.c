// By Virgilio Sanz <virgilio.sanz@gmail.com> April 2017

#include <asn/asn.h>
#include <core/core.h>
#include <getopt.h>

struct asn_stats;
void asn_stats_free(struct asn_stats* stats);
struct asn_stats* asn_stats_alloc();
void asn_stats_add(struct asn_stats* stats, uint32_t asn, size_t bytes);
void asn_stats_print_results(struct asn_stats* stats);

bool search_bytes_and_asn(char* line, uint32_t* bytes, uint32_t* asn);

#define TO_MB(N) ((N) / 1048576)

int main()
{

    char* line = NULL;
    size_t len = 0;
    ssize_t slen = 0;
    size_t lines = 0;
    size_t total_bytes = 0;
    uint32_t bytes = 0;
    uint32_t asn = 0;
    struct asn_stats* stats = NULL;

    stats = asn_stats_alloc();
    error_if(NULL == stats, error, "Cannot allocate memory");

    clock_t begin = clock();
    while ((slen = getline(&line, &len, stdin)) != -1) {
        trim_eol(line);
        lines++;
        total_bytes += slen;

        if ((lines % 500000) == 0)
            info("%zu lines (%zu Mb)", lines, TO_MB(total_bytes));

        if (search_bytes_and_asn(line, &bytes, &asn)) {
            asn_stats_add(stats, asn, bytes);
        }
        else {
            trace("Cannot find bytes and asn info at line %zu: >>%s<<", lines,
                  line);
        }
    }

    ALLOC_FREE(line);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    info("%zu lines (%zu Mb) read in %.2f secs", lines, TO_MB(total_bytes),
         time_spent);

    asn_stats_print_results(stats);

    asn_stats_free(stats);

    return EXIT_SUCCESS;

error:
    asn_stats_free(stats);
    ALLOC_FREE(line);
    return EXIT_FAILURE;
}

/*
#Fields: date time cs-ip cs-method cs-uri sc-status sc-bytes time-taken
cs(Referer) cs(User-Agent) cs(Cookie) x-custom ghostip cache_status
2017-03-20	21:32:25	188.86.49.26	GET
/tedilive-aka2.telecable.es/HLS_Live/SD_02/01.m3u8	200	17560
1	"-"	"VRPC"	"-"	"-"	77.209.227.38	1
*/
bool search_bytes_and_asn(char* line, uint32_t* bytes, uint32_t* asn)
{
    static const uint32_t bytes_tab_num = 7;
    static const uint32_t asn_tab_num = 14;

    char* start = line;
    char* end = {0};
    uint32_t found = 0;

    while ((end = strchr(start, '\t')) != NULL) {
        found++;

        if (found == bytes_tab_num) {
            *bytes = atoi(start);
        }
        else if (found == asn_tab_num) {
            *asn = atoi(start + 1);

            break;
        }
        start = end + 1;
    }
    //    trace("total Found %d: %d %d", found, *bytes, *asn);

    if (found < asn_tab_num)
        return false;

    return true;
}

/* ====================== ASN_STATS Implementation ===================*/
/** Struct designed to save data. Only bytes from now... */
struct asn_stats_data {
    size_t bytes;
};
struct asn_stats {
    struct asn_stats_data* data;
    uint32_t allocated;
};

void asn_stats_free(struct asn_stats* stats)
{
    if (stats == NULL)
        return;
    ALLOC_FREE(stats->data);
    ALLOC_FREE(stats);
}

struct asn_stats* asn_stats_alloc()
{
    struct asn_stats_data* data = NULL;
    struct asn_stats* stats = NULL;

    data = calloc(ASN_NAMES_NUMBER_OF_ASNS, sizeof(struct asn_stats_data));
    error_if(NULL == data, error, "Cannot allocate memory");

    stats = ALLOC_INIT(struct asn_stats, {data, ASN_NAMES_NUMBER_OF_ASNS});
    error_if(NULL == stats, error, "Cannot allocate memory");

    return stats;

error:
    asn_stats_free(stats);
    return NULL;
}
void asn_stats_add(struct asn_stats* stats, uint32_t asn, size_t bytes)
{
    error_if(asn > stats->allocated, error, "ASN Out of range");
    stats->data[asn].bytes += bytes;

error:
    return;
}

void asn_stats_print_results(struct asn_stats* stats)
{
    for (uint32_t i = 0; i < stats->allocated; i++) {
        if (stats->data[i].bytes > 0) {
            printf("%u;%zu\n", i, stats->data[i].bytes);
        }
    }
}
