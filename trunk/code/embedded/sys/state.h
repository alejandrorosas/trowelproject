#ifndef _STATE_H_
#define _STATE_H_

#define ST_EVENT_QUEUE_LENGTH 8

struct st_chain {
	int8_t name;
	
	struct st_event *first_evt;
	struct st_state *first_state;
	struct st_transition *first_transition;

	struct st_state *active_state;
}

struct st_event {
	void * data;
	struct st_event* next;
};

struct st_state {
	void (*state_exec)(struct st_event* evt);
	struct st_state* next;
};

struct st_transition {
	struct st_state *from_state, *to_state;
	struct st_event *evt;
	struct st_transition *next
};

/**
 * Initiate the state flow mechanism.
 */
void st_init(void);

/**
 * Instantiate a new state chain, and fill its members.
 * \param The chain to add.
 */
void st_add_chain(struct st_chain* chain);

/**
 * Add an event to a chain, an fill its members.
 * \param chain The chain to add the event to.
 * \param evt A pointer to the event to add.
 */
void st_add_event(struct st_chain *chain, struct st_event *evt);

/**
 * Add a state to a chain, an fill its members.
 * \param chain The chain to add the event to.
 * \param state A pointer to the state to add.
 */
void st_add_state(struct st_chain *chain, struct st_event *st);

/**
 * Add a transition to a chain, and fills its members.
 * \param chain The chain to add the transition to.
 * \param transition The transition to add.
 */
void st_add_transition(struct st_chain *chain, struct st_transition *transition);

/**
 * Notify the system to process an incoming event.
 * \param struct st_chain *chain The chain on which apply the event.
 * \param evt A pointer to the event that happened.
 */
struct void st_notify_event(struct st_chain *chain, struct st_event *evt);

/**
 * Execute and set a state as active in a chain.
 * \param chain The chain to query.
 * \param state A pointer to the state to process.
 */
struct void st_set_active_state(struct st_chain *chain, struct st_state *state);



#endif
