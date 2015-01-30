#pragma once

#include "rgl.h"

#define NUM_ITEM_MODIFIERS 43

namespace wb
{
	enum item_modifier_types
	{
		imod_plain       = 0,
		imod_cracked     = 1,
		imod_rusty       = 2,
		imod_bent        = 3,
		imod_chipped     = 4,
		imod_battered    = 5,
		imod_poor        = 6,
		imod_crude       = 7,
		imod_old         = 8,
		imod_cheap       = 9,
		imod_fine        = 10,
		imod_well_made   = 11,
		imod_sharp       = 12,
		imod_balanced    = 13,
		imod_tempered    = 14,
		imod_deadly      = 15,
		imod_exquisite   = 16,
		imod_masterwork  = 17,
		imod_heavy       = 18,
		imod_strong      = 19,
		imod_powerful    = 20,
		imod_tattered    = 21,
		imod_ragged      = 22,
		imod_rough       = 23,
		imod_sturdy      = 24,
		imod_thick       = 25,
		imod_hardened    = 26,
		imod_reinforced  = 27,
		imod_superb      = 28,
		imod_lordly      = 29,
		imod_lame        = 30,
		imod_swaybacked  = 31,
		imod_stubborn    = 32,
		imod_timid       = 33,
		imod_meek        = 34,
		imod_spirited    = 35,
		imod_champion    = 36,
		imod_fresh       = 37,
		imod_day_old     = 38,
		imod_two_day_old = 39,
		imod_smelling    = 40,
		imod_rotten      = 41,
		imod_large_bag   = 42,
	};

	struct item_modifier
	{
		rgl::string text;
		rgl::string id;
		float price_factor;
		float rarity;
	};
}
