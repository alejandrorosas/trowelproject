#ifndef _NET_FRAMES_H_
#define _NET_FRAMES_H_

enum {
    FRAME_TYPE_REG   = 0x01, // register to a coordinator
    FRAME_TYPE_ACK   = 0x02, // ACK a registration
    FRAME_TYPE_LIST  = 0x11, // list the services
    FRAME_TYPE_LISTRESP  = 0x12, // list response
    FRAME_TYPE_GET   = 0x21, // get data from a service
    FRAME_TYPE_SET   = 0x31, // set config to a service
    FRAME_TYPE_GETRESP  = 0x41, // data frame after a get
    FRAME_TYPE_ASYNC = 0x51, // asynchronous data from a service
};

// standard frame
union frame {
    uint16_t dummy;
    struct {
        uint8_t type,
                data[58];
    };
};

// frames from the device
struct frame_reg {
    uint8_t type;
};

struct frame_listresp {
    uint8_t type,
            service_filter;
    uint8_t data[57];
};

struct frame_getresp {
    uint8_t type,
            service_id;
    uint16_t param,
             data;
};

struct frame_async {
    uint8_t type,
            service_id;
    uint16_t param;
    uint8_t data_len,
            data[54];
};

// frames from the coordinator
struct frame_ack {
    uint8_t type;
};

struct frame_list {
    uint8_t type,
            service_filter;
};

struct frame_get {
    uint8_t type,
            service_id;
    uint16_t param;
};

struct frame_set {
    uint8_t type,
            service_id;
    uint16_t param,
             config;
};

#endif
