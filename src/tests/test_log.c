#include "config_tests.h"
#include "minunit.h"
#include <core/core.h>
#include <log/log.h>

#define assert_lpc(name, is, should_be)                                        \
    mu_assert(0 == strcmp(is, should_be), "[%s] != [%s]", is, should_be);

struct log_apache_common_test_data {
    char* line;
    struct log_apache_common lpc;
} log_apache_common_test_data[] = {
    {
        "127.0.0.1 jimbowales jimbo [17/Abr/2017:20:10:36 -0600] \"GET "
        "/wiki.png HTTP/1.1\" 200 51200",
        {"127.0.0.1", "jimbowales", "jimbo", "17/Abr/2017:20:10:36 -0600",
         "GET", "/wiki.png", "HTTP/1.1", "200", "51200"},
    },
    {"162.158.79.19 - - [14/May/2017:06:37:42 +0200] \"GET "
     "/busqueda/recetas_de/rezetas-de-carme HTTP/1.1\" 200 6230",
     {"162.158.79.19", "-", "-", "14/May/2017:06:37:42 +0200", "GET",
      "/busqueda/recetas_de/rezetas-de-carme", "HTTP/1.1", "200", "6230"}},
    {NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}}};

char* test_log_apache_common_parse()
{
    struct log_apache_common lpc;
    bool res;
    char line[2048];

    struct log_apache_common_test_data* ll = log_apache_common_test_data;
    while (ll->line != NULL) {
        strcpy(line, ll->line);
        res = log_apache_common_parse(&lpc, line);
        // trace("Line parsed: [%d]", res);

        mu_assert(res == true, "Error parsing line: %s", line);

        assert_lpc("ip", lpc.ip, ll->lpc.ip);
        assert_lpc("ident", lpc.ident, ll->lpc.ident);
        assert_lpc("authuser", lpc.authuser, ll->lpc.authuser);
        assert_lpc("date", lpc.date, ll->lpc.date);
        assert_lpc("method", lpc.method, ll->lpc.method);
        assert_lpc("path", lpc.path, ll->lpc.path);
        assert_lpc("version", lpc.version, ll->lpc.version);
        assert_lpc("status", lpc.status, ll->lpc.status);
        assert_lpc("bytes", lpc.bytes, ll->lpc.bytes);

        ll++;
    }

    return NULL;
}

struct log_apache_combined_test_data {
    char* line;
    struct log_apache_combined lpc;
} log_apache_combined_test_data[] = {
    {
        "127.0.0.1 jimbowales jimbo [17/Abr/2017:20:10:36 -0600] \"GET "
        "/wiki.png HTTP/1.1\" 200 51200 \"Referer\" \"User Agent\"",
        {{"127.0.0.1", "jimbowales", "jimbo", "17/Abr/2017:20:10:36 -0600",
          "GET", "/wiki.png", "HTTP/1.1", "200", "51200"},
         "Referer",
         "User Agent"},
    },
    {"162.158.79.19 - - [14/May/2017:06:37:42 +0200] \"GET "
     "/busqueda/recetas_de/rezetas-de-carme HTTP/1.1\" 200 6230 "
     "\"http://www.example.com/start.html\" \"Mozilla/4.08 [en] (Win98; I "
     ";Nav)\"",
     {{"162.158.79.19", "-", "-", "14/May/2017:06:37:42 +0200", "GET",
       "/busqueda/recetas_de/rezetas-de-carme", "HTTP/1.1", "200", "6230"},
      "http://www.example.com/start.html",
      "Mozilla/4.08 [en] (Win98; I ;Nav)"}},
    {NULL,
     {{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, NULL, NULL}}};

char* test_log_apache_combined_parse()
{
    struct log_apache_combined lac;
    bool res;
    char line[2048];

    struct log_apache_combined_test_data* ll = log_apache_combined_test_data;
    while (ll->line != NULL) {
        strcpy(line, ll->line);
        res = log_apache_combined_parse(&lac, line);
        // trace("Line parsed: [%d]", res);

        mu_assert(res == true, "Error parsing line: %s", line);

        assert_lpc("ip", lac.lpc.ip, ll->lpc.lpc.ip);
        assert_lpc("ident", lac.lpc.ident, ll->lpc.lpc.ident);
        assert_lpc("authuser", lac.lpc.authuser, ll->lpc.lpc.authuser);
        assert_lpc("date", lac.lpc.date, ll->lpc.lpc.date);
        assert_lpc("method", lac.lpc.method, ll->lpc.lpc.method);
        assert_lpc("path", lac.lpc.path, ll->lpc.lpc.path);
        assert_lpc("version", lac.lpc.version, ll->lpc.lpc.version);
        assert_lpc("status", lac.lpc.status, ll->lpc.lpc.status);
        assert_lpc("bytes", lac.lpc.bytes, ll->lpc.lpc.bytes);

        assert_lpc("referer", lac.referer, ll->lpc.referer);
        assert_lpc("user_agent", lac.user_agent, ll->lpc.user_agent);
        ll++;
    }

    return NULL;
}

struct log_apache_error_test_data {
    char* line;
    struct log_apache_error lpc;
} log_apache_error_test_data[] = {
    {"[Fri Sep 09 10:42:29.902022 2011] [core:error] [pid 35708:tid "
     "4328636416] [client 72.15.99.187] File does not exist:  "
     "/usr/local/apache2/htdocs/favicon.ico",
     {"Fri Sep 09 10:42:29.902022 2011", "core:error",
      "pid 35708:tid 4328636416", "72.15.99.187",
      "File does not exist:  /usr/local/apache2/htdocs/favicon.ico"}},
    {NULL, {NULL, NULL, NULL, NULL, NULL}}};

char* test_log_apache_error_parse()
{
    struct log_apache_error lpc;
    bool res;
    char line[2048];

    struct log_apache_error_test_data* ll = log_apache_error_test_data;
    while (ll->line != NULL) {
        strcpy(line, ll->line);
        res = log_apache_error_parse(&lpc, line);
        // trace("Line parsed: [%d]", res);

        mu_assert(res == true, "Error parsing line: %s", line);

        assert_lpc("date", lpc.date, ll->lpc.date);
        assert_lpc("module", lpc.module, ll->lpc.module);
        assert_lpc("proccess", lpc.proccess, ll->lpc.proccess);
        assert_lpc("ip", lpc.ip, ll->lpc.ip);
        assert_lpc("message", lpc.message, ll->lpc.message);

        ll++;
    }

    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_log_apache_common_parse);
    mu_run_test(test_log_apache_combined_parse);
    mu_run_test(test_log_apache_error_parse);

    return NULL;
}

RUN_TESTS(all_tests);
