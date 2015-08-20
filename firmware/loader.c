#include "loader.h"
#include "uart.h"

#include <string.h>

extern void abort(void);

typedef void (*kentry_t)(void);

void loader_main() {
    packet_hdr_t packet_hdr;
    memset(&packet_hdr, 0, sizeof(packet_hdr_t));
    
    while(uart_getc()!=INIT_SYN);
    uart_putc(INIT_ACK);

    while(uart_getc()!=DATA_END) {
        while(uart_getc()!=DATA_SYN);
        uart_putc(DATA_ACK);
        uart_read((uint8_t*)&packet_hdr, sizeof(packet_hdr_t));
        handle_packet(&packet_hdr);
    }
    uart_puts("Successfully loaded, executing uploaded kernel...\r\n");
    kentry_t kentry = (kentry_t)0x8000;
    kentry();
}

void handle_packet(packet_hdr_t *packet_hdr) {
    uint8_t data[256];

    if(!packet_hdr) return;
    if(packet_hdr->magic!=DATA_HDR_MAGIC) return;
    if(!packet_hdr->packet_size) return;
    uart_read(data, (size_t)packet_hdr->packet_size);
    if(packet_hdr->checksum!=bsd_checksum(data, packet_hdr->packet_size)) {
        abort();
    }
}

uint8_t bsd_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    while(size--) {
        checksum = (uint8_t)(((checksum & 0xFF) >> 1) + ((checksum & 0x1) << 7));
        checksum = (uint8_t)((checksum + data[size]) &0xFF);
    }
    return checksum;
}


