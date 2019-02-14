#pragma once

#include "rgl.h"

#define TAG_SHIFT 24

namespace wb
{
	enum opcodes
	{
		go_to = 0,
		call_script = 1,
		try_end = 3,
		try_begin = 4,
		else_try = 5,
		try_for_range = 6,
		try_for_range_backwards = 7,
		break_loop = 8,
		continue_loop = 9,
		try_for_parties = 11,
		try_for_agents = 12,
		try_for_attached_parties = 15,
		try_for_prop_instances = 16,
		try_for_players = 17,
		try_for_dict_keys = 18,
		store_script_param_1 = 21,
		store_script_param_2 = 22,
		store_script_param = 23,
		ge = 30,
		player_set_is_admin = 429,
		kick_player = 465,
		ban_player = 466,
		change_screen_quit = 2055,
	};

	/*struct operand
	{
		int value;
		operand_tag tag;
	};*/

	struct operation
	{
		int opcode;
		int u1;
		int num_operands;
		__int64 operands[16];
		int end_statement;

		operation();
		__int64 get_operand_value(__int64 *locals, const int &operand_no);
		__int64 get_operand_value(__int64 *locals, const int &operand_no, int &operand_type);
		void set_return_value(__int64 *locals, const __int64 &value);
		bool execute(__int64 *locals, const int &context_flags, int &error);
		static bool is_valid_register(int register_no);
		static bool is_valid_script_parameter(int parameter_no, int num_parameters);
		static bool is_valid_string(int type, int string_no);
		static bool is_valid_party(int party_no);
		static bool is_valid_scene_prop_no(int scene_prop_no);
	};

	struct operation_manager
	{
		operation *operations;
		int num_operations;
		bool analyzed;
		rgl::string id;
		
		operation_manager();
		operation_manager(const operation_manager &obj);
		~operation_manager();
		operation_manager &operator =(const operation_manager &obj);
		void analyze();
		bool execute(int context);
		bool execute(int context, int depth, const int &num_params, __int64 *params, int debug_mode = -1);
	};
}
