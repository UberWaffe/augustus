#ifndef BUILDING_DEPOT_H
#define BUILDING_DEPOT_H

#include "game/resource.h"

typedef enum {
	ORDER_CONDITION_NEVER,
	ORDER_CONDITION_ALWAYS,
	ORDER_CONDITION_SOURCE_HAS_MORE_THAN,
	ORDER_CONDITION_DESTINATION_HAS_LESS_THAN
} order_condition_type;

typedef struct {
	resource_type resource_type;
	int src_storage_id;
	int dst_storage_id;
	struct {
		order_condition_type condition_type;
		int threshold;
	} condition;
} order;

#endif // BUILDING_DEPOT_H