#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

#include "core/buffer.h"
#include "scenario/scenario_event_data.h"

typedef enum {
    SCENARIO_ACTIONS_VERSION = 1,

    SCENARIO_ACTIONS_VERSION_NONE = 0,
    SCENARIO_ACTIONS_VERSION_INITIAL = 1,
} scenario_actions_version;

void scenario_action_type_init(scenario_action_t *action);
int scenario_action_type_execute(scenario_action_t *action);

void scenario_action_type_delete(scenario_action_t *action);
void scenario_actions_save_state(buffer *buf);
void scenario_actions_load_state(buffer *buf);

#endif // ACTION_HANDLER_H
