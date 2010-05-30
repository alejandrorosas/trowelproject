#include <io.h>
#include "event.h"
#include "state.h"

// Private Structures
struct st_event_data {
	void (*func)(struct st_event* evt);
	struct st_event* data;
};

// Private Function Prototypes
static void exec_state(struct st_state *state, struct st_event *event);
static void exec_func(void* data);
static struct st_event_data* get_available_dataslot(void);

// Private variables
static struct st_event_data event_data_queue[ST_EVENT_QUEUE_LENGTH];
static int16_t event_data_queue_search_start;


void st_init(void) {
	int16_t i;

	for (i = 0; i < ST_EVENT_QUEUE_LENGTH; i++) {
		event_data_queue[i].func = 0x0;
		event_data_queue[i].data = 0x0;
	}

	event_data_queue_search_start = 0;
}

void st_init_chain(struct st_chain* chain) {
	chain->first_evt = 0x0;
	chain->first_state = 0x0;
	chain->first_transition = 0x0;

	chain->active_state = 0x0;
}

void st_add_event(struct st_chain *chain, struct st_event *evt) {
	
	if (chain->first_evt == 0x0) {
		chain->first_evt = evt;
		evt->next = 0x0;
	} else {
		evt->next = chain->first_evt;
		chain->first_evt = evt;
	}
}

void st_add_state(struct st_chain *chain, struct st_state *st) {
	
	if (chain->first_state == 0x0) {
		chain->first_state = st;
		st->next = 0x0;
	} else {
		st->next = chain->first_state;
		chain->first_state = st;
	}
}

void st_add_transition(struct st_chain *chain, struct st_transition *transition) {
	
	if (chain->first_transition == 0x0) {
		chain->first_transition = transition;
		transition->next = 0x0;
	} else {
		transition->next = chain->first_transition;
		chain->first_transition = transition;
	}
}

void st_notify_event(struct st_chain *chain, struct st_event *evt) {
	struct st_state *active;
	struct st_transition *transition;

	// get the active state of this chain
	active = chain->active_state;

	// check if there is at least one transition
	if (chain->first_transition == 0x0) {
		return;
	}
	
	// loop over the transitions of this chain to find one mathing
	for (transition = chain->first_transition;
			transition != 0x0;
			transition = transition->next) {
		if ( ( (transition->from_state == active) ||
					(transition->from_state == 0x0) ) &&
				(transition->event == evt) ) {
			// we found the transition!
			// call the state
			exec_state(transition->to_state, evt);
			// update the active state pointer
			chain->active_state = transition->to_state;
			return;
		}
	}
}

void st_set_active_state(struct st_chain* chain, struct st_state *state) {
	
	// call the state
	exec_state(state, 0x0);
	// update the active state pointer
	chain->active_state = state;
}

static struct st_event_data* get_available_dataslot(void) {
	int16_t i, ix;

	for (i = 0; i < ST_EVENT_QUEUE_LENGTH; i++) {
		ix = (i + event_data_queue_search_start) % ST_EVENT_QUEUE_LENGTH;
		if (event_data_queue[ix].func == 0x0) {
			// found a place!
			// update search queue
			event_data_queue_search_start += 1;
			event_data_queue_search_start %= ST_EVENT_QUEUE_LENGTH;

			return &event_data_queue[ix];
		}
	}
	return 0x0;
}

static void exec_state(struct st_state* state, struct st_event* event) {
	struct st_event_data *data;

	// get a data slot
	data = get_available_dataslot();
	if (data == 0x0)
		return;

	// store data in it
	data->func = state->exec;
	data->data = event;

	// post state execution
	event_post(exec_func, data);
}



static void exec_func(void* data) {
	struct st_event_data *event_data;

	// cast the data structure
	event_data = (struct st_event_data *) data;

	// call the function
	(event_data->func)(event_data->data);

	// clear the data slot
	event_data->func = 0x0;
	event_data->data = 0x0;
}
