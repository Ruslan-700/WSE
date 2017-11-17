#include "operation.h"

#include "warband.h"

using namespace wb;

operation::operation()
{
	end_statement = -1;
}

__int64 operation::get_operand_value(__int64 *locals, const int &operand_no)
{
	int operand_type;

	return this->get_operand_value(locals, operand_no, operand_type);
}

__int64 operation::get_operand_value(__int64 *locals, const int &operand_no, int &operand_type)
{
	__int64 value = this->operands[operand_no];

	if (value <= 0)
	{
		operand_type = 0;
	}
	else
	{
		operand_type = (value >> 56) & 0x7F;
		value &= 0xFFFFFFFFFFFFFF;
	}

	switch (operand_type)
	{
	case 0:
		return value;
	case 1:
		return warband->basic_game.registers[(int)value];
	case 2:
		return warband->basic_game.global_variables.get((int)value);
	case 17:
		return locals[(int)value];
	default:
		return value;
	}
}

void operation::set_return_value(__int64 *locals, const __int64 &value)
{
	switch ((this->operands[0] >> 56) & 0x7F)
	{
	case 1:
		warband->basic_game.registers[(int)this->operands[0]] = value;
		break;
	case 2:
		warband->basic_game.global_variables.set((int)this->operands[0], value);
		break;
	case 17:
		locals[(int)this->operands[0]] = value;
		break;
	default:
		{
			char buf[512];

			sprintf_s(buf, "Illegal lvalue in statement; Opcode = %d", warband->cur_opcode);
			script::error(buf);
		}

		break;
	}
}

bool operation::execute(__int64 *locals, const int &context_flags, int &error)
{
	THISCALL3(addresses::operation_Execute, this, locals, context_flags, error);
}

bool operation::is_valid_register(int register_no)
{
	if (register_no >= 0 && register_no < 128)
		return true;

	char buf[512];
	
	sprintf_s(buf, "SCRIPT ERROR ON OPCODE %d: Invalid Register ID: %d", warband->cur_opcode, register_no);
	script::error(buf);
	return false;
}

bool operation::is_valid_script_parameter(int parameter_no, int num_parameters)
{
	if (parameter_no >= 0 && parameter_no < num_parameters)
		return true;

	char buf[512];
	
	sprintf_s(buf, "SCRIPT ERROR ON OPCODE %d: Invalid Script Parameter ID: %d", warband->cur_opcode, parameter_no);
	script::error(buf);
	return false;
}

bool is_valid_string(int type, int string_no)
{
	char buf[512];

	switch (type)
	{
	case 0:
		if (string_no >= 0 && string_no < NUM_REGISTERS)
			return true;

		sprintf_s(buf, "SCRIPT WARNING ON OPCODE %d: Invalid String Register ID: %d", warband->cur_opcode, string_no);
		break;
	case 3:
		if (string_no >= 0 && string_no < warband->string_manager.num_strings)
			return true;

		sprintf_s(buf, "SCRIPT WARNING ON OPCODE %d: Invalid String ID: %d", warband->cur_opcode, string_no);
		break;
	case 22:
		if (string_no >= 0 && string_no < warband->string_manager.num_quick_strings)
			return true;

		sprintf_s(buf, "SCRIPT WARNING ON OPCODE %d: Invalid Quick String ID: %d", warband->cur_opcode, string_no);
		break;
	}

	script::error(buf);
	return false;
}

bool operation::is_valid_party(int party_no)
{
	if (warband->cur_game->parties.is_valid_index(party_no))
		return true;

	char buf[512];
	
	sprintf_s(buf, "SCRIPT ERROR ON OPCODE %d: Invalid Party ID: %d", warband->cur_opcode, party_no);
	script::error(buf);
	return false;
}

bool operation::is_valid_scene_prop_no(int scene_prop_no)
{
	if (scene_prop_no >= 0 && scene_prop_no < warband->num_scene_props)
		return true;

	char buf[512];
	
	sprintf_s(buf, "SCRIPT WARNING ON OPCODE %d: Invalid Scene Prop ID: %d", warband->cur_opcode, scene_prop_no);
	script::error(buf);
	return false;
}

operation_manager::operation_manager()
{
	operations = nullptr;
	num_operations = 0;
	analyzed = false;
}

operation_manager::operation_manager(const operation_manager &obj)
{
	operations = nullptr;
	*this = obj;
}

operation_manager::~operation_manager()
{
	rgl::_delete(operations);
}

operation_manager &operation_manager::operator =(const operation_manager &obj)
{
	rgl::_delete(operations);
	num_operations = obj.num_operations;
	analyzed = obj.analyzed;
	id = obj.id;

	if (num_operations)
	{
		operations = rgl::_new<operation>(num_operations);

		for (int i = 0; i < num_operations; ++i)
		{
			operations[i] = obj.operations[i];
		}
	}
	else
	{
		operations = nullptr;
	}

	return *this;
}

void operation_manager::analyze()
{
	if (this->analyzed)
		return;

	int else_statements[256];
	int end_statements[256];
	int depth = 0;

	for (int i = 0; i < this->num_operations; ++i)
	{
		int opcode = this->operations[i].opcode & 0xFFFFFFF;

		switch (opcode)
		{
		case try_begin:
		case try_for_range:
		case try_for_range_backwards:
		case try_for_parties:
		case try_for_agents:
		case try_for_players:
		case try_for_prop_instances:
		case try_for_dict_keys:
			else_statements[depth + 1] = i;
			end_statements[depth + 1] = i;
			depth++;

			if (depth >= 256)
			{
				char buf[512];

				sprintf_s(buf, "Amount of nested loops in %s is greater than %d", this->id.c_str(), 256);
				wb::script::error(buf);
			}

			break;
		case else_try:
			if (depth <= 0)
			{
				char buf[512];

				sprintf_s(buf, "Unmatched else_try in %s, line %d", this->id.c_str(), i);
				wb::script::error(buf);
			}
			else
			{
				for (int j = else_statements[depth] + 1; i - j > 0; ++j)
				{
					if (this->operations[j].end_statement < 0)
						this->operations[j].end_statement = i;
				}

				else_statements[depth] = i;
			}

			break;
		case try_end:
			if (depth <= 0)
			{
				char buf[512];

				sprintf_s(buf, "Unmatched try_end in %s, line %d", this->id.c_str(), i);
				wb::script::error(buf);
			}
			else
			{
				for (int j = end_statements[depth]; i - j > 0; ++j)
				{
					if (this->operations[j].end_statement < 0)
						this->operations[j].end_statement = i;
				}

				--depth;
			}

			break;
		}
	}

	for (int i = 0; i < this->num_operations; ++i)
	{
		if (this->operations[i].end_statement < 0)
			this->operations[i].end_statement = this->num_operations;
	}

	this->analyzed = true;
}

bool operation_manager::execute(int context)
{
	__int64 params[16];

	return this->execute(context, 0, 0, params);
}

bool operation_manager::execute(int context, int depth, const int &num_params, __int64 *params, int debug_mode)
{
	THISCALL5(addresses::operation_manager_Execute, this, context, depth, num_params, params, debug_mode);
}
