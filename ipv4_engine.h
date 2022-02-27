#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define IPv4_ADDR_SIZE 15

typedef union { // https://doc.riot-os.org/unionipv4__addr__t.html
    uint8_t u8[4];
    uint32_t u32;
} ipv4_addr; // Not in big endian, as we are not transmitting anything through networks

/* Converts an IPv4 address string representation to a byte-represented IPv4 address. */
ipv4_addr *ipv4_addr_from_str(ipv4_addr *result, const char *addr);

/* Converts an IPv4 address to its string representation. */
char *ipv4_addr_to_str(char *result, const ipv4_addr *addr, uint8_t result_len);

/* Checks if two IPv4 addresses are equal. */
static bool ipv4_addr_equal(ipv4_addr *a, ipv4_addr *b);