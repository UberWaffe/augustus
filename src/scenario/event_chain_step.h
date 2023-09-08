#ifndef SCENARIO_EVENT_CHAIN_STEP_H
#define SCENARIO_EVENT_CHAIN_STEP_H

#include "scenario/scenario_event_data.h"

typedef enum {
    SCENARIO_EVENT_CHAIN_STEP_CURRENT_VERSION = 1,

    SCENARIO_EVENT_CHAIN_STEP_VERSION_NONE = 0,
    SCENARIO_EVENT_CHAIN_STEP_VERSION_INITIAL = 1,
} scenario_event_chain_step_version;

void scenario_event_chain_steps_init(void);
void scenario_event_chain_steps_clear(void);
scenario_event_chain_step_t *scenario_event_chain_steps_get(int id);
scenario_event_chain_step_t *scenario_event_chain_steps_create(void);
void scenario_event_chain_steps_delete(scenario_event_chain_step_t *chain_step);
int scenario_event_chain_steps_get_count(void);
int scenario_event_chain_steps_get_total_actions_count(void);

void scenario_event_chain_steps_save_state(buffer *blob_buffer);
void scenario_event_chain_steps_load_state(buffer *blob_buffer);
void scenario_event_chain_steps_link_action(scenario_event_chain_step_t *chain_step, scenario_action_t *action);

#endif // SCENARIO_EVENT_CHAIN_STEP_H
