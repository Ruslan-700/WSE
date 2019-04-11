#pragma once

#define NUM_TIMERS 5

namespace rgl
{
	struct timer
	{
		int timer_no;
		int u1;
		__int64 value;
		
		timer(int timer_no = 0);
		double get_elapsed_time() const;
		void update(int offset = 0);
		void increase(float offset);
	};

	struct toggle_timer : timer
	{
		bool toggle;
	};

	struct mission_timer : timer
	{
		mission_timer();
	};
}
