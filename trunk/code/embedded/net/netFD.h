#ifndef _NET_H_
#define _NET_H_

/**
 * Initiate the Network
 */
void net_init(void);

/**
 * Set a callback function to be called when a device registers itself
 */
void net_set_newnode_cb(int (*cb)(uint16_t node));


// the service type is defined as follows:
enum {
    SERVICE_TYPE_ACCEL = 1,
    SERVICE_TYPE_MAGN  = 2,
    SERVICE_TYPE_GYRO  = 3,
    SERVICE_TYPE_TEMP  = 4,
    SERVICE_TYPE_SERVO = 5,
    SERVICE_TYPE_LEDS   = 6
};

/**
 * The structure defining the service.
 */
struct net_service {
    uint8_t id;
    uint8_t type;
    char* name;
    uint16_t (*get)(uint16_t param);
    void (*set)(uint16_t param, uint16_t config);
    struct net_service* next;
};

/**
 * Request a service listing of a node
 */
int net_list_services(uint16_t node, uint16_t filter);
/**
 * Set a callback for service list response
 */
void net_set_listresp_cb(int (*cb)(uint16_t node, uint8_t *data, int data_len));
/**
 * Get the value of a service
 */
int net_service_get(uint16_t node, uint8_t service_id, uint8_t param);
/**
 * Set a callback for a response to a get request
 */
void net_set_getresp_cb(int (*cb)(uint16_t node, uint8_t service_id, uint8_t param, uint8_t value));
/**
 * Configure a remote service
 */
int net_service_set(uint16_t node, uint8_t service_id, uint8_t param, uint8_t config);

enum {
    SERVICE_ACCEL_X = 1,
    SERVICE_ACCEL_Y = 2,
    SERVICE_ACCEL_Z = 3
};

enum {
    SERVICE_LEDS_GET = 4,
    SERVICE_LEDS_SET = 5
};

#endif
