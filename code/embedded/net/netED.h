#ifndef _NETED_H_
#define _NETED_H_

/**
 * Initiate the Network
 */
void net_init(void);

/**
 * Register the device on the network to a coordinator.
 */
int net_register(void);

/**
 * Set a callback function to be called when the device
 * is registered to a coordinator.
 */
void net_set_register_cb(int (*cb)(void));


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
 * Declare a service to the net module.
 */
void net_add_service(struct net_service *new_service);

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
