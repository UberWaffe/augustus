#include "condition_handler.h"

#include "core/log.h"
#include "game/resource.h"
#include "scenario/condition_types/condition_types.h"
#include "scenario/options.h"
#include "scenario/scenario_event.h"
#include "scenario/scenario_events_controller.h"

void scenario_condition_type_init(scenario_condition_t *condition)
{
    switch (condition->type) {
        case CONDITION_TYPE_TIME_PASSED:
            scenario_condition_type_time_init(condition);
            break;
        default:
            break;
    }
}

int scenario_condition_type_is_met(scenario_condition_t *condition)
{
    switch (condition->type) {
        case CONDITION_TYPE_BUILDING_COUNT_ACTIVE:
            return scenario_condition_type_building_count_active_met(condition);
        case CONDITION_TYPE_BUILDING_COUNT_ANY:
            return scenario_condition_type_building_count_any_met(condition);
        case CONDITION_TYPE_BUILDING_COUNT_AREA:
            return scenario_condition_type_building_count_area_met(condition);
        case CONDITION_TYPE_CITY_POPULATION:
            return scenario_condition_type_city_population_met(condition);
        case CONDITION_TYPE_COUNT_OWN_TROOPS:
            return scenario_condition_type_count_own_troops_met(condition);
        case CONDITION_TYPE_CUSTOM_VARIABLE_CHECK:
            return scenario_condition_type_custom_variable_check_met(condition);
        case CONDITION_TYPE_DIFFICULTY:
            return scenario_condition_type_difficulty_met(condition);
        case CONDITION_TYPE_MONEY:
            return scenario_condition_type_money_met(condition);
        case CONDITION_TYPE_POPS_UNEMPLOYMENT:
            return scenario_condition_type_population_unemployed_met(condition);
        case CONDITION_TYPE_REQUEST_IS_ONGOING:
            return scenario_condition_type_request_is_ongoing_met(condition);
        case CONDITION_TYPE_RESOURCE_STORAGE_AVAILABLE:
            return scenario_condition_type_resource_storage_available_met(condition);
        case CONDITION_TYPE_RESOURCE_STORED_COUNT:
            return scenario_condition_type_resource_stored_count_met(condition);
        case CONDITION_TYPE_ROME_WAGES:
            return scenario_condition_type_rome_wages_met(condition);
        case CONDITION_TYPE_SAVINGS:
            return scenario_condition_type_savings_met(condition);
        case CONDITION_TYPE_STATS_CITY_HEALTH:
            return scenario_condition_type_stats_city_health_met(condition);
        case CONDITION_TYPE_STATS_CULTURE:
            return scenario_condition_type_stats_culture_met(condition);
        case CONDITION_TYPE_STATS_FAVOR:
            return scenario_condition_type_stats_favor_met(condition);
        case CONDITION_TYPE_STATS_PEACE:
            return scenario_condition_type_stats_peace_met(condition);
        case CONDITION_TYPE_STATS_PROSPERITY:
            return scenario_condition_type_stats_prosperity_met(condition);
        case CONDITION_TYPE_TIME_PASSED:
            return scenario_condition_type_time_met(condition);
        case CONDITION_TYPE_TRADE_ROUTE_OPEN:
            return scenario_condition_type_trade_route_open_met(condition);
        case CONDITION_TYPE_TRADE_ROUTE_PRICE:
            return scenario_condition_type_trade_route_price_met(condition);
        case CONDITION_TYPE_TRADE_SELL_PRICE:
            return scenario_condition_type_trade_sell_price_met(condition);
        case CONDITION_TYPE_TAX_RATE:
            return scenario_condition_type_tax_rate_met(condition);
        default:
            // If we cannot figure condition type (such as with deleted conditions) then default to passed.
            return 1;
    }
}

void scenario_condition_type_delete(scenario_condition_t *condition)
{
    memset(condition, 0, sizeof(scenario_condition_t));
    condition->type = CONDITION_TYPE_UNDEFINED;
}

static void save_state(buffer *buf, scenario_condition_t *condition, int link_type, int32_t link_id)
{
    buffer_write_i16(buf, link_type);
    buffer_write_i32(buf, link_id);
    buffer_write_i16(buf, condition->type);
    buffer_write_i32(buf, condition->parameter1);
    buffer_write_i32(buf, condition->parameter2);
    buffer_write_i32(buf, condition->parameter3);
    buffer_write_i32(buf, condition->parameter4);
    buffer_write_i32(buf, condition->parameter5);
}

void scenario_conditions_save_state(buffer *buf)
{
    int32_t array_size = scenario_events_get_total_conditions_count()
        + scenario_options_get_total_conditions_count();

    int32_t struct_size = (2 * sizeof(int16_t)) + (6 * sizeof(int32_t));
    buffer_init_dynamic_piece(buf,
        SCENARIO_CONDITIONS_VERSION,
        array_size,
        struct_size);

    int event_count = scenario_events_get_count();
    for (int i = 0; i < event_count; i++) {
        scenario_event_t *current_event = scenario_event_get(i);

        for (int j = 0; j < current_event->conditions.size; j++) {
            scenario_condition_t *current_condition = array_item(current_event->conditions, j);
            save_state(buf, current_condition, LINK_TYPE_SCENARIO_EVENT, current_event->id);
        }
    }

    int options_count = scenario_options_get_count();
    for (int i = 0; i < options_count; i++) {
        scenario_option_t *current_option = scenario_options_get(i);

        for (int j = 0; j < current_option->visible_conditions.size; j++) {
            scenario_condition_t *current_condition = array_item(current_option->visible_conditions, j);
            save_state(buf, current_condition, LINK_TYPE_SCENARIO_OPTION_VISIBLE_CONDITIONS, current_option->id);
        }
        for (int k = 0; k < current_option->enabled_conditions.size; k++) {
            scenario_condition_t *current_condition = array_item(current_option->enabled_conditions, k);
            save_state(buf, current_condition, LINK_TYPE_SCENARIO_OPTION_ENABLED_CONDITIONS, current_option->id);
        }
    }
}

static void load_link_condition(scenario_condition_t *condition, int link_type, int32_t link_id)
{
    switch (link_type) {
        case LINK_TYPE_SCENARIO_EVENT:
            {
                scenario_event_t *event = scenario_event_get(link_id);
                scenario_event_link_condition(event, condition);
            }
            break;
        case LINK_TYPE_SCENARIO_OPTION_VISIBLE_CONDITIONS:
        case LINK_TYPE_SCENARIO_OPTION_ENABLED_CONDITIONS:
            {
                scenario_option_t *option = scenario_options_get(link_id);
                scenario_options_link_condition(option, condition, link_type);
            }
            break;
        default:
            log_error("Unhandled condition link type. The game will probably crash.", 0, 0);
            break;
    }
}

static void load_state(buffer *buf, scenario_condition_t *condition, int *link_type, int32_t *link_id)
{
    *link_type = buffer_read_i16(buf);
    *link_id = buffer_read_i32(buf);
    condition->type = buffer_read_i16(buf);
    condition->parameter1 = buffer_read_i32(buf);
    condition->parameter2 = buffer_read_i32(buf);
    condition->parameter3 = buffer_read_i32(buf);
    condition->parameter4 = buffer_read_i32(buf);
    condition->parameter5 = buffer_read_i32(buf);

    if (condition->type == CONDITION_TYPE_TRADE_SELL_PRICE) {
        condition->parameter1 = resource_remap(condition->parameter1);
    } else if (condition->type == CONDITION_TYPE_RESOURCE_STORED_COUNT) {
        condition->parameter1 = resource_remap(condition->parameter1);
    } else if (condition->type == CONDITION_TYPE_RESOURCE_STORAGE_AVAILABLE) {
        condition->parameter1 = resource_remap(condition->parameter1);
    }
}

void scenario_conditions_load_state(buffer *buf)
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
        scenario_condition_t condition;
        load_state(buf, &condition, &link_type, &link_id);
        load_link_condition(&condition, link_type, link_id);
    }
}
