#ifndef SCENARIO_EVENT_CHAINS_H
#define SCENARIO_EVENT_CHAINS_H

#include "scenario/scenario_event_data.h"

typedef enum {
    SCENARIO_EVENT_CHAINS_CURRENT_VERSION = 1,

    SCENARIO_EVENT_CHAINS_VERSION_NONE = 0,
    SCENARIO_EVENT_CHAINS_VERSION_INITIAL = 1,
} scenario_event_chains_version;

void scenario_event_chains_clear(void);
scenario_event_chain_t *scenario_event_chains_get(int id);
scenario_event_chain_t *scenario_event_chains_create(void);
void scenario_event_chains_delete(scenario_event_chain_t *chain);
int scenario_event_chains_get_count(void);

void scenario_event_chains_save_state(buffer *blob_buffer);
void scenario_event_chains_load_state(buffer *blob_buffer);

#endif // SCENARIO_EVENT_CHAINS_H
