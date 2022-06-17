# asn_lookup
This a project that creates a Trie data structure for fast mapping an IP to a
ASN. It is able to do 9million queries per second on a macbook air.

The implementations consist of a trie data structure based on binary
information of the ASN. So, alphabet size is 2 (1 or 0)

for example:
   ASN 213.37.0.0/16 (6739) - 11010101001001010000000000000000
   asns->leafs[1] -> asns->leafs[1] -> asns->leafs[0] -> ... 16 bits -> ASN
Data needed is obtained from:
http://thyme.apnic.net/current/data-raw-table
http://thyme.apnic.net/current/data-used-autnums

No IPv6 Support yet

see http://www.geeksforgeeks.org/trie-insert-and-search/

A few tools that I might need in the future

https://github.com/criticalstack/libevhtp
https://faker.readthedocs.io/en/master/index.html
