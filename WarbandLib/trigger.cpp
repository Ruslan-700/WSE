#include "trigger.h"

#include "wb.h"

using namespace wb;

void trigger::execute(int context)
{
	if (this->status == ts_rearm && this->rearm_interval <= this->rearm_interval_timer.get_elapsed_time())
	{
		this->check_interval_timer.update();
		this->status = ts_ready;
	}

	if (this->status == ts_ready)
	{
		if (this->check_interval > -0.1)
		{
			if (context < 0 || this->check_interval > this->check_interval_timer.get_elapsed_time())
				return;

			this->check_interval_timer.update();
			
			if (this->conditions.execute(context))
			{
				this->delay_interval_timer.update();
				this->status = ts_delay;
			}
		}
		else
		{
			if (round_half_up(this->check_interval) == context && this->conditions.execute(context))
			{
				this->rearm_interval_timer.update();
				this->status = ts_rearm;
				this->consequences.execute(context);
			}
		}
	}

	if (this->status == ts_delay && this->delay_interval <= this->delay_interval_timer.get_elapsed_time())
	{
		this->rearm_interval_timer.update();
		this->status = ts_rearm;
		this->consequences.execute(context);
	}
}

void trigger_manager::execute(int context)
{
	for (int i = 0; i < num_triggers; ++i)
	{
		this->triggers[i].execute(context);
	}
}

int trigger_manager::addTrigger(const trigger &newTrigger)
{
	size_t oldTriggersSize = this->num_triggers * sizeof(trigger);
	size_t newTriggersSize = oldTriggersSize + sizeof(trigger);

	void *oldTriggers = (void*)this->triggers;
	void *newTriggers = malloc(newTriggersSize);

	memcpy_s(newTriggers, newTriggersSize, oldTriggers, oldTriggersSize);
	free(oldTriggers);
	this->triggers = (trigger*)newTriggers;

	this->triggers[this->num_triggers].conditions.id.initialize();
	this->triggers[this->num_triggers].consequences.id.initialize();
	this->triggers[this->num_triggers] = newTrigger;

	return this->num_triggers++;
}