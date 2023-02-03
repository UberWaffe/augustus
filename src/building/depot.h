#ifndef BUILDING_DEPOT_H
#define BUILDING_DEPOT_H

#include "game/resource.h"

typedef enum {
	ALWAYS,
	SOURCE_HAS_AT_LEAST,
	DESTINATION_HAS_AT_MOST
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