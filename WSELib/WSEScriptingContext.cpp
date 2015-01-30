#include "WSEScriptingContext.h"

#include <fstream>
#include "WSE.h"
#include "WSEOperationContext.h"

void BreakNetworkFailOperation(WSEOperationContext *context)
{
	context->ScriptError("Operation %s requires network_compatible = 0 in module.ini", context->m_descriptor->m_name.c_str());
}

bool FailOperation(WSEOperationContext *context)
{
	return false;
}

void WSEScriptingLoopManager::AddLoop(int statement_no, int value)
{
	if (num_loops >= 0 && num_loops < MAX_NUM_NESTED_LOOPS)
	{
		loops[num_loops].statement_no = statement_no;
		loops[num_loops].value = value;
		num_loops++;
	}
}

void WSEScriptingLoopManager::Clear()
{
	num_loops = 0;
}

int WSEScriptingLoopManager::Size()
{
	return num_loops;
}

void WSEScriptingLoopManager::EndLoop()
{
	if (num_loops > 0)
		num_loops--;
}

void WSEScriptingLoopManager::EndLoop(int index)
{
	num_loops = index;
}

WSEScriptingLoop *WSEScriptingLoopManager::GetLoop()
{
	return &loops[num_loops - 1];
}

WSEScriptingLoop *WSEScriptingLoopManager::GetLoop(int index)
{
	return &loops[index];
}

WSEScriptingContext::WSEScriptingContext()
{
	m_cur_statement_block = nullptr;
	memset(m_descriptors, 0, WSE_MAX_NUM_OPERATIONS * sizeof(WSEOperationDescriptor *));
}

void WSEScriptingContext::OnLoad()
{
	WSE->Hooks.HookFunction(this, wb::addresses::operation_Execute_entry, OperationExecuteHook);
	WSE->Hooks.HookFunction(this, wb::addresses::operation_manager_Execute, OperationManagerExecuteHook);
}

void WSEScriptingContext::OnUnload()
{
	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		delete m_descriptors[i];
		m_descriptors[i] = nullptr;
	}
}

void WSEScriptingContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ModuleLoad:
		m_allow_unset_script_params = WSE->ModuleSettingsIni.Bool("", "allow_unset_script_params", false);
		WSE->SendContextEvent(this, LoadOperations);
		DumpOperationsHeader();
		break;
	}
}

bool WSEScriptingContext::ExecuteStatementBlock(wb::operation_manager *operation_manager, int context_flags, int depth, int &num_parameters, __int64 *parameters)
{
	m_cur_statement_block = operation_manager;
	WSE->Profiling.StartProfilingBlock(operation_manager);

	if (m_allow_unset_script_params && num_parameters < 16)
	{
		memset(parameters + num_parameters, 0, (16 - num_parameters) * sizeof(__int64));
		num_parameters = 16;
	}
	
	WSEScriptingLoopManager loop_manager;
	int cur_block = -1;
	__int64 local_variables[1024];
	int num_operations = operation_manager->num_operations;
	int operation_no = 0;
	bool success = true;
	bool block_success = true;

	loop_manager.Clear();
	warband->skip_next_operation = 0;
	warband->script_error_occurred = 0;
	operation_manager->analyze();

	if (warband->script_error_occurred > 0)
	{
		char buf[512];

		sprintf_s(buf, "At %s.", operation_manager->id.c_str());
#if defined WARBAND
		warband->window_manager.display_message(buf, 0xFFFF5555, 0);
#endif
		warband->log_stream.write_c_str(buf);
		warband->log_stream.write_c_str("\n");
		warband->script_error_occurred = 0;
	}

	while (operation_no < num_operations)
	{
		bool statement_success = true;
		wb::operation *operation = &operation_manager->operations[operation_no];
		warband->cur_statement_no = operation_no;
		warband->cur_opcode = operation->opcode & 0xFFFFFFF;
		
		if (warband->cur_opcode == wb::call_script || warband->cur_opcode >= wb::store_script_param_1)
		{
			if (warband->cur_opcode >= wb::ge)
			{
				int error = 0;

				statement_success = operation->execute(local_variables, context_flags, error);

				if (statement_success)
				{
					operation_no++;
				}
				else
				{
					operation_no = operation->end_statement;
					block_success = false;
				}
			}
			else if (warband->cur_opcode == wb::call_script)
			{
				int operand_type;
				int script_no = (int)operation->get_operand_value(local_variables, 0, operand_type);

				if (script_no >= 0 && script_no < warband->script_manager.num_scripts && depth < 256)
				{
					__int64 script_params[16];

					for (int i = 1; i < operation->num_operands; ++i)
					{
						script_params[i - 1] = operation->get_operand_value(local_variables, i, operand_type);
					}
					
					statement_success = warband->script_manager.scripts[script_no].operations.execute(context_flags, depth + 1, operation->num_operands - 1, script_params);

					if (statement_success)
					{
						operation_no++;
					}
					else
					{
						operation_no = operation->end_statement;
						block_success = false;
					}
				}
				else
				{
					statement_success = false;
					operation_no = operation->end_statement;
				}
			}
			else if (warband->cur_opcode == wb::store_script_param_1)
			{
				if (wb::operation::is_valid_script_parameter(0, num_parameters))
					operation->set_return_value(local_variables, parameters[0]);

				operation_no++;
			}
			else if (warband->cur_opcode == wb::store_script_param_2)
			{
				if (wb::operation::is_valid_script_parameter(1, num_parameters))
					operation->set_return_value(local_variables, parameters[1]);

				operation_no++;
			}
			else if (warband->cur_opcode == wb::store_script_param)
			{
				int operand_type;
				int parameter_no = (int)operation->get_operand_value(local_variables, 1, operand_type);

				if (wb::operation::is_valid_script_parameter(parameter_no - 1, num_parameters))
					operation->set_return_value(local_variables, parameters[parameter_no - 1]);

				operation_no++;
			}
			else
			{
				operation_no++;
			}
		}
		else if (operation->opcode == wb::break_loop)
		{
			bool found = false;

			for (int i = loop_manager.Size() - 1; i >= 0 && !found; --i)
			{
				wb::operation *loop_operation = &operation_manager->operations[loop_manager.GetLoop(i)->statement_no];
				
				if (loop_operation->opcode != wb::try_begin)
				{
					operation_no = loop_operation->end_statement;
					loop_manager.EndLoop(i);
					found = true;
				}
			}
			
			operation_no++;
		}
		else if (operation->opcode == wb::continue_loop)
		{
			bool found = false;

			for (int i = loop_manager.Size() - 1; i >= 0 && !found; --i)
			{
				wb::operation *loop_operation = &operation_manager->operations[loop_manager.GetLoop(i)->statement_no];

				if (loop_operation->opcode != wb::try_begin)
				{
					operation_no = loop_operation->end_statement;
					found = true;
				}
			}

			if (!found)
				operation_no++;
		}
		else if (operation->opcode == wb::try_end)
		{
			block_success = true;
			EndLoop(operation_manager, local_variables, loop_manager, operation_no);
		}
		else if (operation->opcode == wb::try_begin)
		{
			block_success = true;
			loop_manager.AddLoop(operation_no, 0);
			operation_no++;
		}
		else if (operation->opcode == wb::else_try)
		{
			if (block_success)
			{
				operation_no = operation->end_statement;
			}
			else
			{
				block_success = true;
				operation_no++;
			}
		}
		else
		{
			block_success = true;
			StartLoop(operation_manager, local_variables, loop_manager, operation_no);
		}

		if (!loop_manager.Size())
			success = statement_success;
		
		if (warband->script_error_occurred > 0)
		{
			char buf[512];

			sprintf_s(buf, "At %s.", operation_manager->id.c_str());
#if defined WARBAND
			warband->window_manager.display_message(buf, 0xFFFF5555, 0);
#endif
			warband->log_stream.write_c_str(buf);
			warband->log_stream.write_c_str("\n");
		}

		if (!success)
			break;
	}

	WSE->Profiling.StopProfilingBlock();
	m_cur_statement_block = nullptr;
	return success;
}

void WSEScriptingContext::StartLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager, int &statement_no)
{
	wb::operation *statement = &operation_manager->operations[statement_no];
	int start_value;
	int operand_type;
	wb::mission_grid_iterator grid_iterator;

	switch (statement->opcode)
	{
	case wb::try_for_range:
		start_value = (int)statement->get_operand_value(local_variables, 1, operand_type);
		break;
	case wb::try_for_range_backwards:
		start_value = (int)statement->get_operand_value(local_variables, 2, operand_type) - 1;
		break;
	case wb::try_for_agents:
		{
			if (statement->num_operands > 1)
			{
				int position_register_no = (int)statement->get_operand_value(local_variables, 1, operand_type);
				float radius = (float)statement->get_operand_value(local_variables, 2, operand_type) / warband->basic_game.fixed_point_multiplier;

				if (warband->cur_mission->grid.initialize_iterator(grid_iterator, warband->basic_game.position_registers[position_register_no].o, radius))
					start_value = grid_iterator.agent_obj->agent->no;
				else
					start_value = -1;
			}
			else
			{
				start_value = warband->cur_mission->agents.get_first_valid_index();
			}
		}

		break;
	case wb::try_for_parties:
		start_value = warband->cur_game->parties.get_first_valid_index();
		break;
	case wb::try_for_active_players:
		{
			start_value = (statement->num_operands > 1 && statement->get_operand_value(local_variables, 1, operand_type)) ? 1 : 0;

			for (; start_value < NUM_NETWORK_PLAYERS; ++start_value)
			{
				wb::network_player *player = &warband->multiplayer_data.players[start_value];

				if (player->is_active() && player->ready)
					break;
			}
		}

		break;
	case wb::try_for_prop_instances:
		{
			int subKindNo = -1;

			if (statement->num_operands < 2)
				subKindNo = (int)statement->get_operand_value(local_variables, 1, operand_type);

			for (int start_value = warband->cur_mission->mission_objects.get_first_valid_index(); start_value < warband->cur_mission->mission_objects.size(); start_value = warband->cur_mission->mission_objects.get_next_valid_index(start_value))
			{
				wb::mission_object *mission_object = &warband->cur_mission->mission_objects[start_value];

				if ((mission_object->meta_type == wb::mt_scene_prop || mission_object->meta_type == wb::mt_spawned_prop) && (subKindNo < 0 || mission_object->sub_kind_no == subKindNo))
					break;
			}
		}

		break;
	case wb::try_for_dict_keys:
		{
			WSEDictionary *dict = (WSEDictionary *)WSE->Objects.FetchObject(WSE->DictionaryOperations.m_type, (int)statement->get_operand_value(local_variables, 1, operand_type));
			std::map<std::string, std::string>::const_iterator it = dict->m_values.begin();
			std::map<std::string, std::string>::const_iterator end_it = dict->m_values.end();
			int start_value = it == end_it ? -1 : 1;

			loop_manager.AddLoop(statement_no, start_value);
			loop_manager.GetLoop()->dict_it = it;
			loop_manager.GetLoop()->dict_end_it = end_it;

			if (CanLoop(operation_manager, local_variables, loop_manager))
			{
				int register_no = (int)statement->get_operand_value(local_variables, 0, operand_type);

				if (wb::operation::is_valid_register(register_no))
					warband->basic_game.string_registers[register_no] = it->first;

				statement_no++;
			}
			else
			{
				statement_no = statement->end_statement;
			}
			
			return;
		}

		break;
	default:
		{
			char buf[512];

			sprintf_s(buf, "Unrecognized loop opcode %d", statement->opcode);
			wb::script::error(buf);
		}

		break;
	}

	loop_manager.AddLoop(statement_no, start_value);
	loop_manager.GetLoop()->mission_grid_iterator = grid_iterator;
	statement->set_return_value(local_variables, start_value);

	if (CanLoop(operation_manager, local_variables, loop_manager))
		statement_no++;
	else
		statement_no = statement->end_statement;
}

void WSEScriptingContext::EndLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager, int &statement_no)
{
	statement_no++;

	if (!loop_manager.Size())
		return;
	
	WSEScriptingLoop *loop = loop_manager.GetLoop();
	int value = loop->value;
	int operand_type;
	wb::operation *statement = &operation_manager->operations[loop->statement_no];
	
	switch (statement->opcode)
	{
	case wb::try_begin:
	case wb::else_try:
		loop_manager.EndLoop();
		return;
	case wb::try_for_range:
		value++;
		break;
	case wb::try_for_range_backwards:
		value--;
		break;
	case wb::try_for_agents:
		{
			if (statement->num_operands > 1)
			{
				if (warband->cur_mission->grid.advance_iterator(loop->mission_grid_iterator))
					value = loop->mission_grid_iterator.agent_obj->agent->no;
				else
					value = -1;
			}
			else
			{
				value = warband->cur_mission->agents.get_next_valid_index(value);
			}
		}
		
		break;
	case wb::try_for_parties:
		value = warband->cur_game->parties.get_next_valid_index(value);
		break;
	case wb::try_for_active_players:
		{
			++value;

			for (; value < NUM_NETWORK_PLAYERS; ++value)
			{
				wb::network_player *player = &warband->multiplayer_data.players[value];

				if (player->is_active() && player->ready)
					break;
			}
		}

		break;
	case wb::try_for_prop_instances:
		{
			int subKindNo = -1;

			if (statement->num_operands < 2)
				subKindNo = (int)statement->get_operand_value(local_variables, 1, operand_type);

			value = warband->cur_mission->mission_objects.get_next_valid_index(value);

			for (; value < warband->cur_mission->mission_objects.size(); value = warband->cur_mission->mission_objects.get_next_valid_index(value))
			{
				wb::mission_object *mission_object = &warband->cur_mission->mission_objects[value];

				if ((mission_object->meta_type == wb::mt_scene_prop || mission_object->meta_type == wb::mt_spawned_prop) && (subKindNo < 0 || mission_object->sub_kind_no == subKindNo))
					break;
			}
		}

		break;
	case wb::try_for_dict_keys:
		{
			if (loop->dict_it != loop->dict_end_it)
				loop->dict_it++;

			loop->value = loop->dict_it == loop->dict_end_it ? -1 : 1;

			if (CanLoop(operation_manager, local_variables, loop_manager))
			{
				int register_no = (int)statement->get_operand_value(local_variables, 0, operand_type);

				if (wb::operation::is_valid_register(register_no))
					warband->basic_game.string_registers[register_no] = loop->dict_it->first;

				statement_no = loop->statement_no + 1;
			}
			else
			{
				loop_manager.EndLoop();
			}

			return;
		}
		
		break;
	default:
		return;
	}

	loop->value = value;

	if (CanLoop(operation_manager, local_variables, loop_manager))
	{
		statement->set_return_value(local_variables, value);
		statement_no = loop->statement_no + 1;
	}
	else
	{
		loop_manager.EndLoop();
	}
}

bool WSEScriptingContext::CanLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager)
{
	if (!loop_manager.Size())
		return false;

	WSEScriptingLoop *loop = loop_manager.GetLoop();
	int value = loop->value;
	int operand_type;
	wb::operation *statement = &operation_manager->operations[loop->statement_no];

	switch (statement->opcode)
	{
	case wb::try_for_range:
		return value < statement->get_operand_value(local_variables, 2, operand_type);
	case wb::try_for_range_backwards:
		return value >= statement->get_operand_value(local_variables, 1, operand_type);
	case wb::try_for_agents:
		if (statement->num_operands > 1)
			return value != -1;
		else
			return value < warband->cur_mission->agents.size();
	case wb::try_for_parties:
		return value < warband->cur_game->parties.num_created;
	case wb::try_for_active_players:
		return value < NUM_NETWORK_PLAYERS;
	case wb::try_for_dict_keys:
		return value != -1;
	case wb::try_for_prop_instances:
		return value < warband->cur_mission->mission_objects.size();
	default:
		return value < statement->get_operand_value(local_variables, 1, operand_type);
	}
}

void WSEScriptingContext::AddOperation(WSEContext *context, void *callback, WSEOperationTarget target, unsigned short flags, short min_operands, short max_operands, unsigned int opcode, const std::string &name, const std::string &description, std::string *operands)
{

	if (opcode >= WSE_MAX_NUM_OPERATIONS)
	{
		WSE->Log.Error("failed to register %s: opcode %d exceeds the maximum value %d.", name.c_str(), opcode, WSE_MAX_NUM_OPERATIONS);
		return;
	}

	if (m_descriptors[opcode])
	{
		WSE->Log.Error("failed to register %s: opcode %d is already assigned to %s.", name.c_str(), opcode, m_descriptors[opcode]->m_name.c_str());
		return;
	}

	if (!(flags & Control) && opcode < WSE_FIRST_WARBAND_OPCODE)
	{
		WSE->Log.Error("failed to register %s: opcode %d is reserved.", name.c_str(), opcode);
		return;
	}

	if (flags & Lhs && max_operands == 0)
	{
		WSE->Log.Error("failed to register %s: lhs operation must have at least one operand.", name.c_str());
		return;
	}

	WSEOperationDescriptor *descriptor = new WSEOperationDescriptor();
	
	descriptor->m_context = context;
	descriptor->m_callback = callback;
	descriptor->m_type = Operation;
	descriptor->m_target = target;
	descriptor->m_flags = flags;
	descriptor->m_opcode = opcode;
	descriptor->m_min_operands = min_operands;
	descriptor->m_max_operands = max_operands;
	descriptor->m_name = name;
	descriptor->m_description = description;

	for (int i = 0; i < max_operands; ++i)
	{
		descriptor->m_operands[i] = operands[i];
	}
	
	if (flags & Cf)
		descriptor->m_type = CfOperation;
	else if (flags & Lhs)
		descriptor->m_type = LhsOperation;
	else if (flags & Disable)
		descriptor->m_type = NoOperation;

	if (flags & BreakNetwork && WSE->Network.IsNetworkCompatible())
	{
		descriptor->m_type = Operation;
		descriptor->m_callback = BreakNetworkFailOperation;
	}
#if defined WARBAND
	else if (target == Server)
#elif defined WARBAND_DEDICATED
	else if (target == Client)
#endif
	{
		if (flags & Cf && flags & Fail)
			descriptor->m_callback = FailOperation;
		else
			descriptor->m_type = NoOperation;
	}

	if (opcode >= WSE_FIRST_WARBAND_OPCODE && opcode <= WSE_LAST_WARBAND_OPCODE)
		WSE->Hooks.HookJumptable(context, wb::addresses::operation_Execute_jumptable, opcode - 30, OperationExecuteHook);
	
	m_descriptors[opcode] = descriptor;
}

void WSEScriptingContext::UnregisterOperations(WSEContext *context)
{
	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		if (m_descriptors[i] && m_descriptors[i]->m_context == context)
		{
			if (m_descriptors[i]->m_opcode >= WSE_FIRST_WARBAND_OPCODE && m_descriptors[i]->m_opcode <= WSE_LAST_WARBAND_OPCODE)
				WSE->Hooks.UnhookJumptable(context, wb::addresses::operation_Execute_jumptable, m_descriptors[i]->m_opcode - 30);

			delete m_descriptors[i];
			m_descriptors[i] = nullptr;
		}
	}
}

void WSEScriptingContext::DumpOperationsHeader()
{
	if (!WSE->SettingsIni.Bool("operations", "auto_generate_header", false))
		return;

	std::string path = WSE->GetPath() + "header_operations_addon.py";

	if (WSE->SettingsIni.HasKey("operations", "header_path"))
		path = WSE->SettingsIni.String("operations", "header_path");

	if (path.length() && path[path.length() - 1] != '\\')
		path.push_back('\\');

	std::ofstream stream(path + "header_operations_addon.py", std::ios::trunc);
	
	if (!stream)
	{
		WSE->Log.Error("failed to auto-generate header_operations_addon.py");
		return;
	}

	stream << "#Add the following definitions to the end (!) of header_operations.py" << std::endl;

	std::vector<int> max_lengths;
	int cur_block = 0;
	int max_length = 0;
	int last = 0;

	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		WSEOperationDescriptor *descriptor = m_descriptors[i];

		if (descriptor && !(descriptor->m_flags & Undocumented))
		{
			if (last / 100 != i / 100 && i > WSE_LAST_WARBAND_OPCODE)
			{
				last = i;
				max_lengths.push_back(max_length);
				max_length = 0;
			}
			
			char name[512];

			if (descriptor->m_name.length())
				sprintf_s(name, descriptor->m_name.c_str());
			else
				sprintf_s(name, "op_%d", descriptor->m_opcode);

			int length = strlen(name);

			if (length > max_length)
				max_length = length;
		}
	}

	max_lengths.push_back(max_length);
	last = 0;

	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		WSEOperationDescriptor *descriptor = m_descriptors[i];

		if (descriptor && !(descriptor->m_flags & Undocumented))
		{
			if (last / 100 != i / 100 && i > WSE_LAST_WARBAND_OPCODE)
			{
				cur_block++;
				last = i;
				stream << std::endl;
			}
			
			char name[512];

			if (descriptor->m_name.length())
				sprintf_s(name, descriptor->m_name.c_str());
			else
				sprintf_s(name, "op_%d", descriptor->m_opcode);

			int length = strlen(name);

			stream << name;

			for (int i = length; i < max_lengths[cur_block]; ++i)
				stream << " ";

			stream << " = " << descriptor->m_opcode;

			if (descriptor->m_min_operands >= 0)
			{
				stream << " #(";

				if (descriptor->m_name.length())
					stream << descriptor->m_name;
				else
					stream << descriptor->m_opcode;

				for (int i = 0; i < descriptor->m_max_operands; ++i)
				{
					stream << ", ";

					if (i >= descriptor->m_min_operands)
						stream << "[";

					stream << "<" << descriptor->m_operands[i] << ">";

					if (i >= descriptor->m_min_operands)
						stream << "]";
				}
			}

			stream << "),";

			if (descriptor->m_description.length())
			{
				std::string desc = descriptor->m_description;
				
				for (int i = 0; i < descriptor->m_max_operands; ++i)
				{
					std::string tok = "<" + itostr(i) + ">";
					int index;

					std::string replacement = "<" + descriptor->m_operands[i] + ">";

					if (i >= descriptor->m_min_operands)
						replacement = "[" + replacement + "]";
					while ((index = desc.find(tok)) >= 0)
					{
						desc.replace(index, tok.length(), replacement);
					}
				}
				
				stream << " #" << desc;
			}

			if (descriptor->m_flags & BreakNetwork)
				stream << " (requires network_compatible = 0 in wse_settings.ini)";

			stream << std::endl;
		}
	}
	
	stream << std::endl << "lhs_operations += [" << std::endl;

	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		if (m_descriptors[i] && m_descriptors[i]->m_flags & Lhs)
		{
			stream << "\t" << m_descriptors[i]->m_name.c_str() << "," << std::endl;
		}
	}
	
	stream << "]" << std::endl << std::endl << "can_fail_operations += [" << std::endl;

	for (int i = 0; i < WSE_MAX_NUM_OPERATIONS; ++i)
	{
		if (m_descriptors[i] && m_descriptors[i]->m_flags & Cf)
		{
			stream << "\t" << m_descriptors[i]->m_name.c_str() << "," << std::endl;
		}
	}
	
	stream << "]" << std::endl;
}

int WSEScriptingContext::GetTriggerParam(int index) const
{
	if (index == 1)
		return (int)warband->basic_game.trigger_param_1;
	else if (index == 2)
		return (int)warband->basic_game.trigger_param_2;
	else if (index == 3)
		return (int)warband->basic_game.trigger_param_3;
	else if (index > 0 && index <= 16)
		return m_trigger_params[index - 1];
	else
		return 0;
}

void WSEScriptingContext::SetTriggerParam(int index, int value)
{
	if (index == 1)
		warband->basic_game.trigger_param_1 = value;
	else if (index == 2)
		warband->basic_game.trigger_param_2 = value;
	else if (index == 3)
		warband->basic_game.trigger_param_3 = value;
	else if (index > 0 && index <= 16)
		m_trigger_params[index - 1] = value;
}

bool WSEScriptingContext::OnOperationExecute(wb::operation *operation, int *operand_types, __int64 *operand_values, bool *continue_loop, __int64 *locals, int context_flags)
{
	WSEOperationDescriptor *descriptor = m_descriptors[operation->opcode & 0xFFFFFFF];
		
	if (!descriptor)
		return true;

	((WSEOperationContext *)descriptor->m_context)->Prepare(operation, descriptor, context_flags, operand_types, operand_values, locals);

	try
	{
		__int64 value;

		switch (descriptor->m_type)
		{
		case Operation:
			((OperationCallback)descriptor->m_callback)(descriptor->m_context);
			break;
		case LhsOperation:
			value = ((LhsOperationCallback)descriptor->m_callback)(descriptor->m_context);
			operation->set_return_value(locals, value);
			break;
		case CfOperation:
			if (!((CfOperationCallback)descriptor->m_callback)(descriptor->m_context))
				*continue_loop = false;

			break;
		case NoOperation:
			return false;
		default:
			return true;
		}
	}
	catch (...)
	{
	}

	return false;
}
