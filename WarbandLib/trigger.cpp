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
	trigger *newTriggers = rgl::_new<trigger>(this->num_triggers + 1);

	for (int i = 0; i < this->num_triggers; ++i)
	{
		newTriggers[i] = this->triggers[i];
	}
	rgl::_free(this->triggers);
	this->triggers = newTriggers;
	this->triggers[this->num_triggers] = newTrigger;

	return this->num_triggers++;
}

bool trigger_manager::removeTrigger(int index)
{
	if (index < 0)
		index += this->num_triggers;

	if (index < 0 || index >= this->num_triggers)
		return false;


	trigger *newTriggers = rgl::_new<trigger>(this->num_triggers - 1);
	for (int i = 0; i < index; ++i)
	{
		newTriggers[i] = this->triggers[i];
	}
	for (int i = index; i < this->num_triggers - 1; ++i)
	{
		newTriggers[i] = this->triggers[i + 1];
	}
	rgl::_free(this->triggers);
	this->triggers = newTriggers;
	this->num_triggers--;

	return true;


	/*size_t newTriggersSize = (this->num_triggers - 1) * sizeof(trigger);
	trigger *newTriggers = (trigger*)malloc(newTriggersSize);

	int oldIndex = 0;
	int newIndex = 0;
	while (oldIndex < this->num_triggers)
	{
		if (oldIndex != index)
		{
			void *oldIndexPtr = &(this->triggers[oldIndex]);
			void *newIndexPtr = &(newTriggers[newIndex]);
			memcpy_s(newIndexPtr, sizeof(trigger), oldIndexPtr, sizeof(trigger));

			newIndex++;
		}
		
		oldIndex++;
	}

	free(this->triggers);
	this->triggers = newTriggers;
	this->num_triggers--;

	return true;*/
}