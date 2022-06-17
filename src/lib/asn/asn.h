#pragma once
/**
 * @file asn.h
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief This contains the asn data and asn tree for data structure definitions
 *
 * The implementations consist of a trie data structure based on binary
 * information of the ASN. So, alphabet size is 2 (1 or 0)
 *
 * for example:
 *    ASN 213.37.0.0/16 (6739) - 11010101001001010000000000000000
 *    asns->leafs[1] -> asns->leafs[1] -> asns->leafs[0] -> ... 16 bits -> ASN
 * Data needed is obtained from:
 * http://thyme.apnic.net/current/data-raw-table
 * http://thyme.apnic.net/current/data-used-autnums
 *
 * No IPv6 Support yet
 *
 * @see http://www.geeksforgeeks.org/trie-insert-and-search/
 */
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Fills the string of bits in the bits array passed (currently allocated)
 */
void in_addr_to_binary_s(const struct in_addr ip, char* bits);

/**
 * fills the array of boolean bits in the bits array passed (currently
 * allocated)
 */
void in_addr_to_binary(const struct in_addr ip, bool* bits);

/**
 * Contains ASN Information
 */
struct asn;

/**
 * Builds an asn struct from network and ans number.
 * Network should come as IP/Mask -> 1.2.3.4/24
 */
struct asn* asn_alloc(const char* network, size_t number);
void asn_free(struct asn* asn);
uint32_t asn_number(const struct asn* asn);
uint8_t asn_mask(const struct asn* asn);
/** Returns string representing the IP */
char* asn_ip_s(const struct asn* asn);
/** Fills bits bool array (32 elements) of an asn struct */
void asn_to_binary(const struct asn* asn, bool* bits);
/** Fills bits char array (33 elements) == string of an asn struct */
void asn_to_binary_s(const struct asn* asn, char* bits);
/** Check whether an IP is part of the ASN Network */
bool asn_has_ip(const struct asn* asn, const char* ip_s);
// void asn_print(const struct asn* asn);

/**
 * Total number os allocated asns.
 * count lines at misc/data-used-autnums.txt
 */
#define ASN_NAMES_NUMBER_OF_ASNS (460000)

/**
 * Contains ASN database.
 */
struct asn_table;

void asn_table_free(struct asn_table* table);

/**
 * Create new asn_table from: data-raw-table.txt & data-used-autnums.txt
 * downloaded from:
 *
 * http://thyme.apnic.net/current/data-raw-table
 * http://thyme.apnic.net/current/data-used-autnums
 *
 * RAW TABLE:
 * Network\tasnum
 * 1.0.4.0/24\t56203
 *
 * USED AUTNUMS
 * as_num\tas_code - as_name
 * 1\tLVLT-1 - Level 3 Communications, Inc., US
 *
 * @param data_raw_table raw data file path
 * @param data_used_autnums used autnums file path
 */
struct asn_table* asn_table_init(const char* data_raw_table,
                                 const char* data_used_autnums);

struct asn* asn_table_find_by_ip(const struct asn_table* table,
                                 const struct in_addr in);
struct asn* asn_table_find_by_ip_s(const struct asn_table* table,
                                   const char* ip_s);

char* asn_table_find_name_by_asnum(const struct asn_table* table,
                                   const uint32_t asnum);

#define asn_table_find_name_by_asn(table, asn)                                 \
    asn_table_find_name_by_asnum(table, asn_asnum(asn))

void* asn_table_find_user_data_by_asnum(const struct asn_table* table,
                                        const uint32_t asnum);
#define asn_table_find_user_data_by_asn(table, asn)                            \
    asn_table_find_user_data_by_asnum(table, asn_asnum(asn))

void* asn_table_init_asnum_user_data(struct asn_table* table,
                                     const uint32_t asnum, void* data);
#define asn_table_init_asn_user_data(table, asn, data)                         \
    asn_table_init_asnum_user_data(table, asn_number(asn), data)

/**
 * Callback for asn_table_loop. It will be called only for asns that contains
 * user_data.
 *
 * @param asnum ASN number
 * @param name ASN name
 * @param user data for the ASN (set and defined previously by user)
 * @param ctxt Global data for the loop (ser defined)
 * */
typedef void (*asn_table_loop_cb_t)(const uint32_t asnum, const char* name,
                                    void* user_data, void* ctxt);
/** Loop asn_table for registers that contains user data */
void asn_table_loop(const struct asn_table* table, asn_table_loop_cb_t cb,
                    void* ctxt);
/** free user data */
void asn_table_free_user_data(struct asn_table* table);

/** Print asn table to stdout (debug pourposes) */
void asn_table_print(const struct asn_table* table);
