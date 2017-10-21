#include "simple_trigger.h"

#include "warband.h"

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
	size_t oldTriggersSize = this->num_simple_triggers * sizeof(simple_trigger);
	size_t newTriggersSize = oldTriggersSize + sizeof(simple_trigger);

	void *oldTriggers = (void*)this->simple_triggers;
	void *newTriggers = malloc(newTriggersSize);

	memcpy_s(newTriggers, newTriggersSize, oldTriggers, oldTriggersSize);
	free(oldTriggers);
	this->simple_triggers = (simple_trigger*)newTriggers;

	this->simple_triggers[this->num_simple_triggers].operations.operations = rgl::_new<operation>();
	this->simple_triggers[this->num_simple_triggers].operations.id.initialize();
	this->simple_triggers[this->num_simple_triggers] = newTrigger;

	return this->num_simple_triggers++;
}
