#pragma once

#define _XOPEN_SOURCE /* See feature_test_macros(7) */
#include <stdbool.h>
#include <time.h>

// -------------- APACHE LOGS -----------------
/**
 * Fills struct tm with date in string forma "DD/MM/YYYY:HH:MM:SS -999"
 */
#define log_apache_strptime(s, tm) strptime(s, "%d/%b/%Y:%T", tm)

// Common format:
// host ident authuser date request status bytes
// 127.0.0.1 jimbowales jimbo [17/Abr/2017:20:10:36 -0600] "GET /wiki.png
// HTTP/1.1" 200 51200

struct log_apache_common {
    char* ip;
    char* ident;
    char* authuser;
    char* date;
    char* method;
    char* path;
    char* version;
    char* status;
    char* bytes;
};

/**
 * Parse string and create a log_apache_common struct that points to the string
 * passed. It substitute chars by \0 so the strings are valid C strings... I
 * mean, string passed as argument is modified, if you need to keep it copy it
 * before...
 * return true if ok
 */
bool log_apache_common_parse(struct log_apache_common* l, char* s);

// Combined Format
// "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\""
// 127.0.0.1 - frank [10/Oct/2000:13:55:36 -0700] "GET /apache_pb.gif
// HTTP/1.0" 200 2326 "http://www.example.com/start.html" "Mozilla/4.08 [en]
// (Win98; I ;Nav)"
struct log_apache_combined {
    struct log_apache_common lpc;
    char* referer;
    char* user_agent;
};
bool log_apache_combined_parse(struct log_apache_combined* l, char* s);

// Error log
// date module proccess client_ip error_message
// [Fri Sep 09 10:42:29.902022 2011] [core:error] [pid 35708:tid 4328636416]
// [client 72.15.99.187] File does not exist:
// /usr/local/apache2/htdocs/favicon.ico
struct log_apache_error {
    char* date;
    char* module;
    char* proccess;
    char* ip;
    char* message;
};

#define log_apache_error_strptime(s, tm) strptime(s, "%a %b %d %T.%n %y", tm)
bool log_apache_error_parse(struct log_apache_error* l, char* s);

// -------------- AKAMAI LOGS -----------------
// --- Extended
// date_YYYY-MM-DD \t time_HH:MM:SS \t client_ip \t http_method \t arl_stem \t
// status_code \t total_bytes \t transfer_time \t "referrer"\t "user_agent"\t
// "cookie"
struct log_akamai_extended {
    char* datetime;
    char* client_ip;
    char* http_method;
    char* arl_stem;
    char* status_code;
    char* total_bytes;
    char* transfer_time;
    char* referrer;
    char* user_agent;
    char* cookie;
};
bool log_akamai_extended_parse(struct log_akamai_extended* l, char* s);

// ---  Extended + Custom field + GhostIP + Cache Status
// date time cs-ip cs-method cs-uri sc-status sc-bytes time-taken cs(Referer)
// cs(User-Agent) cs(Cookie) x-custom ghostip cache_status 2017-08-27  13:26:06
// 85.87.128.87  GET /domain.com/path/to/file.ext 200 2527  0
// "/otherdomain.com/otherpath/otherfile.ext" "User-Agent"  "Cookie" "custom"
// 173.223.11.162  1
struct log_akamai_extended_1 {
    struct log_akamai_extended lpc;
    char* custom;
    char* ghostip;
    char* cache_status;
};
bool log_akamai_extended_1_parse(struct log_akamai_extended_1* l, char*);
