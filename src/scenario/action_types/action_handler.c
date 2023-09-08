#include "action_handler.h"

#include "core/log.h"
#include "game/resource.h"
#include "scenario/action_types/action_types.h"
#include "scenario/event_chain_step.h"
#include "scenario/options.h"
#include "scenario/scenario_event.h"
#include "scenario/scenario_events_controller.h"

void scenario_action_type_init(scenario_action_t *action)
{
    switch (action->type) {
        case ACTION_TYPE_ADJUST_CITY_HEALTH:
            scenario_action_type_city_health_init(action);
            break;
        case ACTION_TYPE_ADJUST_MONEY:
            scenario_action_type_money_add_init(action);
            break;
        case ACTION_TYPE_ADJUST_ROME_WAGES:
            scenario_action_type_rome_wages_init(action);
            break;
        case ACTION_TYPE_ADJUST_SAVINGS:
            scenario_action_type_savings_add_init(action);
            break;
        default:
            break;
    }
}

int scenario_action_type_execute(scenario_action_t *action)
{
    switch (action->type) {
        case ACTION_TYPE_ADJUST_CITY_HEALTH:
            return scenario_action_type_city_health_execute(action);
        case ACTION_TYPE_ADJUST_FAVOR:
            return scenario_action_type_favor_add_execute(action);
        case ACTION_TYPE_ADJUST_ROME_WAGES:
            return scenario_action_type_rome_wages_execute(action);
        case ACTION_TYPE_ADJUST_MONEY:
            return scenario_action_type_money_add_execute(action);
        case ACTION_TYPE_ADJUST_SAVINGS:
            return scenario_action_type_savings_add_execute(action);
        case ACTION_TYPE_BUILDING_FORCE_COLLAPSE:
            return scenario_action_type_building_force_collapse_execute(action);
        case ACTION_TYPE_CHANGE_ALLOWED_BUILDINGS:
            return scenario_action_type_change_allowed_buildings_execute(action);
        case ACTION_TYPE_CHANGE_CITY_RATING:
            return scenario_action_type_change_city_rating_execute(action);
        case ACTION_TYPE_CHANGE_CUSTOM_VARIABLE:
            return scenario_action_type_change_custom_variable_execute(action);
        case ACTION_TYPE_CHANGE_RESOURCE_PRODUCED:
            return scenario_action_type_change_resource_produced_execute(action);
        case ACTION_TYPE_CHANGE_RESOURCE_STOCKPILES:
            return scenario_action_type_change_resource_stockpiles_execute(action);
        case ACTION_TYPE_EMPIRE_MAP_CONVERT_FUTURE_TRADE_CITY:
            return scenario_action_type_empire_map_convert_future_trade_city_execute(action);
        case ACTION_TYPE_GLADIATOR_REVOLT:
            return scenario_action_type_gladiator_revolt_execute(action);
        case ACTION_TYPE_INVASION_IMMEDIATE:
            return scenario_action_type_invasion_immediate_execute(action);
        case ACTION_TYPE_REQUEST_IMMEDIATELY_START:
            return scenario_action_type_request_immediately_start_execute(action);
        case ACTION_TYPE_SEND_STANDARD_MESSAGE:
            return scenario_action_type_send_standard_message_execute(action);
        case ACTION_TYPE_TRADE_ROUTE_ADD_NEW_RESOURCE:
            return scenario_action_type_trade_add_new_resource_execute(action);
        case ACTION_TYPE_TRADE_ADJUST_PRICE:
            return scenario_action_type_trade_price_adjust_execute(action);
        case ACTION_TYPE_TRADE_ADJUST_ROUTE_AMOUNT:
            return scenario_action_type_trade_route_amount_execute(action);
        case ACTION_TYPE_TRADE_ADJUST_ROUTE_OPEN_PRICE:
            return scenario_action_type_trade_route_adjust_open_price_execute(action);
        case ACTION_TYPE_TRADE_ROUTE_SET_OPEN:
            return scenario_action_type_trade_route_open_execute(action);
        case ACTION_TYPE_TRADE_PROBLEM_LAND:
            return scenario_action_type_trade_problems_land_execute(action);
        case ACTION_TYPE_TRADE_PROBLEM_SEA:
            return scenario_action_type_trade_problems_sea_execute(action);
        case ACTION_TYPE_TRADE_SET_PRICE:
            return scenario_action_type_trade_price_set_execute(action);
        case ACTION_TYPE_TRADE_SET_BUY_PRICE_ONLY:
            return scenario_action_type_trade_set_buy_price_execute(action);
        case ACTION_TYPE_TRADE_SET_SELL_PRICE_ONLY:
            return scenario_action_type_trade_set_sell_price_execute(action);
        case ACTION_TYPE_SHOW_CUSTOM_MESSAGE:
            return scenario_action_type_show_custom_message_execute(action);
        case ACTION_TYPE_TAX_RATE_SET:
            return scenario_action_type_tax_rate_set_execute(action);
        default:
            return 0;
    }
}

void scenario_action_type_delete(scenario_action_t *action)
{
    memset(action, 0, sizeof(scenario_action_t));
    action->type = ACTION_TYPE_UNDEFINED;
}

static void save_state(buffer *buf, const scenario_action_t *action, int link_type, int32_t link_id)
{
    buffer_write_i16(buf, link_type);
    buffer_write_i32(buf, link_id);
    buffer_write_i16(buf, action->type);
    buffer_write_i32(buf, action->parameter1);
    buffer_write_i32(buf, action->parameter2);
    buffer_write_i32(buf, action->parameter3);
    buffer_write_i32(buf, action->parameter4);
    buffer_write_i32(buf, action->parameter5);
}

void scenario_actions_save_state(buffer *buf)
{
    int32_t array_size = scenario_events_get_total_actions_count()
        + scenario_options_get_total_actions_count();

    int32_t struct_size = (2 * sizeof(int16_t)) + (6 * sizeof(int32_t));
    buffer_init_dynamic_piece(buf,
        SCENARIO_ACTIONS_VERSION,
        array_size,
        struct_size);

    int event_count = scenario_events_get_count();
    for (int i = 0; i < event_count; i++) {
        scenario_event_t *current_event = scenario_event_get(i);

        for (int j = 0; j < current_event->actions.size; j++) {
            scenario_action_t *current_action = array_item(current_event->actions, j);
            save_state(buf, current_action, LINK_TYPE_SCENARIO_EVENT, current_event->id);
        }
    }

    int options_count = scenario_options_get_count();
    for (int i = 0; i < options_count; i++) {
        scenario_option_t *current_option = scenario_options_get(i);

        for (int j = 0; j < current_option->actions.size; j++) {
            scenario_action_t *current_action = array_item(current_option->actions, j);
            save_state(buf, current_action, LINK_TYPE_SCENARIO_OPTION, current_option->id);
        }
    }
}

static void load_link_action(scenario_action_t *action, int link_type, int32_t link_id)
{
    switch (link_type) {
        case LINK_TYPE_SCENARIO_EVENT:
            {
                scenario_event_t *event = scenario_event_get(link_id);
                scenario_event_link_action(event, action);
            }
            break;
        case LINK_TYPE_SCENARIO_OPTION:
            {
                scenario_option_t *option = scenario_options_get(link_id);
                scenario_options_link_action(option, action);
            }
            break;
        case LINK_TYPE_SCENARIO_EVENT_CHAIN_STEP:
            {
                scenario_event_chain_step_t *chain_step = scenario_event_chain_steps_get(link_id);
                scenario_event_chain_steps_link_action(chain_step, action);
            }
            break;
        default:
            log_error("Unhandled action link type. The game will probably crash.", 0, 0);
            break;
    }
}

static void load_state(buffer *buf, scenario_action_t *action, int *link_type, int32_t *link_id)
{
    *link_type = buffer_read_i16(buf);
    *link_id = buffer_read_i32(buf);
    action->type = buffer_read_i16(buf);
    action->parameter1 = buffer_read_i32(buf);
    action->parameter2 = buffer_read_i32(buf);
    action->parameter3 = buffer_read_i32(buf);
    action->parameter4 = buffer_read_i32(buf);
    action->parameter5 = buffer_read_i32(buf);

    if (action->type == ACTION_TYPE_CHANGE_RESOURCE_PRODUCED) {
        action->parameter1 = resource_remap(action->parameter1);
    } else if (action->type == ACTION_TYPE_TRADE_ADJUST_PRICE) {
        action->parameter1 = resource_remap(action->parameter1);
    } else if (action->type == ACTION_TYPE_TRADE_ADJUST_ROUTE_AMOUNT) {
        action->parameter2 = resource_remap(action->parameter2);
    } else if (action->type == ACTION_TYPE_TRADE_ROUTE_ADD_NEW_RESOURCE) {
        action->parameter2 = resource_remap(action->parameter2);
    } else if (action->type == ACTION_TYPE_TRADE_SET_PRICE) {
        action->parameter1 = resource_remap(action->parameter1);        
    } else if (action->type == ACTION_TYPE_TRADE_SET_BUY_PRICE_ONLY) {
        action->parameter1 = resource_remap(action->parameter1);        
    } else if (action->type == ACTION_TYPE_TRADE_SET_SELL_PRICE_ONLY) {
        action->parameter1 = resource_remap(action->parameter1);        
    }
}

void scenario_actions_load_state(buffer *buf)
{
    int buffer_size, version, array_size, struct_size;
    buffer_load_dynamic_piece_header_data(buf,
        &buffer_size,
        &version,
        &array_size,
        &struct_size);

    int link_type = 0;
    int32_t link_id = 0;
    for (int i = 0; i < array_size; i++) {
        scenario_action_t action;
        load_state(buf, &action, &link_type, &link_id);
        load_link_action(&action, link_type, link_id);
    }
}
