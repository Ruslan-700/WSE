#pragma once

#define PRIORITY_SELECTOR_SIZE 10000

namespace rgl
{
	struct priority_selector_item
	{
		float priority;
		int size_in_bytes;
		int table_no;
		int row_no;
	};

	struct priority_selector
	{
		int indices[PRIORITY_SELECTOR_SIZE];
		int cursor;
		priority_selector_item items[PRIORITY_SELECTOR_SIZE];
		int ordered_indices[PRIORITY_SELECTOR_SIZE];
		int ordered_cursor;
	};
}
