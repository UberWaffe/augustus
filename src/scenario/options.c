#include "options.h"

#include "core/log.h"
#include "game/save_version.h"
#include "scenario/action_types/action_handler.h"
#include "scenario/condition_types/condition_handler.h"
#include "scenario/scenario.h"
#include "scenario/scenario_event.h"

#define SCENARIO_OPTIONS_SIZE_STEP 50

static array(scenario_option_t) scenario_options;

static int option_in_use(const scenario_option_t *option)
{
    return option->in_use;
}

static void new_scenario_option(scenario_option_t *obj, int position)
{
    obj->id = position;
}

static int action_in_use(const scenario_action_t *action)
{
    return action->type != ACTION_TYPE_UNDEFINED;
}

static int condition_in_use(const scenario_condition_t *condition)
{
    return condition->type != CONDITION_TYPE_UNDEFINED;
}

void scenario_options_init(void)
{
    scenario_option_t *current;
    array_foreach(scenario_options, current) {
        scenario_condition_t *condition;
        array_foreach(current->visible_conditions, condition) {
            scenario_condition_type_init(condition);
        }
        array_foreach(current->enabled_conditions, condition) {
            scenario_condition_type_init(condition);
        }

        scenario_action_t *action;
        array_foreach(current->actions, action) {
            scenario_action_type_init(action);
        }
    }
}

void scenario_options_clear(void)
{
    scenario_options.size = 0;
    if (!array_init(scenario_options, SCENARIO_OPTIONS_SIZE_STEP, new_scenario_option, option_in_use) ||
        !array_next(scenario_options)) {
        log_error("Unable to allocate enough memory for the scenario options array. The game will now crash.", 0, 0);
    }
}

scenario_option_t *scenario_options_get(int id)
{
    return array_item(scenario_options, id);
}

scenario_option_t *scenario_options_create(void)
{
    scenario_option_t *option = 0;
    array_new_item(scenario_options, 0, option);
    if (!option) {
        return 0;
    }
    option->in_use = 1;

    return option;
}

void scenario_options_delete(scenario_option_t *option)
{
    memset(option, 0, sizeof(scenario_option_t));
    option->in_use = 0;
    array_trim(scenario_options);
}

int scenario_options_get_count(void)
{
    return scenario_options.size;
}

int scenario_options_get_total_conditions_count(void)
{
    int32_t total_count = 0;
    scenario_option_t *current_option;
    array_foreach(scenario_options, current_option) {
        total_count += current_option->visible_conditions.size;
        total_count += current_option->enabled_conditions.size;
    }
    return total_count;
}

int scenario_options_get_total_actions_count(void)
{
    int32_t total_count = 0;
    scenario_option_t *current_option;
    array_foreach(scenario_options, current_option) {
        total_count += current_option->actions.size;
    }
    return total_count;
}

void scenario_options_save_state(buffer *blob_buffer)
{
    int32_t array_size = scenario_options_get_count();
    int32_t struct_size = (6 * sizeof(int32_t)) + (3 * sizeof(int16_t));
    buffer_init_dynamic_piece(blob_buffer,
        SCENARIO_OPTIONS_CURRENT_VERSION,
        array_size,
        struct_size);
    
    scenario_option_t *current;
    int link_id;
    array_foreach(scenario_options, current) {
        buffer_write_i32(blob_buffer, current->id);
        buffer_write_i32(blob_buffer, current->in_use);
        buffer_write_i32(blob_buffer, current->linked_id);

        link_id = current && current->label && current->label->id ? current->label->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        link_id = current && current->tooltip_enabled && current->tooltip_enabled->id ? current->tooltip_enabled->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        link_id = current && current->tooltip_disabled && current->tooltip_disabled->id ? current->tooltip_disabled->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        buffer_write_i16(blob_buffer, current->visible_conditions.size);
        buffer_write_i16(blob_buffer, current->enabled_conditions.size);
        buffer_write_i16(blob_buffer, current->actions.size);
    }
}

void scenario_options_load_state(buffer *blob_buffer)
{
    int buffer_size, version, array_size, struct_size;
    buffer_load_dynamic_piece_header_data(blob_buffer,
        &buffer_size,
        &version,
        &array_size,
        &struct_size);

    for (int i = 0; i < array_size; i++) {
        scenario_option_t *option = scenario_options_create();

        int saved_id = buffer_read_i32(blob_buffer);
        option->in_use = buffer_read_i32(blob_buffer);
        option->linked_id = buffer_read_i32(blob_buffer);

        int text_blob_id = buffer_read_i32(blob_buffer);
        option->label = message_media_text_blob_get_entry(text_blob_id);

        text_blob_id = buffer_read_i32(blob_buffer);
        option->tooltip_enabled = message_media_text_blob_get_entry(text_blob_id);

        text_blob_id = buffer_read_i32(blob_buffer);
        option->tooltip_disabled = message_media_text_blob_get_entry(text_blob_id);

        int visible_conditions_count = buffer_read_i16(blob_buffer);
        int enabled_conditions_count = buffer_read_i16(blob_buffer);
        int actions_count = buffer_read_i16(blob_buffer);

        if (!array_init(option->visible_conditions, SCENARIO_CONDITIONS_ARRAY_SIZE_STEP, 0, condition_in_use) ||
            !array_expand(option->visible_conditions, visible_conditions_count)) {
            log_error("Unable to create conditions array. The game will now crash.", 0, 0);
        }
        if (!array_init(option->enabled_conditions, SCENARIO_CONDITIONS_ARRAY_SIZE_STEP, 0, condition_in_use) ||
            !array_expand(option->enabled_conditions, enabled_conditions_count)) {
            log_error("Unable to create conditions array. The game will now crash.", 0, 0);
        }
        if (!array_init(option->actions, SCENARIO_ACTIONS_ARRAY_SIZE_STEP, 0, action_in_use) ||
            !array_expand(option->actions, actions_count)) {
            log_error("Unable to create actions array. The game will now crash.", 0, 0);
        }
        if (option->id != saved_id) {
            log_error("Loaded option id does not match what it was saved with. The game will likely crash. option->id: ", 0, option->id);
        }
    }
}

void scenario_options_link_condition(scenario_option_t *option, scenario_condition_t *condition, link_type_t type)
{
    scenario_condition_t *new_condition = 0;
    if (type == LINK_TYPE_SCENARIO_OPTION_VISIBLE_CONDITIONS) {
        array_new_item(option->visible_conditions, 0, new_condition);
    } else {
        array_new_item(option->enabled_conditions, 0, new_condition);
    }
    new_condition->type = condition->type;
    new_condition->parameter1 = condition->parameter1;
    new_condition->parameter2 = condition->parameter2;
    new_condition->parameter3 = condition->parameter3;
    new_condition->parameter4 = condition->parameter4;
    new_condition->parameter5 = condition->parameter5;
}

void scenario_options_link_action(scenario_option_t *option, scenario_action_t *action)
{
    scenario_action_t *new_action = 0;
    array_new_item(option->actions, 0, new_action);

    new_action->type = action->type;
    new_action->parameter1 = action->parameter1;
    new_action->parameter2 = action->parameter2;
    new_action->parameter3 = action->parameter3;
    new_action->parameter4 = action->parameter4;
    new_action->parameter5 = action->parameter5;
}
