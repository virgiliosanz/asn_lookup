#include "config_tests.h"
#include "minunit.h"
#include <csv.h>

struct csv_parser parser;

char* test_init()
{
    int r = csv_init(&parser, CSV_STRICT);
    mu_assert(r == 0, "Creating parser: %s", csv_strerror(csv_error(&parser)));

    return NULL;
}

char* test_load_file() { return NULL; }

char* test_benchmark() { return NULL; }

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_init);
    mu_run_test(test_load_file);
    mu_run_test(test_benchmark);

    return NULL;
}

RUN_TESTS(all_tests);
