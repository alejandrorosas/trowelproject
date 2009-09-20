#include <io.h>
#include "csma.h"
#include "netED.h"
#include "net_frames.h"

/*-----DEFINES----*/
#define STATE_UNREGISTERED 0x1
#define STATE_REGISTERED   0x2
#ifndef NULL
 #define NULL 0x0
#endif

/*---PROTOTYPES---*/
static int register_frame_received(void);
static int frame_received(void);
static int send_service_list(struct frame_list* f, int len);
static int service_get(struct frame_get* f, int len);
static int service_set(struct frame_set* f, int len);
static inline struct net_service* find_service(uint8_t service_id);

/*-------DATA-----*/
static uint16_t state;
static struct net_service *service_list;
static uint16_t service_count;
static int (*register_done_cb)(void);
static uint16_t coord_addr;

/*----PUBLIC FUNCTIONS----*/
void net_init(void) {
    // init mac layer
    csma_init();
    
    // init state and service list
    state = STATE_UNREGISTERED;
    service_list = NULL;
    service_count = 0;
    register_done_cb = NULL;
    coord_addr = 0;
}

int net_register(void) {
    struct frame_reg reg;
    
    // prepare frame
    reg.type = FRAME_TYPE_REG;
    
    if (csma_send(CSMA_BROADCAST, (uint8_t*)&reg, sizeof(reg))) {
        // if send is happening
        // configure mac rx callback
        csma_set_rx_cb(register_frame_received);
        
        return 1;
    }
    return 0;
}

void net_set_register_cb(int (*cb)(void)) {
    register_done_cb = cb;
}

void net_add_service(struct net_service *new_service) {
    struct net_service* temp;
    service_count++;
    new_service->next = NULL;
    new_service->id = (uint8_t)service_count;
    
    if (service_list == NULL) {
        service_list = new_service;
    } else {
        temp = service_list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_service;
    }
}

/*---PRIVATE FUNCTIONS---*/
static int register_frame_received(void) {
    // a frame has been received, we're hoping for an ACK frame
    struct frame_ack ack;
    uint16_t from;
    
    if ( !csma_read(&from, (uint8_t*)&ack, sizeof(ack)) ) {
        return 0;
    }
    
    // check if frame type corresponds
    if (ack.type == FRAME_TYPE_ACK) {
        // store coordinator address
        coord_addr = from;
        // update state
        state = STATE_REGISTERED;
        // change callback for frame received
        csma_set_rx_cb(frame_received);
        // call callback
        if (register_done_cb) {
            return register_done_cb();
        }
    }
    return 0;
}

static int frame_received(void) {
    union frame f;
    uint16_t from;
    int len;
    
    len = csma_read(&from, (uint8_t*)&f, sizeof(f));
    if ( len == 0 ) {
        return 0;
    }
    
    if (from != coord_addr) {
        return 0;
    }
    
    switch (f.type) {
    case FRAME_TYPE_LIST:
        return send_service_list((struct frame_list*)&f, len);
    case FRAME_TYPE_GET:
        return service_get((struct frame_get*)&f, len);
    case FRAME_TYPE_SET:
        return service_set((struct frame_set*)&f, len);
    }
    
    return 0;
}


static int send_service_list(struct frame_list* f, int len) {
    struct frame_listresp fresp;
    struct net_service *service;
    int data_len;
    
    if (len != sizeof(struct frame_list)) {
        return 0;
    }
    
    fresp.type = FRAME_TYPE_LISTRESP;
    data_len = 0;
    service = service_list;
    
    while (service != NULL) {
        if ( (f->service_filter == 0xFF) ||(service->type == f->service_filter) ) {
            fresp.data[data_len] = service->type;
            fresp.data[data_len+1] = service->id;
            data_len+=2;
        }
    }
    
    csma_send(coord_addr, (uint8_t*) &fresp, 2+data_len);
    return 0;
}

static int service_get(struct frame_get* f, int len) {
    struct frame_getresp fresp;
    struct net_service *service;
    
    // check length
    if (len != sizeof(struct frame_get)) {
        return 0;
    }
    
    // try to find service
    service = find_service(f->service_id);
    
    if (service == NULL) {
        return 0;
    }
    
    fresp.type = FRAME_TYPE_GETRESP;
    fresp.service_id = f->service_id;
    fresp.param = f->param;
    fresp.data = service->get(fresp.param);
    
    return 0;
}
    
static int service_set(struct frame_set* f, int len) {
    struct net_service *service;
    
    // check length
    if (len != sizeof(struct frame_set)) {
        return 0;
    }
    
     // try to find service
    service = find_service(f->service_id);
    
    if (service == NULL) {
        return 0;
    }
    
    service->set(f->param, f->config);
    return 0;
}

static inline struct net_service* find_service(uint8_t service_id) {
    struct net_service *temp;
    
    temp = service_list;
    while ( (temp->id != service_id) && (temp->next != NULL) ) {
        temp = temp->next;
    }
    
    if (temp->id == service_id) {
        return temp;
    } else {
        return NULL;
    }
}
