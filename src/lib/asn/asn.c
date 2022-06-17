/**
 * @file asn.c
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief This files contains implemention for asn.h
 *
 */
#include <asn/asn.h>
#include <core/core.h>
#include <stdio.h>
#include <string.h>

#define ERROR_PARAM "Param Error"
#define ERROR_NO_MEMORY "Cannot allocate Memory"

/**
 * Returns the bit at position pos on an ntohl in_addr struct number
 */
#define BIT(var, pos) (((var) >> (31 - pos)) & 1)

// ----- Generic IP functions ------
void in_addr_to_binary_s(const struct in_addr ip, char *bits) {
    error_if(bits == NULL, error, ERROR_PARAM);

    uint32_t n = ntohl(ip.s_addr);

    for (int i = 0; i < 32; i++) {
        bits[i] = BIT(n, i) ? '1' : '0';
    }

    bits[32] = 0;

error:
    return;
}

void in_addr_to_binary(const struct in_addr ip, bool *bits) {
    error_if(bits == NULL, error, ERROR_PARAM);

    uint32_t n = ntohl(ip.s_addr);

    for (int i = 0; i < 32; i++) {
        bits[i] = BIT(n, i) ? 1 : 0;
    }

error:
    return;
}

// ------- ASN -------
struct asn {
    struct in_addr ip;
    uint8_t bits;
    uint32_t number;
};

struct asn *asn_alloc(const char *network, size_t number) {
    struct asn *asn = NULL;

    char *host = strdup(network);
    strtok(host, "/");
    char *bits = strtok(NULL, "/");

    asn = malloc(sizeof(struct asn));
    error_if(NULL == asn, error, ERROR_NO_MEMORY);

    struct in_addr in;
    int valid = inet_aton(host, &in);
    error_if(0 == valid, error, "Error parsing network host: %s [%s]", host,
             network);

    asn->bits = atoi(bits);
    asn->number = number;
    asn->ip = in;

    free(host);

    return asn;

error:
    ALLOC_FREE(host);
    asn_free(asn);
    return NULL;
}

void asn_free(struct asn *asn) {
    ALLOC_FREE(asn);
}

uint32_t asn_number(const struct asn *asn) {
    error_if(NULL == asn, error, ERROR_PARAM);

    return asn->number;

error:
    return 0;
}

uint8_t asn_mask(const struct asn *asn) {
    error_if(NULL == asn, error, ERROR_PARAM);

    return asn->bits;

error:
    return 0;
}

char *asn_ip_s(const struct asn *asn) {
    error_if(NULL == asn, error, ERROR_PARAM);

    return inet_ntoa((asn)->ip);

error:
    return NULL;
}

void asn_to_binary(const struct asn *asn, bool *bits) {
    error_if(NULL == asn, error, ERROR_PARAM);
    error_if(NULL == bits, error, ERROR_PARAM);

    in_addr_to_binary(asn->ip, bits);

error:
    return;
}

void asn_to_binary_s(const struct asn *asn, char *bits) {
    error_if(NULL == asn, error, ERROR_PARAM);
    error_if(NULL == bits, error, ERROR_PARAM);

    in_addr_to_binary_s(asn->ip, bits);

error:
    return;
}

bool asn_has_ip(const struct asn *asn, const char *ip_s) {
    error_if(asn == NULL, error, ERROR_PARAM);
    error_if(ip_s == NULL, error, ERROR_PARAM);

    uint32_t ip, net, mask;
    struct in_addr ip_addr;
    net = ntohl(asn->ip.s_addr);

    inet_aton(ip_s, &ip_addr);
    ip = ntohl(ip_addr.s_addr); // Machine ENDIAN fix

    mask = -1 << (32 - asn->bits);
    net &= mask;
    return ((ip & mask) == net);

error:
    return false;
}

// -------- ASN TABLE ------
/** Struct that containts asn name and user data for calculations */
struct asn_names_and_data {
    void *data; /** User defined data */
    char *name; /** ASN name*/
};
/** List of asn names and user data */
struct asn_names {
    struct asn_names_and_data **names; /** Array of allocated number of ASNs */
    size_t allocated;                  /** Number of allocated ASNs */
};

struct asn_index {
    struct asn_index *leafs[2];
    unsigned depth; /** Tree depth Max values = 32 for IPv4 */
    struct asn *asn;
};

struct asn_table {
    struct asn_names *names;
    struct asn_index *index;
};

// -- Private/Static functions for ASN TABLE --
static void asn_table_init_raw(struct asn_table *table, const char *fname);
static void asn_table_init_used(struct asn_table *table, const char *fname);
static void asn_table_free_index_loop(struct asn_index *index);
static struct asn *asn_table_find_by_ip_loop(const struct asn_index *index,
        const uint32_t ip, const int depth,
        struct asn *last_found);
static void asn_table_print_loop(const struct asn_index *index,
                                 struct asn_names_and_data *names[],
                                 char bits[]);

static struct asn_names_and_data *asn_name_by_asnum(struct asn_names *names,
        const uint32_t asnum);

// -- Public/EXternal functions for ASN_TABLE --
struct asn_table *asn_table_alloc() {
    struct asn_table *table = NULL;

    table = ALLOC_INIT(struct asn_table, {NULL, NULL});
    error_if(NULL == table, error, ERROR_NO_MEMORY);

    table->index = ALLOC_INIT(struct asn_index, {{NULL, NULL}, 0, NULL});
    error_if(NULL == table->index, error, ERROR_NO_MEMORY);

    // Allocating array of names
    table->names = ALLOC_INIT(struct asn_names, {NULL, 0});
    error_if(NULL == table->names, error, ERROR_NO_MEMORY);

    table->names->names =
        calloc(ASN_NAMES_NUMBER_OF_ASNS, sizeof(struct asn_names_and_data *));
    error_if(NULL == table->names->names, error,
             "Cannot allocate memory for %zu names", table->names->allocated);
    table->names->allocated = ASN_NAMES_NUMBER_OF_ASNS;

    return table;

error:
    asn_table_free(table);
    return NULL;
}

void asn_table_free(struct asn_table *table) {
    if (NULL == table) {
        return;
    }

    if (NULL != table->names->names) {
        for (size_t i = 0; i < table->names->allocated; i++) {
            ALLOC_FREE(table->names->names[i]->name);
            ALLOC_FREE(table->names->names[i]->data);
            ALLOC_FREE(table->names->names[i]);
        }
    }

    ALLOC_FREE(table->names->names);
    ALLOC_FREE(table->names);

    asn_table_free_index_loop(table->index);
    ALLOC_FREE(table);
}

struct asn_table *asn_table_init(const char *data_raw_table,
                                 const char *data_used_autnums) {

    struct asn_table *table = asn_table_alloc();
    error_if(NULL == table, error, ERROR_NO_MEMORY);

    asn_table_init_raw(table, data_raw_table);
    asn_table_init_used(table, data_used_autnums);

    return table;

error:
    asn_table_free(table);
    return NULL;
}

/*
 * Build the bit trie = tree data structure
 */
void asn_table_add_asn(struct asn_table *table, struct asn *asn) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->index, error, ERROR_PARAM);
    error_if(NULL == asn, error, ERROR_PARAM);

    unsigned N = asn_mask(asn);
    uint32_t ip = ntohl(asn->ip.s_addr); // Ensure endiannes is ok
    struct asn_index *current = table->index;

    unsigned i;

    for (i = 0; i < N; i++) {
        bool bitv = BIT(ip, i);

        if (NULL == current->leafs[bitv]) {
            // trace("Creating new node: %u depth leaf: %u", i, bitv);
            current->leafs[bitv] = ALLOC_INIT(
            struct asn_index, {{NULL, NULL}, .depth = i + 1, NULL});

            error_if(NULL == current->leafs[bitv], error, ERROR_NO_MEMORY);
        }

        current = current->leafs[bitv];
    }

    current->asn = asn;

error:
    return;
}

void asn_table_add_name(struct asn_table *table, const uint32_t number,
                        const char *name) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);
    error_if(NULL == name, error, ERROR_PARAM);

    struct asn_names *names = table->names;

    if (number > names->allocated) {
        char **new_names;
        new_names = realloc(names->names, number);
        error_if(NULL == new_names, error,
                 "Error reallocating memory %zu -> %u elements",
                 names->allocated, number);
        names->allocated = number;
    }

    names->names[number] = ALLOC_INIT(struct asn_names_and_data,
    {.data = NULL, .name = strdup(name)});

error:
    return;
}

struct asn *asn_table_find_by_ip(const struct asn_table *table,
                                 const struct in_addr in) {

    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->index, error, ERROR_PARAM);
    uint32_t ip = ntohl(in.s_addr);
    return asn_table_find_by_ip_loop(table->index, ip, 0, NULL);

error:
    return NULL;
}

struct asn *asn_table_find_by_ip_s(const struct asn_table *table,
                                   const char *ip_s) {

    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == ip_s, error, ERROR_PARAM);

    struct in_addr in;
    int valid = inet_aton(ip_s, &in);
    error_if(valid == 0, error, "Error parsing ip: >%s<", ip_s);

    return asn_table_find_by_ip(table, in);

error:
    return NULL;
}

char *asn_table_find_name_by_asnum(const struct asn_table *table,
                                   const uint32_t asnum) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);

    struct asn_names_and_data *nd = asn_name_by_asnum(table->names, asnum);
    error_if(NULL == nd, error, "Cannot find ASN asnum: %d", asnum);
    return nd->name;

error:
    return "Unknown";
}

void *asn_table_find_user_data_by_asnum(const struct asn_table *table,
                                        const uint32_t asnum) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);

    struct asn_names_and_data *nd = asn_name_by_asnum(table->names, asnum);
    error_if(NULL == nd, error, "Cannot find ASN asnum: %d", asnum);

    return nd->data;

error:
    return NULL;
}

// AQUI
void *asn_table_init_asnum_user_data(struct asn_table *table,
                                     const uint32_t asnum, void *data) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);
    error_if(NULL == data, error, ERROR_PARAM);

    struct asn_names_and_data *nd = asn_name_by_asnum(table->names, asnum);
    warn_if(NULL != nd, "asnum %d user_data alredy initialized", asnum);

    nd->data = data;
    return nd->data;

error:
    return NULL;
}

void asn_table_loop(const struct asn_table *table, asn_table_loop_cb_t cb,
                    void *ctxt) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);
    error_if(NULL == cb, error, ERROR_PARAM);

    for (size_t i = 0; i < table->names->allocated; i++) {
        cb(i, table->names->names[i]->name, table->names->names[i]->data, ctxt);
    }

error:
    return;
}

void asn_table_free_user_data(struct asn_table *table) {
    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == table->names, error, ERROR_PARAM);

    for (size_t i = 0; i < table->names->allocated; i++) {
        if (NULL == table->names->names[i]->data) {
            free(table->names->names[i]->data);
        }
    }

error:
    return;
}

void asn_table_print(const struct asn_table *table) {
    char bits[33] = "00000000000000000000000000000000";
    asn_table_print_loop(table->index, table->names->names, bits);
}

// ---------------------------------------------------------------------------
// --- Private/Static functions implementation
static void asn_table_init_raw(struct asn_table *table, const char *fname) {
    FILE *fp = NULL;
    size_t lines = 0;
    char *network = {0};
    uint32_t number = 0;
    struct asn *asn = NULL;

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == fname, error, ERROR_PARAM);

    fp = fopen(fname, "r");
    error_if(NULL == fp, error, "Cannot read %s", fname);
    trace("Reading ASN network and number from raw data file: %s", fname);

    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        trim_eol(line);

        network = strtok(line, "\t");
        number = atoi(strtok(NULL, "\t"));

        // trace("network: %s number: %d", network, number);
        asn = asn_alloc(network, number);

        if (NULL == asn) {
            warn("Error in line %zu: [%s] [%d]", lines, network, number);
        } else {
            asn_table_add_asn(table, asn);
        }

        lines++;
    }

    trace("%zu asns added", lines);

error:
    ALLOC_FREE(line);
    fclose(fp);
}

static void asn_table_init_used(struct asn_table *table, const char *fname) {
    FILE *fp = NULL;
    size_t lines = 0;
    char *name = NULL;
    uint32_t number = 0;
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    error_if(NULL == table, error, ERROR_PARAM);
    error_if(NULL == fname, error, ERROR_PARAM);

    trace("Reading ASN used and name from raw data file: %s", fname);
    fp = fopen(fname, "r");
    error_if(NULL == fp, error, "Cannot read %s", fname);

    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        trim_eol(line);

        number = atoi(line);
        name = &line[7];

        asn_table_add_name(table, number, name);
        lines++;
    }

    trace("%zu used asns names added", lines);

error:
    ALLOC_FREE(line);
    fclose(fp);
}

static void asn_table_free_index_loop(struct asn_index *index) {
    if (NULL == index) {
        return;
    }

    asn_free(index->asn);
    asn_table_free_index_loop(index->leafs[0]);
    asn_table_free_index_loop(index->leafs[1]);

    ALLOC_FREE(index);
}

static struct asn *asn_table_find_by_ip_loop(const struct asn_index *index,
        const uint32_t ip, const int depth,
        struct asn *last_found) {

    error_if(index == NULL, error, ERROR_PARAM);

    if (depth > 31) {
        return last_found;
    }

    if (index->asn) {
        last_found = index->asn;
    }

    bool bit = BIT(ip, depth);

    if (!index->leafs[bit]) {
        return last_found;
    } else
        return asn_table_find_by_ip_loop(index->leafs[bit], ip, depth + 1,
                                         last_found);

error:
    return NULL;
}

static void asn_table_print_loop(const struct asn_index *index,
                                 struct asn_names_and_data *names[],
                                 char bits[]) {
    error_if(NULL == names, error, ERROR_PARAM);
    error_if(NULL == index, error, ERROR_PARAM);

    struct asn *asn = index->asn;

    if (NULL != asn) {
        char binary[33];
        asn_to_binary_s(asn, binary);
        printf("%s/%d %u %s - %s: %s\n", inet_ntoa(asn->ip), asn->bits,
               asn->number, bits, binary, names[asn->number]->name);
    }

    if (NULL != index->leafs[1]) {
        bits[index->depth] = '1';
        asn_table_print_loop(index->leafs[1], names, bits);
    }

    if (NULL != index->leafs[0]) {
        bits[index->depth] = '0';
        asn_table_print_loop(index->leafs[0], names, bits);
    }

    bits[index->depth] = '0';

error:
    return;
}

static struct asn_names_and_data *asn_name_by_asnum(struct asn_names *names,
        const uint32_t asnum) {
    error_if(NULL == names, error, ERROR_PARAM);
    error_if(asnum > names->allocated, error, "Out Of Range: %u > %lu", asnum,
             names->allocated);
    error_if(NULL == names->names[asnum], error, "NULL Name for ASN: %u",
             asnum);

    return names->names[asnum];

error:
    return NULL;
}
