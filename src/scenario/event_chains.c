#include "event_chains.h"

#include "core/log.h"
#include "scenario/event_chain_step.h"

#define SCENARIO_EVENT_CHAINS_SIZE_STEP 50

static array(scenario_event_chain_t) scenario_event_chains;

static int event_chain_in_use(const scenario_event_chain_t *chain)
{
    return chain->state != EVENT_CHAIN_STATE_UNDEFINED;
}

static void new_scenario_event_chain(scenario_event_chain_t *obj, int position)
{
    obj->id = position;
}

void scenario_event_chains_clear(void)
{
    scenario_event_chains.size = 0;
    if (!array_init(scenario_event_chains, SCENARIO_EVENT_CHAINS_SIZE_STEP, new_scenario_event_chain, event_chain_in_use)) {
        log_error("Unable to allocate enough memory for the scenario events array. The game will now crash.", 0, 0);
    }
}

scenario_event_chain_t *scenario_event_chains_get(int id)
{
    return array_item(scenario_event_chains, id);
}

scenario_event_chain_t *scenario_event_chains_create(void)
{
    scenario_event_chain_t *chain = 0;
    array_new_item(scenario_event_chains, 0, chain);
    if (!chain) {
        return 0;
    }
    chain->state = EVENT_CHAIN_STATE_ACTIVE;

    return chain;
}

void scenario_event_chains_delete(scenario_event_chain_t *chain)
{
    memset(chain, 0, sizeof(scenario_event_chain_t));
    chain->state = EVENT_CHAIN_STATE_UNDEFINED;
    array_trim(scenario_event_chains);
}

int scenario_event_chains_get_count(void)
{
    return scenario_event_chains.size;
}

void scenario_event_chains_save_state(buffer *blob_buffer)
{
    int32_t array_size = scenario_event_chains_get_count();
    int32_t struct_size = (4 * sizeof(int32_t));
    buffer_init_dynamic_piece(blob_buffer,
        SCENARIO_EVENT_CHAINS_CURRENT_VERSION,
        array_size,
        struct_size);
    
    scenario_event_chain_t *current;
    int link_id;
    array_foreach(scenario_event_chains, current) {
        buffer_write_i32(blob_buffer, current->id);
        buffer_write_i32(blob_buffer, current->state);

        link_id = current && current->linked_uid && current->linked_uid->id ? current->linked_uid->id : 0;
        buffer_write_i32(blob_buffer, link_id);

        link_id = current && current->initial_step && current->initial_step->id ? current->initial_step->id : 0;
        buffer_write_i32(blob_buffer, link_id);
    }
}

void scenario_event_chains_load_state(buffer *blob_buffer)
{
    int buffer_size, version, array_size, struct_size;
    buffer_load_dynamic_piece_header_data(blob_buffer,
        &buffer_size,
        &version,
        &array_size,
        &struct_size);

    for (int i = 0; i < array_size; i++) {
        scenario_event_chain_t *chain = scenario_event_chains_create();

        int saved_id = buffer_read_i32(blob_buffer);
        chain->state = buffer_read_i32(blob_buffer);

        int text_blob_id = buffer_read_i32(blob_buffer);
        chain->linked_uid = message_media_text_blob_get_entry(text_blob_id);

        int step_id = buffer_read_i32(blob_buffer);
        chain->initial_step = scenario_event_chain_steps_get(step_id);
    }
}
