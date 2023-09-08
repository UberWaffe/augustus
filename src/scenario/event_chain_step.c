#include "event_chain_step.h"

#include "core/log.h"
#include "game/save_version.h"
#include "scenario/action_types/action_handler.h"
#include "scenario/options.h"
#include "scenario/scenario.h"
#include "scenario/scenario_event.h"

#define SCENARIO_EVENT_CHAIN_STEP_SIZE_STEP 50

static array(scenario_event_chain_step_t) scenario_event_chain_steps;

static int action_in_use(const scenario_action_t *action)
{
    return action->type != ACTION_TYPE_UNDEFINED;
}

static int chain_step_in_use(const scenario_event_chain_step_t *chain_step)
{
    return chain_step->state != EVENT_CHAIN_STATE_UNDEFINED;
}

static void new_scenario_event_chain_step(scenario_event_chain_step_t *obj, int position)
{
    obj->id = position;
}

void scenario_event_chain_steps_init(void)
{
    scenario_event_chain_step_t *current;
    array_foreach(scenario_event_chain_steps, current) {
        scenario_action_t *action;
        array_foreach(current->actions, action) {
            scenario_action_type_init(action);
        }
    }
}

void scenario_event_chain_steps_clear(void)
{
    scenario_event_chain_steps.size = 0;
    if (!array_init(scenario_event_chain_steps, SCENARIO_EVENT_CHAIN_STEP_SIZE_STEP, new_scenario_event_chain_step, chain_step_in_use) ||
        !array_next(scenario_event_chain_steps)) {
        log_error("Unable to allocate enough memory for the scenario event chain steps array. The game will now crash.", 0, 0);
    }
}

scenario_event_chain_step_t *scenario_event_chain_steps_get(int id)
{
    return array_item(scenario_event_chain_steps, id);
}

scenario_event_chain_step_t *scenario_event_chain_steps_create(void)
{
    scenario_event_chain_step_t *chain_step = 0;
    array_new_item(scenario_event_chain_steps, 0, chain_step);
    if (!chain_step) {
        return 0;
    }
    chain_step->state = EVENT_CHAIN_STATE_ACTIVE;

    return chain_step;
}

void scenario_event_chain_steps_delete(scenario_event_chain_step_t *chain_step)
{
    memset(chain_step, 0, sizeof(scenario_event_chain_step_t));
    chain_step->state = EVENT_CHAIN_STATE_UNDEFINED;
    array_trim(scenario_event_chain_steps);
}

int scenario_event_chain_steps_get_count(void)
{
    return scenario_event_chain_steps.size;
}

int scenario_event_chain_steps_get_total_actions_count(void)
{
    int32_t total_count = 0;
    scenario_event_chain_step_t *current_step;
    array_foreach(scenario_event_chain_steps, current_step) {
        total_count += current_step->actions.size;
    }
    return total_count;
}

void scenario_event_chain_steps_save_state(buffer *blob_buffer)
{
    int32_t array_size = scenario_event_chain_steps_get_count();
    int32_t struct_size = (5 * sizeof(int32_t)) + (3 * sizeof(int16_t));
    buffer_init_dynamic_piece(blob_buffer,
        SCENARIO_EVENT_CHAIN_STEP_CURRENT_VERSION,
        array_size,
        struct_size);
    
    scenario_event_chain_step_t *current;
    int link_id;
    array_foreach(scenario_event_chain_steps, current) {
        buffer_write_i32(blob_buffer, current->id);
        buffer_write_i32(blob_buffer, current->state);
        buffer_write_i32(blob_buffer, current->event_chain_id);
        buffer_write_i32(blob_buffer, current->execution_count);

        buffer_write_i16(blob_buffer, current->actions.size);

        link_id = current && current->linked_uid && current->linked_uid->id ? current->linked_uid->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        link_id = current && current->message && current->message->id ? current->message->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        for (int i = 0; i < CHAIN_STEP_MAX_OPTIONS; i++) {
            link_id = current->options[i] && current->options[i]->id ? current->options[i]->id : 0;
            buffer_write_i32(blob_buffer, link_id);
        }
    }
}

void scenario_event_chain_steps_load_state(buffer *blob_buffer)
{
    int buffer_size, version, array_size, struct_size;
    buffer_load_dynamic_piece_header_data(blob_buffer,
        &buffer_size,
        &version,
        &array_size,
        &struct_size);

    for (int i = 0; i < array_size; i++) {
        scenario_event_chain_step_t *step = scenario_event_chain_steps_create();

        int saved_id = buffer_read_i32(blob_buffer);
        step->state = buffer_read_i32(blob_buffer);
        step->event_chain_id = buffer_read_i32(blob_buffer);
        step->execution_count = buffer_read_i32(blob_buffer);

        int actions_count = buffer_read_i16(blob_buffer);
        if (!array_init(step->actions, SCENARIO_ACTIONS_ARRAY_SIZE_STEP, 0, action_in_use) ||
            !array_expand(step->actions, actions_count)) {
            log_error("Unable to create actions array. The game will now crash.", 0, 0);
        }

        int text_blob_id = buffer_read_i32(blob_buffer);
        step->linked_uid = message_media_text_blob_get_entry(text_blob_id);

        int message_id = buffer_read_i32(blob_buffer);
        step->message = custom_messages_get(message_id);

        int option_id = 0;
        // Expects options to already be loaded.
        for (int i = 0; i < CHAIN_STEP_MAX_OPTIONS; i++) {
            option_id = buffer_read_i32(blob_buffer);
            step->options[i] = scenario_options_get(option_id);
        }
    }
}

void scenario_event_chain_steps_link_action(scenario_event_chain_step_t *chain_step, scenario_action_t *action)
{
    scenario_action_t *new_action = 0;
    array_new_item(chain_step->actions, 0, new_action);

    new_action->type = action->type;
    new_action->parameter1 = action->parameter1;
    new_action->parameter2 = action->parameter2;
    new_action->parameter3 = action->parameter3;
    new_action->parameter4 = action->parameter4;
    new_action->parameter5 = action->parameter5;
}
