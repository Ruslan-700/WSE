#pragma once

#define NUM_TIMERS 5

namespace rgl
{
	enum timer_kind
	{
		app = 0,
		game = 1,
		mission = 2
	};

	struct timer
	{
		int timer_no;
		int u1;
		__int64 value;
		
		timer(int timer_no = timer_kind::app);
		double get_elapsed_time() const;
		void update(int offset = 0);
		void increase(float offset);
	};

	struct toggle_timer : timer
	{
		bool toggle;
	};
}