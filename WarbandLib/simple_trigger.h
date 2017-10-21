#pragma once

#include "rgl.h"
#include "operation.h"

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

		int addTrigger(const simple_trigger &newTrigger);
	};
}
