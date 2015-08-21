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

    while(1) {
        uint8_t c = uart_getc();
        if(c==DATA_END)
            break;
        else if(c!=DATA_SYN)
            continue;
        uart_read((uint8_t*)&packet_hdr, sizeof(packet_hdr_t));
        if(handle_packet(&packet_hdr)) {
            uart_putc(DATA_ERR);
            return;
        }
        uart_putc(DATA_ACK);
    }
    uart_puts("Successfully loaded, executing uploaded kernel...\r\n");
    kentry_t kentry = (kentry_t)UPLOAD_ADDRESS;
    kentry();
}

int32_t handle_packet(packet_hdr_t *packet_hdr) {
    uint8_t data[256];
    static uint32_t counter = UPLOAD_ADDRESS;
    uint32_t err = 0;
    if(!packet_hdr) 
        err = 1;
    if(packet_hdr->magic!=DATA_HDR_MAGIC) 
        err = 1;
    if(!packet_hdr->packet_size) 
        err = 1;
    uart_read(data, (size_t)packet_hdr->packet_size);
    if(!err) {
        memcpy((void*)counter, data, (size_t)packet_hdr->packet_size);
        counter+=packet_hdr->packet_size;
    }
    return err;
}

