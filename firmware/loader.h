#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <stddef.h>

/*
 * THE PROTOCOL
 * The bootloader listens on UART for INIT_SYN and sends INIT_ACK back
 * Next goes packet_hdr_t and packet after it (raw n bytes, where n=packet_size)
 */

enum {
    INIT_SYN = 0x6D, /* ('i' + 'n' + 'i' + 't')/4 */
    INIT_ACK = 0x65, /* ('a' + 'c' + 'k')/3 */
    UPLOAD_ADDRESS = 0x8000,
    DATA_HDR_MAGIC = 0xEA,
    DATA_SYN = 0xAC,
    DATA_ACK = 0xDC,
    DATA_END = 0xDE,
    DATA_ERR = 0xEE
};

typedef struct {
    uint8_t magic; /* = DATA_HDR_MAGIC */
    uint8_t packet_size;
} packet_hdr_t;

uint8_t bsd_checksum(const uint8_t *data, size_t size);
int32_t handle_packet(packet_hdr_t *packet_hdr);
void loader_main();

#endif /* LOADER_H */

