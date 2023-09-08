#ifndef SCENARIO_OPTIONS_H
#define SCENARIO_OPTIONS_H

#include "scenario/scenario_event_data.h"

typedef enum {
    SCENARIO_OPTIONS_CURRENT_VERSION = 1,

    SCENARIO_OPTIONS_VERSION_NONE = 0,
    SCENARIO_OPTIONS_VERSION_INITIAL = 1,
} scenario_options_version;

void scenario_options_init(void);
void scenario_options_clear(void);
scenario_option_t *scenario_options_get(int id);
scenario_option_t *scenario_options_create(void);
void scenario_options_delete(scenario_option_t *option);
int scenario_options_get_count(void);
int scenario_options_get_total_conditions_count(void);
int scenario_options_get_total_actions_count(void);

void scenario_options_save_state(buffer *blob_buffer);
void scenario_options_load_state(buffer *blob_buffer);
void scenario_options_link_condition(scenario_option_t *option, scenario_condition_t *condition, link_type_t type);
void scenario_options_link_action(scenario_option_t *option, scenario_action_t *action);

#endif // SCENARIO_OPTIONS_H
