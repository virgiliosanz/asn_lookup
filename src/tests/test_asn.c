#include "config_tests.h"
#include "minunit.h"
#include <asn/asn.h>
#include <core/core.h>

char* test_binary()
{

    struct in_addr in;
    char bits[33];

    int i = 0;
    while (0 != ips[i].asn) {
        inet_aton(ips[i].ip, &in);
        in_addr_to_binary_s(in, bits);

        mu_assert(0 != strcpy(bits, ips[i].ip_b),
                  "error converting bits %s != %s", bits, ips[i].ip_b);
        i++;
    }

    return NULL;
}

char* test_alloc()
{
    struct asn* asn;

    int i = 0;
    while (0 != asns[i].number) {

        asn = asn_alloc(asns[i].network, asns[i].number);
        mu_assert(NULL != asn, "Cannot create ip from %s", asns[i].network);
        asn_free(asn);

        i++;
    }

    return NULL;
}

char* test_parsing()
{
    struct asn* asn;
    int i = 0;
    while (0 != asns[i].number) {

        asn = asn_alloc(asns[i].network, asns[i].number);
        mu_assert(0 != strcmp(asns[i].network, asn_ip_s(asn)),
                  "Error parsing IP: [%s], [%s]", asns[i].network,
                  asn_ip_s(asn));
        mu_assert(asns[i].mask == asn_mask(asn),
                  "Error parsing network [%s]: %d = %d", asns[i].network,
                  asns[i].mask, asn_mask(asn));
        asn_free(asn);

        i++;
    }
    return NULL;
}

char* test_functions()
{
    struct asn* asn;
    int i = 0;

    while (0 != asns[i].number) {
        asn = asn_alloc(asns[i].network, asns[i].number);
        mu_assert(NULL != asn, "Error creating asn struct");
        mu_assert(asns[i].number == asn_number(asn), "Differnt Numbers");
        mu_assert(asns[i].mask == asn_mask(asn), "Differnt Mask");

        mu_assert(
            0 != strncpy(asns[i].network, asn_ip_s(asn), strlen(asn_ip_s(asn))),
            "Error converting to string >%s< >%s<", asn_ip_s(asn),
            asns[i].network);

        asn_free(asn);
        i++;
    }
    return NULL;
}

char* test_ip_functions()
{
    struct asn* asn;
    int i = 0;
    int j = 0;

    while (0 != asns[i].number) {

        asn = asn_alloc(asns[i].network, asns[i].number);
        mu_assert(NULL != asn, "Error creating asn struct");
        mu_assert(asns[i].number == asn_number(asn), "Differnt Numbers");
        mu_assert(asns[i].mask == asn_mask(asn), "Differnt Mask");

        j = 0;
        while (0 != ips[j].asn) {
            if (i == ips[j].asn_pos) {
                mu_assert(true == asn_has_ip(asn, ips[j].ip),
                          "%s should be in %s", ips[j].ip, asns[i].network);
            }
            else {
                mu_assert(false == asn_has_ip(asn, ips[j].ip),
                          "%s should not be in %s", ips[j].ip, asns[i].network);
            }

            j++;
        }
        i++;

        asn_free(asn);
    }
    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_functions);
    mu_run_test(test_binary);
    mu_run_test(test_alloc);
    mu_run_test(test_parsing);
    mu_run_test(test_ip_functions);

    return NULL;
}

RUN_TESTS(all_tests);
