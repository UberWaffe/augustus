#ifndef SCENARIO_EVENT_DATA_H
#define SCENARIO_EVENT_DATA_H

#include "core/array.h"
#include "scenario/custom_messages.h"

#include <stdint.h>

#define SCENARIO_EVENTS_ARRAY_SIZE_STEP 100
#define SCENARIO_ACTIONS_ARRAY_SIZE_STEP 20
#define SCENARIO_CONDITIONS_ARRAY_SIZE_STEP 20

#define CHAIN_STEP_MAX_OPTIONS 10

typedef enum {
    EVENT_STATE_UNDEFINED = 0,
    EVENT_STATE_DISABLED = 1,
    EVENT_STATE_ACTIVE = 2,
    EVENT_STATE_PAUSED = 3,
    EVENT_STATE_DELETED = 4
} event_state;

typedef enum {
    EVENT_CHAIN_STATE_UNDEFINED = 0,
    EVENT_CHAIN_STATE_ACTIVE = 1,
    EVENT_CHAIN_STATE_DISABLED = 2,
    EVENT_CHAIN_STATE_DELETED = 3
} event_chain_state;

typedef enum {
    CONDITION_TYPE_UNDEFINED = 0,
    CONDITION_TYPE_TIME_PASSED = 1,
    CONDITION_TYPE_DIFFICULTY = 2,
    CONDITION_TYPE_MONEY = 3,
    CONDITION_TYPE_SAVINGS = 4,
    CONDITION_TYPE_STATS_FAVOR = 5,
    CONDITION_TYPE_STATS_PROSPERITY = 6,
    CONDITION_TYPE_STATS_CULTURE = 7,
    CONDITION_TYPE_STATS_PEACE = 8,
    CONDITION_TYPE_TRADE_SELL_PRICE = 9,
    CONDITION_TYPE_POPS_UNEMPLOYMENT = 10,
    CONDITION_TYPE_ROME_WAGES = 11,
    CONDITION_TYPE_CITY_POPULATION = 12,
    CONDITION_TYPE_BUILDING_COUNT_ACTIVE = 13,
    CONDITION_TYPE_STATS_CITY_HEALTH = 14,
    CONDITION_TYPE_COUNT_OWN_TROOPS = 15,
    CONDITION_TYPE_REQUEST_IS_ONGOING = 16,
    CONDITION_TYPE_TAX_RATE = 17,
    CONDITION_TYPE_BUILDING_COUNT_ANY = 18,
    CONDITION_TYPE_CUSTOM_VARIABLE_CHECK = 19,
    CONDITION_TYPE_TRADE_ROUTE_OPEN = 20,
    CONDITION_TYPE_TRADE_ROUTE_PRICE = 21,
    CONDITION_TYPE_RESOURCE_STORED_COUNT = 22,
    CONDITION_TYPE_RESOURCE_STORAGE_AVAILABLE = 23,
    CONDITION_TYPE_BUILDING_COUNT_AREA = 24,
    CONDITION_TYPE_MAX,
    // helper constants
    CONDITION_TYPE_MIN = CONDITION_TYPE_TIME_PASSED,
} condition_types;

typedef enum {
    ACTION_TYPE_UNDEFINED = 0,
    ACTION_TYPE_ADJUST_FAVOR = 1,
    ACTION_TYPE_ADJUST_MONEY = 2,
    ACTION_TYPE_ADJUST_SAVINGS = 3,
    ACTION_TYPE_TRADE_ADJUST_PRICE = 4,
    ACTION_TYPE_TRADE_PROBLEM_LAND = 5,
    ACTION_TYPE_TRADE_PROBLEM_SEA = 6,
    ACTION_TYPE_TRADE_ADJUST_ROUTE_AMOUNT = 7,
    ACTION_TYPE_ADJUST_ROME_WAGES = 8,
    ACTION_TYPE_GLADIATOR_REVOLT = 9,
    ACTION_TYPE_CHANGE_RESOURCE_PRODUCED = 10,
    ACTION_TYPE_CHANGE_ALLOWED_BUILDINGS = 11,
    ACTION_TYPE_SEND_STANDARD_MESSAGE = 12,
    ACTION_TYPE_ADJUST_CITY_HEALTH = 13,
    ACTION_TYPE_TRADE_SET_PRICE = 14,
    ACTION_TYPE_EMPIRE_MAP_CONVERT_FUTURE_TRADE_CITY = 15,
    ACTION_TYPE_REQUEST_IMMEDIATELY_START = 16,
    ACTION_TYPE_SHOW_CUSTOM_MESSAGE = 17,
    ACTION_TYPE_TAX_RATE_SET = 18,
    ACTION_TYPE_CHANGE_CUSTOM_VARIABLE = 19,
    ACTION_TYPE_TRADE_ADJUST_ROUTE_OPEN_PRICE = 20,
    ACTION_TYPE_CHANGE_CITY_RATING = 21,
    ACTION_TYPE_CHANGE_RESOURCE_STOCKPILES = 22,
    ACTION_TYPE_TRADE_ROUTE_SET_OPEN = 23,
    ACTION_TYPE_TRADE_ROUTE_ADD_NEW_RESOURCE = 24,
    ACTION_TYPE_TRADE_SET_BUY_PRICE_ONLY = 25,
    ACTION_TYPE_TRADE_SET_SELL_PRICE_ONLY = 26,
    ACTION_TYPE_BUILDING_FORCE_COLLAPSE = 27,
    ACTION_TYPE_INVASION_IMMEDIATE = 28,
    ACTION_TYPE_MAX,
    // helper constants
    ACTION_TYPE_MIN = ACTION_TYPE_ADJUST_FAVOR,
} action_types;

typedef enum {
    LINK_TYPE_UNDEFINED = -1,
    LINK_TYPE_SCENARIO_EVENT = 0,
    LINK_TYPE_SCENARIO_OPTION = 1,
    LINK_TYPE_SCENARIO_OPTION_VISIBLE_CONDITIONS = 2,
    LINK_TYPE_SCENARIO_OPTION_ENABLED_CONDITIONS = 3,
    LINK_TYPE_SCENARIO_EVENT_CHAIN_STEP = 4,
    LINK_TYPE_SCENARIO_EVENT_CHAIN = 5
} link_type_t;

enum {
    COMPARISON_TYPE_UNDEFINED = 0,
    COMPARISON_TYPE_EQUAL = 1,
    COMPARISON_TYPE_EQUAL_OR_LESS = 2,
    COMPARISON_TYPE_EQUAL_OR_MORE = 3,
    COMPARISON_TYPE_NOT_EQUAL = 4,
    COMPARISON_TYPE_LESS_THAN = 5,
    COMPARISON_TYPE_GREATER_THAN = 6
};

enum {
    POP_CLASS_UNDEFINED = 0,
    POP_CLASS_ALL = 1,
    POP_CLASS_PATRICIAN = 2,
    POP_CLASS_PLEBEIAN = 3,
    POP_CLASS_SLUMS = 4
};

typedef struct {
    condition_types type;
    int parameter1;
    int parameter2;
    int parameter3;
    int parameter4;
    int parameter5;
} scenario_condition_t;

typedef struct {
    action_types type;
    int parameter1;
    int parameter2;
    int parameter3;
    int parameter4;
    int parameter5;
} scenario_action_t;

typedef struct {
    int id;
    event_state state;
    int repeat_months_min;
    int repeat_months_max;
    int max_number_of_repeats;
    int execution_count;
    int months_until_active;
    array(scenario_condition_t) conditions;
    array(scenario_action_t) actions;
} scenario_event_t;

typedef struct {
    int id;
    int in_use;
    const text_blob_string_t *label;
    const text_blob_string_t *tooltip_enabled;
    const text_blob_string_t *tooltip_disabled;
    array(scenario_condition_t) visible_conditions;
    array(scenario_condition_t) enabled_conditions;
    array(scenario_action_t) actions;
    int linked_id;
} scenario_option_t;

typedef struct {
    int id;
    event_chain_state state;
    int event_chain_id;
    int execution_count;
    const text_blob_string_t *linked_uid;
    const custom_message_t *message;
    array(scenario_action_t) actions;
    scenario_option_t* options[CHAIN_STEP_MAX_OPTIONS];
} scenario_event_chain_step_t;

typedef struct {
    int id;
    event_chain_state state;
    const text_blob_string_t *linked_uid;
    const scenario_event_chain_step_t* initial_step;
} scenario_event_chain_t;

#endif // SCENARIO_EVENT_DATA_H
