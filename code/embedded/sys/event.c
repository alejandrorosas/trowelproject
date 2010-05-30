#include <io.h>
#include "event.h"

/* DEFINES */
#define EVENT_QUEUE_SIZE 8

/* DATA */
struct event_data {
    event_t func;
    void* data;
};
static struct event_data event_queue[EVENT_QUEUE_SIZE];
static int16_t queue_start, queue_len;

/* FUNCTIONS */

void event_init(void) {
    queue_start = 0;
    queue_len = 0;
}

critical void event_post(event_t event, void* data) {
    int16_t queue_pt;
    
    if (queue_len < EVENT_QUEUE_SIZE) {
        queue_pt = queue_start + queue_len;
        queue_pt %= EVENT_QUEUE_SIZE;
        
        event_queue[queue_pt].func = event;
        event_queue[queue_pt].data = data;
        queue_len += 1;
    }
}

static critical inline void queue_remove(void) {
    queue_len -= 1;
    queue_start += 1;
    queue_start %= EVENT_QUEUE_SIZE;
}

void event_process(void) {
    
    while (queue_len) {
        // call the event
        (event_queue[queue_start].func)(event_queue[queue_start].data);
        // remove event from the list
        queue_remove();
    }
}
