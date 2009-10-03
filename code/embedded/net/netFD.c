#include <io.h>
#include "csma.h"
#include "netFD.h"
#include "net_frames.h"
#include "event.h"

/*-----DEFINES----*/
#ifndef NULL
 #define NULL 0x0
#endif

/*---PROTOTYPES---*/
static int frame_received_evt(void);
static void frame_received(void* data);
static int handle_register_request(uint16_t from, struct frame_reg *f, int len);
static int handle_getresp(uint16_t from, struct frame_getresp *f, int len);
static int handle_listresp(uint16_t from, struct frame_listresp *f, int len);
static inline int isnodeknown(uint16_t addr);

/*-------DATA-----*/
int (*newnode_cb)(uint16_t node);
int (*listresp_cb)(uint16_t node, uint8_t *data, int data_len);
int (*getresp_cb)(uint16_t node, uint8_t service_id, uint8_t param, uint8_t value);

#define MAX_NODES 10
uint16_t nodes[MAX_NODES];



/*----PUBLIC FUNCTIONS----*/
void net_init(void) {
    printf("net_init\n");
    int i;
    // init mac layer
    csma_init();
    csma_set_rx_cb(frame_received);
    
    // clear node list
    for (i=0;i<MAX_NODES; i++) nodes[i] = 0x0;
}

void net_set_newnode_cb(int (*cb)(uint16_t node)) {
    newnode_cb = cb;
}

int net_list_services(uint16_t node, uint16_t filter) {
    struct frame_list f;
    
    // check node is known
    if (!isnodeknown(node)) {
        return 0;
    }
    
    f.type = FRAME_TYPE_LIST;
    f.service_filter = filter;
    
    return csma_send(node, (uint8_t*)&f, sizeof(struct frame_list));
}

void net_set_listresp_cb(int (*cb)(uint16_t node, uint8_t *data, int data_len)) {
    listresp_cb = cb;
}

int net_service_get(uint16_t node, uint8_t service_id, uint8_t param) {
    struct frame_get f;
    
    // check node is known
    if (!isnodeknown(node)) {
        return 0;
    }
    
    f.type = FRAME_TYPE_GET;
    f.service_id = service_id;
    f.param = param;
    
    return csma_send(node, (uint8_t*)&f, sizeof(struct frame_get));
}

void net_set_getresp_cb(int (*cb)(uint16_t node, uint8_t service_id, uint8_t param, uint8_t value)) {
    getresp_cb = cb;
}

int net_service_set(uint16_t node, uint8_t service_id, uint8_t param, uint8_t config) {
    struct frame_set f;
    
    // check node is known
    if (!isnodeknown(node)) {
        return 0;
    }
    
    f.type = FRAME_TYPE_SET;
    f.service_id = service_id;
    f.param = param;
    f.config = config;
    
    return csma_send(node, (uint8_t*)&f, sizeof(struct frame_set));
}


/*---PRIVATE FUNCTIONS---*/
static int frame_received_evt(void) {
    event_post(frame_received, 0);
    return 1;
}
 
static void frame_received(void* data) {
    printf("net_frame_received\n");
    union frame f;
    uint16_t from;
    int len;
    
    len = csma_read(&from, (uint8_t*)&f, sizeof(f));
    if ( len == 0 ) {
        return 0;
    }
    
    switch (f.type) {
    case FRAME_TYPE_REG:
        return handle_register_request(from, (struct frame_reg*)&f, len);
    case FRAME_TYPE_GETRESP:
        return handle_getresp(from, (struct frame_getresp*)&f, len);
    case FRAME_TYPE_LISTRESP:
        return handle_listresp(from, (struct frame_listresp*)&f, len);
    }
    
    return 0;
}

static int handle_register_request(uint16_t from, struct frame_reg *f, int len) {
    struct frame_ack f;
    
    // check length
    if (len != sizeof(struct frame_reg)) {
        return 0;
    }
    
    f.type = FRAME_TYPE_ACK;
    csma_send(from, (uint8_t*)from, sizeof(struct frame_ack));
    
    // if callback, call it
    if (newnode_cb) {
        return newnode_cb(from);
    }
    
    return 0;
}

static int handle_getresp(uint16_t from, struct frame_getresp *f, int len) {
    // check length
    if (len != sizeof(struct frame_getresp)) {
        return 0;
    }
    
    // if callback, call it
    if (getresp_cb) {
        return getresp_cb(from, f->service_id, f->param, f->data);
    }
    
    return 0;
}

static int handle_listresp(uint16_t from, struct frame_listresp *f, int len) {
    // check length
    if (len != sizeof(struct frame_listresp)) {
        return 0;
    }
    
    // if callback, call it
    if (listresp_cb) {
        return listresp_cb(from, f->data, len-2);
    }
    
    return 0;
}

static inline int isnodeknown(uint16_t addr) {
    int i;
    for (i=0; i<MAX_NODES; i++) {
        if (addr == nodes[i]) {
            return 1;
        }
    }
    return 0;
}
