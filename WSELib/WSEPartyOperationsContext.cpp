#include "WSEPartyOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int PartyStackGetExperience(WSEPartyOperationsContext *context)
{
	int party_no;
	int party_stack_no;
	
	context->ExtractPartyNo(party_no);
	context->ExtractPartyStackNo(party_stack_no, party_no);
	
	return rglRound(warband->cur_game->parties[party_no].stacks[party_stack_no].experience);
}

int PartyStackGetNumUpgradeable(WSEPartyOperationsContext *context)
{
	int party_no;
	int party_stack_no;
	
	context->ExtractPartyNo(party_no);
	context->ExtractPartyStackNo(party_stack_no, party_no);
	
	return warband->cur_game->parties[party_no].stacks[party_stack_no].num_upgradeable;
}

bool PartyHasFlag(WSEItemOperationsContext *context)
{
	int party_no, flag;
	
	context->ExtractPartyNo(party_no);
	context->ExtractValue(flag);
	
	return (warband->cur_game->parties[party_no].flags & flag) > 0;
}

void PartyHealMembers(WSEPartyOperationsContext *context)
{
	int party_no;
	int troop_no;
	int num_to_heal;
	
	context->ExtractPartyNo(party_no);
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(num_to_heal);

	wb::party *party = &warband->cur_game->parties[party_no];

	for (int i = 0; i < party->num_stacks && num_to_heal > 0; ++i)
	{
		if (party->stacks[i].troop_no == troop_no && party->stacks[i].num_wounded > 0)
		{
			if (party->stacks[i].num_wounded > num_to_heal)
			{
				party->stacks[i].num_wounded -= num_to_heal;
				return;
			}
			else
			{
				num_to_heal -= party->stacks[i].num_wounded;
				party->stacks[i].num_wounded = 0;
			}
		}
	}
}

void PartySwitchStacks(WSEPartyOperationsContext *context)
{
	int party_no, party_stack_no_1, party_stack_no_2;

	context->ExtractPartyNo(party_no);
	context->ExtractPartyStackNo(party_stack_no_1, party_no);
	context->ExtractPartyStackNo(party_stack_no_2, party_no);

	wb::party *party = &warband->cur_game->parties[party_no];

	std::swap(party->stacks[party_stack_no_1], party->stacks[party_stack_no_2]);

	if (party_no == warband->cur_game->main_party_no && party_stack_no_1 < MB_NUM_DNA_PARTY_STACKS && party_stack_no_2 < MB_NUM_DNA_PARTY_STACKS)
		std::swap(warband->cur_game->main_party_dnas->dnas[party_stack_no_1], warband->cur_game->main_party_dnas->dnas[party_stack_no_2]);
}

void PartyStackSetNumUpgradeable(WSEPartyOperationsContext *context)
{
	int party_no;
	int party_stack_no;
	int value;

	context->ExtractPartyNo(party_no);
	context->ExtractPartyStackNo(party_stack_no, party_no);
	context->ExtractValue(value);

	warband->cur_game->parties[party_no].stacks[party_stack_no].num_upgradeable = value;
}

WSEPartyOperationsContext::WSEPartyOperationsContext() : WSEOperationContext("party", 3900, 3999)
{
}

void WSEPartyOperationsContext::OnLoad()
{
	RegisterOperation("party_stack_get_experience", PartyStackGetExperience, Both, Lhs, 3, 3,
		"Stores the experience of <1>'s <2> into <0>",
		"destination", "party_no", "party_stack_no");

	RegisterOperation("party_stack_get_num_upgradeable", PartyStackGetNumUpgradeable, Both, Lhs, 3, 3,
		"Stores the amount of upgradeable troops in <1>'s <2> into <0>",
		"destination", "party_no", "party_stack_no");
	
	RegisterOperation("party_has_flag", PartyHasFlag, Both, Cf, 2, 2,
		"Fails if <0> doesn't have <1>",
		"party_no", "flag");

	RegisterOperation("party_heal_members", PartyHealMembers, Both, None, 3, 3,
		"Heals <2> <1> of <0>",
		"party_no", "troop_no", "number");

	RegisterOperation("party_switch_stacks", PartySwitchStacks, Both, None, 3, 3,
		"Switches <0>'s <1> and <2>",
		"party_no", "party_stack_no_1", "party_stack_no_2");

	RegisterOperation("party_stack_upgrade", nullptr, Both, WSE2, 4, 4,
		"Upgrades <0>'s <1>'s <2> of troops (<3> can be 0 or 1)",
		"party_no", "party_stack_no", "amount", "upgrade_path");

	RegisterOperation("party_stack_set_num_upgradeable", PartyStackSetNumUpgradeable, Both, None, 3, 3,
		"Sets 0>'s <1>'s amount of upgradeable troops to <2>",
		"party_no", "party_stack_no", "value");
}
