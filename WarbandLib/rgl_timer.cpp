#include "rgl_timer.h"

#include "warband.h"

using namespace rgl;

timer::timer(int timer_no)
{
	this->timer_no = timer_no;
	this->update();
}

double timer::get_elapsed_time() const
{
	return (warband->timers[this->timer_no] - this->value) / 100000.0;
}

void timer::update(int offset)
{
	this->value = warband->timers[this->timer_no] + offset;
}

void timer::increase(float offset)
{
	this->value = warband->timers[this->timer_no] + (__int64)(offset * 100000.0f);
}


mission_timer::mission_timer() : timer(0)
{
}
