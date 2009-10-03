#ifndef _EVENT_H_
#define _EVENT_H_

/**
 * Initialize the event processing system.
 */
void event_init(void);

/**
 * The event prototype.
 * \param data the data to pass to the event.
 */
typedef void (*event_t)(void* data);

/**
 * Post an event.
 * \param event the function prototype to post.
 */
void event_post(event_t event, void* data);

/**
 * Process the waiting events.
 */
void event_process(void);

#endif
