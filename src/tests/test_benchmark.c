#include "config_tests.h"
#include "minunit.h"
#include <asn/asn.h>
#include <core/core.h>

#define RAW_TABLE_FILE "misc/data-raw-table.txt"
#define USED_FILE_AUTNUMS "misc/data-used-autnums.txt"

struct asn_table* table;

char* test_load_file()
{
    table = asn_table_init(RAW_TABLE_FILE, USED_FILE_AUTNUMS);
    mu_assert(NULL != table, "Error creating table from files");
    return NULL;
}

char* test_benchmark()
{
#define NTESTS (1000000)
    struct asn* asn;
    size_t i;

    info("Starting the Benchmark");
    clock_t begin = clock();
    for (i = 0; i < NTESTS; i++) {
        asn = asn_table_find_by_ip_s(table, ips[0].ip);
        asn = asn_table_find_by_ip_s(table, ips[1].ip);
        asn = asn_table_find_by_ip_s(table, ips[2].ip);
        asn = asn_table_find_by_ip_s(table, ips[3].ip);
        asn = asn_table_find_by_ip_s(table, ips[4].ip);
        asn = asn_table_find_by_ip_s(table, ips[5].ip);
        asn = asn_table_find_by_ip_s(table, ips[6].ip);
        asn = asn_table_find_by_ip_s(table, ips[7].ip);
        asn = asn_table_find_by_ip_s(table, ips[8].ip);
        asn = asn_table_find_by_ip_s(table, ips[9].ip);
        //        mu_assert(NULL != asn, "asn es NULL!!");
    }
    double time_spent = (clock() - begin) / CLOCKS_PER_SEC;
    info("%u queries in %.2f secs -> %.2f queries per sec", 10 * NTESTS,
         time_spent, (10 * NTESTS) / time_spent);

    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_load_file);
    mu_run_test(test_benchmark);

    //    asn_table_free(table);

    return NULL;
}

RUN_TESTS(all_tests);
