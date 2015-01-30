#pragma once

#include "rgl.h"
#include "operation.h"

namespace wb
{
	struct sentence
	{
		rgl::string text;
		rgl::string id;
		int speaker;
		int input_token_no;
		operation_manager conditions;
		operation_manager consequences;
		rgl::string voice_over_path;
		int output_token_no;
	};

	struct conversation_option
	{
		int sentence_no;
		int repeat_object;
		unsigned int flags;
		rgl::string text;
	};

	struct conversation_manager
	{
		int num_sentences;
		sentence *sentences;
		int active_token_no;
		int conversation_partner_troop_no;
		int conversation_partner_party_no;
		int conversation_partner_agent_no;
		int conversation_speaker_agent_no;
		int cur_sentence_no;
		int repeat_object;
		int num_options;
		conversation_option speaker_options[1024];
		conversation_option partner_options[1024];
		bool in_meta_mission;
		rgl::string speaker_text;
	};
}
