#pragma once

#include "rgl.h"
#include "operation.h"
#include "array_util.h"

namespace wb
{
	struct simple_trigger
	{
		float interval;
		operation_manager operations;
		rgl::timer interval_timer;

		void execute(int context);
	};

	struct simple_trigger_manager
	{
		int num_simple_triggers;
		simple_trigger *simple_triggers;
		int timer_no;

		void execute(int context);
		bool has_trigger(int trigger_no) const;

		int add_trigger(const simple_trigger &newTrigger) { return array_add_elem(simple_triggers, num_simple_triggers, newTrigger); };
		bool remove_trigger(int index) {	return array_remove_elem(simple_triggers, num_simple_triggers, index); }
	};
}
