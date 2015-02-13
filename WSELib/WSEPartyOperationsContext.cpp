#include "WSEPartyOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int PartyStackGetExperience(WSEPartyOperationsContext *context)
{
	int party_no;
	int party_stack_no;
	
	context->ExtractPartyNo(party_no);
	context->ExtractPartyStackNo(party_stack_no, party_no);
	
	return (int)warband->cur_game->parties[party_no].stacks[party_stack_no].experience;
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

WSEPartyOperationsContext::WSEPartyOperationsContext() : WSEOperationContext("party", 3900, 3999)
{
}

void WSEPartyOperationsContext::OnLoad()
{
	RegisterOperation("party_stack_get_experience", PartyStackGetExperience, Client, Lhs, 3, 3,
		"Stores the experience of <1>'s <2> into <0>",
		"destination", "party_no", "party_stack_no");

	RegisterOperation("party_stack_get_num_upgradeable", PartyStackGetNumUpgradeable, Client, Lhs, 3, 3,
		"Stores the amount of upgradeable troops in <1>'s <2> into <0>",
		"destination", "party_no", "party_stack_no");
	
	RegisterOperation("party_has_flag", PartyHasFlag, Both, Cf, 2, 2,
		"Fails if <0> doesn't have <1>",
		"party_no", "flag");

	RegisterOperation("party_heal_members", PartyHealMembers, Both, None, 3, 3,
		"Heals <2> <1> of <0>",
		"party_no", "troop_no", "number");
}
