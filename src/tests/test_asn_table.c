#include "config_tests.h"
#include "minunit.h"
#include <asn/asn.h>
#include <core/core.h>

#define RAW_TABLE_FILE "misc/data-raw-table.txt"
#define USED_FILE_AUTNUMS "misc/data-used-autnums.txt"

struct asn_table* table;

/*
char* test_load_sample() {
    table = asn_table_alloc();
    mu_assert(NULL != table, "Cannot alloc memory for asn_table");
    char bits[33];

    struct asn* asn;
    size_t i = 0;
    while (1) {
        if (0 == asns[i].number)
            break;

        asn = asn_alloc(asns[i].network, asns[i].number);
        mu_assert(NULL != asn, "Error parsing network: %s", asns[i].network);

        asn_to_binary_s(asn, bits);
        //        trace("Adding: %s - %d: %s", asns[i].network, asns[i].number,
        //        bits);
        asn_table_add_asn(table, asn);

        asn_table_add_name(table, asns[i].number, asns[i].name);

        i++;
    }

    info("%zu asns added", i);
    printf("=========== TABLE ============\n");
    asn_table_print(table);
    printf("=========== TABLE ============\n");

    return NULL;
}
*/

char* test_load_file()
{
    table = asn_table_init(RAW_TABLE_FILE, USED_FILE_AUTNUMS);
    mu_assert(NULL != table, "Error creating table from files");
    return NULL;
}

char* test_lookup()
{
    struct asn* asn;

    char bits_ip[33];
    struct in_addr in;
    int valid;

    int i = 0;
    while (0 != ips[i].asn) {

        valid = inet_aton(ips[i].ip, &in);
        mu_assert(0 != valid, "Error parsing IP: %s", ips[i].ip);
        in_addr_to_binary_s(in, bits_ip);
        // trace("Searching for: %s - %s", inet_ntoa(in), bits_ip);

        asn = asn_table_find_by_ip_s(table, ips[i].ip);
        mu_assert(NULL != asn, "%s [%s] Not found in table", ips[i].ip,
                  bits_ip);
        mu_assert(asn_number(asn) == ips[i].asn,
                  "Error searching asn (%u != %zu) for %s [%s]",
                  asn_number(asn), ips[i].asn, ips[i].ip, bits_ip);

        /*
                char bits_net[33];
                in_addr_to_binary_s(asn->ip, bits_net);
                trace("%s found in %s/%d (%u) - %s: %s", ips[i].ip,
           asn_ip_s(asn),
                      asn_mask(asn), asn_number(asn), bits_net,
                      asn_table_find_asn_name_by_number(table,
           asn_number(asn)));
        */
        i++;
    }

    return NULL;
}

char* test_names()
{
    char* name = NULL;

    int i = 0;
    while (0 != asns[i].number) {
        name = asn_table_find_name_by_asnum(table, asns[i].number);
        mu_assert(0 == strcmp(asns[i].name, name),
                  "Names are different: >%s< >%s<", asns[i].name, name);
        i++;
    }

    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    /*
mu_run_test(test_load_sample);
mu_run_test(test_lookup);
mu_run_test(test_names);
asn_table_free(table);
*/
    mu_run_test(test_load_file);
    mu_run_test(test_lookup);
    mu_run_test(test_names);

    //    asn_table_free(table);

    return NULL;
}

RUN_TESTS(all_tests);
