#include "simple_trigger.h"
#include "warband.h"
#include "array_util.h"

using namespace wb;

void simple_trigger::execute(int context)
{
	if (this->interval >= -1.0)
	{
		if (context >= 0 && this->interval <= this->interval_timer.get_elapsed_time())
		{
			this->interval_timer.update();
			this->operations.execute(context);
		}
	}
	else if (round_half_up(this->interval) == context)
	{
		this->operations.execute(context);
	}
}

void simple_trigger_manager::execute(int context)
{
	for (int i = 0; i < this->num_simple_triggers; ++i)
	{
		this->simple_triggers[i].execute(context);
		
		if (context > 0 && context & 0x1 && warband->cur_game->window_redirects.size() && warband->cur_game->window_redirects.back().redirect_no == 18)
			break;
	}
}

bool simple_trigger_manager::has_trigger(int trigger_no) const
{
	for (int i = 0; i < this->num_simple_triggers; ++i)
	{
		if (round_half_up(this->simple_triggers[i].interval) == trigger_no)
			return true;
	}

	return false;
}

int simple_trigger_manager::addTrigger(const simple_trigger &newTrigger)
{
	return array_add_elem(this->simple_triggers, this->num_simple_triggers, newTrigger);
}

bool simple_trigger_manager::removeTrigger(int index)
{
	return array_remove_elem(this->simple_triggers, this->num_simple_triggers, index);
}