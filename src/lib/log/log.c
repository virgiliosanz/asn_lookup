#include <core/core.h>
#include <log/log.h>

// TODO Implementar tests de las conversiones de fecha

#define ERROR_PARAM "Param Error"
#define move_to_next_char(tag, ss, cc)                                         \
    do {                                                                       \
        while (*ss != cc) {                                                    \
            ss++;                                                              \
            error_if(0 == *ss, error, "EoL searching [%s] in [%s]", tag, ss);  \
        }                                                                      \
        *ss = '\0';                                                            \
        ss++;                                                                  \
    } while (0)

// host ident authuser date request status bytes
// 127.0.0.1 jimbowales jimbo [17/Abr/2017:20:10:36 -0600] "GET /wiki.png
// HTTP/1.1" 200 51200
bool log_apache_common_parse(struct log_apache_common* lpc, char* s)
{
    error_if(lpc == NULL, error, ERROR_PARAM);
    error_if(s == NULL, error, ERROR_PARAM);

    lpc->ip = s;
    move_to_next_char("ip", s, ' ');
    // trace("ip: %s", lpc->ip);

    // ident
    lpc->ident = s;
    move_to_next_char("ident", s, ' ');
    // trace("ident: %s", lpc->ident);

    // authuser
    lpc->authuser = s;
    move_to_next_char("authuser", s, ' ');
    // trace("authuser: %s", lpc->authuser);

    // date
    s++; // move away from [
    lpc->date = s;
    move_to_next_char("date", s, ']');
    s++; // move next space
    // trace("date: %s", lpc->date);

    // method
    s++; // move away from "
    lpc->method = s;
    move_to_next_char("method", s, ' ');
    // trace("method: %s", lpc->method);

    // path
    lpc->path = s;
    move_to_next_char("path", s, ' ');
    // trace("path: %s", lpc->path);

    // version
    lpc->version = s;
    move_to_next_char("version", s, '"');
    s++; // move next space
    // trace("version: %s", lpc->version);

    // status
    lpc->status = s;
    move_to_next_char("status", s, ' ');
    // trace("status: %s", lpc->status);

    // bytes
    lpc->bytes = s;
    // trace("bytes: %s", lpc->bytes);

    return true;

error:
    return false;
}

// host ident authuser date request status bytes referer user_agent
// 127.0.0.1 jimbowales jimbo [17/Abr/2017:20:10:36 -0600] "GET /wiki.png
// HTTP/1.1" 200 51200 "http://www.example.com/start.html" "Mozilla/4.08 [en]
// (Win98; I ;Nav)"
bool log_apache_combined_parse(struct log_apache_combined* l, char* s)
{
    error_if(l == NULL, error, ERROR_PARAM);
    error_if(s == NULL, error, ERROR_PARAM);

    bool res = log_apache_common_parse(&l->lpc, s);
    error_if(false == res, error, "Error Parsing Apache Common Part");

    // User Agent
    char* ss = l->lpc.bytes;
    move_to_next_char("referer", ss, ' ');
    // trace("bytes: %s", l->lpc.bytes);
    // Move next char after  first '"'
    ss++;
    l->referer = ss;
    move_to_next_char("referer", ss, '"');
    // trace("referer: %s", l->referer);

    // User Agent
    // Move next char after  first '"'
    ss += 2;
    l->user_agent = ss;
    move_to_next_char("user_agent", ss, '"');
    // trace("user_agent: %s", l->user_agent);

    return true;
error:
    return false;
}

// date module proccess client_ip error_message
// [Fri Sep 09 10:42:29.902022 2011] [core:error] [pid 35708:tid 4328636416]
// [client 72.15.99.187] File does not exist:
// /usr/local/apache2/htdocs/favicon.ico
bool log_apache_error_parse(struct log_apache_error* l, char* s)
{
    error_if(l == NULL, error, ERROR_PARAM);
    error_if(s == NULL, error, ERROR_PARAM);

    s++;
    l->date = s;
    move_to_next_char("date", s, ']');
    // trace("date: %s", l->date);

    s++;
    s++;
    l->module = s;
    move_to_next_char("module", s, ']');
    // trace("module: %s", l->module);

    s++;
    s++;
    l->proccess = s;
    move_to_next_char("proccess", s, ']');
    // trace("proccess: %s", l->proccess);

    move_to_next_char("ip1", s, 't');
    s++;
    l->ip = s;
    move_to_next_char("ip", s, ']');
    // trace("ip: %s", l->ip);

    s++;
    l->message = s;
    // trace("message: %s", l->message);

    return true;

error:
    return false;
}
