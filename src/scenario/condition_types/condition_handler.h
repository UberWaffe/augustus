#ifndef CONDITION_HANDLER_H
#define CONDITION_HANDLER_H

#include "core/buffer.h"
#include "scenario/scenario_event_data.h"

typedef enum {
    SCENARIO_CONDITIONS_VERSION = 1,

    SCENARIO_CONDITIONS_VERSION_NONE = 0,
    SCENARIO_CONDITIONS_VERSION_INITIAL = 1,
} scenario_conditions_version;

void scenario_condition_type_init(scenario_condition_t *condition);
int scenario_condition_type_is_met(scenario_condition_t *condition);

void scenario_condition_type_delete(scenario_condition_t *condition);
void scenario_conditions_save_state(buffer *buf);
void scenario_conditions_load_state(buffer *buf);

#endif // CONDITION_HANDLER_H
