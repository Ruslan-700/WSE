#include "WSEOperationContext.h"

#include <ShlObj.h>
#include "WSE.h"

WSEOperationContext::WSEOperationContext(const std::string &name, int opcode_range, int opcode_range_max)
{
	m_name = name;
	m_opcode_range = opcode_range;
	m_opcode_range_max = opcode_range_max;
	m_opcode_range_cur = m_opcode_range;
}

void WSEOperationContext::OnUnload()
{
	WSE->Scripting.UnregisterOperations(this);
	m_opcode_range_cur = m_opcode_range;
}

void WSEOperationContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case LoadOperations:
		Load();
		break;
	}
}

void WSEOperationContext::Prepare(wb::operation *operation, WSEOperationDescriptor *descriptor, int context_flags, int *operand_types, __int64 *operand_values, __int64 *locals)
{
	m_operation = operation;
	m_descriptor = descriptor;
	m_context_flags = context_flags;
	m_operand_count = m_operation->num_operands;
	m_operand_types = operand_types;
	m_operand_values = operand_values;
	m_locals = locals;
	m_cur_operand = descriptor->m_flags & Lhs ? 1 : 0;
}

bool WSEOperationContext::HasMoreOperands()
{
	return m_cur_operand < m_operand_count;
}

void WSEOperationContext::SetReturnValue(int value)
{
	m_operation->set_return_value(m_locals, value);
}

void WSEOperationContext::ExtractLhsValue(int &value)
{
	if (m_descriptor->m_max_operands == 0)
		ScriptError("operand index (1) is above maximum (%d)", m_descriptor->m_min_operands);

	value = (int)m_operand_values[0];
}

void WSEOperationContext::ExtractPointer(void *&value, void *def)
{
	value = (void *)GetNextOperand((int)def);
}

void WSEOperationContext::ExtractBigValue(__int64 &value, __int64 def)
{
	value = GetNextOperandRaw(def);
}

void WSEOperationContext::ExtractValue(int &value, int def)
{
	value = GetNextOperand(def);
}

void WSEOperationContext::ExtractBoundedValue(int &value, int lower_bound, int upper_bound, int def)
{
	value = GetNextOperand(def);

	if (value < lower_bound)
		ScriptError("%s is below minimum (%d)", m_descriptor->m_operands[m_cur_operand - 1].c_str(), lower_bound);
	else if (value >= upper_bound)
		ScriptError("%s is above maximum (%d)", m_descriptor->m_operands[m_cur_operand - 1].c_str(), upper_bound - 1);
}

void WSEOperationContext::ExtractClampedValue(int &value, int lower_bound, int upper_bound, int def)
{
	value = GetNextOperand(def);

	if (value < lower_bound)
		value = lower_bound;
	else if (value >= upper_bound)
		value = upper_bound - 1;
}

void WSEOperationContext::ExtractBoolean(bool &value, bool def)
{
	value = GetNextOperand(def) != 0;
}

void WSEOperationContext::ExtractFixedPoint(float &value, float def)
{
	if (!HasMoreOperands())
		value = def;

	value = GetNextOperand() / (float)warband->basic_game.fixed_point_multiplier;
}

void WSEOperationContext::ExtractString(rgl::string &value, const char *def)
{
	if (!HasMoreOperands())
	{
		value = def;
		return;
	}

	int type = GetOperandType();
	int index = GetNextOperand();

	switch (type)
	{
	case 0:
		if (index < 0 || index >= 128)
			ScriptError("invalid string register index %d", index);

		break;
	case 3:
		if (index < 0 || index >= warband->string_manager.num_strings)
			ScriptError("invalid string index %d", index);

		break;
	case 22:
		if (index < 0 || index >= warband->string_manager.num_quick_strings)
			ScriptError("invalid quick string index %d", index);

		break;
	default:
		ScriptError("operand type %d is not a string", type);
	}

	rgl::string temp;

	warband->string_manager.get_operand_string(temp, index, type);
	warband->basic_game.parse_string(value, temp);
}

void WSEOperationContext::ExtractString(std::string &value, const std::string &def)
{
	rgl::string s;

	ExtractString(s, def.c_str());
	value = s.c_str();
}

void WSEOperationContext::ExtractWideString(CStringW &value, const char *def)
{
	rgl::string s;

	ExtractString(s, def);
	s.widen(value);
}

void WSEOperationContext::ExtractPath(std::string &path)
{
	ExtractString(path);
	
	char invalid[] = "<>:\"/\\|?*";
	size_t index = path.find_first_of(invalid);

	if (index != std::string::npos)
		ScriptError("invalid character in file path (%s)", invalid[index]);

	trim(path);
	ltrim(path, ".");
	trim(path);

	if (path.empty())
		ScriptError("invalid empty file path");
}

void WSEOperationContext::ExtractRegister(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= 128)
		ScriptError("invalid register %d", value);
}

void WSEOperationContext::ExtractScriptNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->script_manager.num_scripts)
		ScriptError("invalid script no %d", value);
}

void WSEOperationContext::ExtractMenuNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->num_menus)
		ScriptError("invalid menu no %d", value);
}

void WSEOperationContext::ExtractItemKindNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->num_item_kinds)
		ScriptError("invalid item kind no %d", value);
}

void WSEOperationContext::ExtractScenePropNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->num_scene_props)
		ScriptError("invalid scene prop no %d", value);
}

void WSEOperationContext::ExtractItemModifierNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= NUM_ITEM_MODIFIERS)
		ScriptError("invalid item modifier no %d", value);
}

void WSEOperationContext::ExtractPartyTemplateNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->num_party_templates)
		ScriptError("invalid party template no %d", value);
}

void WSEOperationContext::ExtractParticleSystemNo(int &value)
{
#if defined WARBAND
	value = GetNextOperand();

	if (value < 0 || value >= warband->particle_system_manager.num_particle_systems)
		ScriptError("invalid particle system no %d", value);
#endif
}

void WSEOperationContext::ExtractTroopNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_game || value < 0 || value >= warband->cur_game->num_troops)
		ScriptError("invalid troop no %d", value);
}

void WSEOperationContext::ExtractFactionNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_game || value < 0 || value >= warband->cur_game->num_factions)
		ScriptError("invalid faction no %d", value);
}

void WSEOperationContext::ExtractSkinNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->face_generator.num_skins)
		ScriptError("invalid skin no %d", value);
}

void WSEOperationContext::ExtractSkillNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= NUM_SKILLS)
		ScriptError("invalid skill no %d", value);
}

void WSEOperationContext::ExtractQuestNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_game || value < 0 || value >= warband->cur_game->num_quests)
		ScriptError("invalid quest no %d", value);
}

void WSEOperationContext::ExtractSiteNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_game || value < 0 || value >= warband->cur_game->num_sites)
		ScriptError("invalid scene no %d", value);
}

void WSEOperationContext::ExtractPresentationNo(int &value)
{
#if defined WARBAND
	value = GetNextOperand();

	if (value < 0 || value >= warband->presentation_manager.num_presentations)
		ScriptError("invalid presentation no %d", value);
#endif
}

void WSEOperationContext::ExtractSoundNo(int &value)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->sound_manager.num_sounds)
	{
		if (!warband->config.disable_sound)
			ScriptError("invalid sound no %d", value);
		else
			throw 0;
	}
}

void WSEOperationContext::ExtractPartyNo(int &value)
{
	value = GetNextOperand();
	
	if (!warband->cur_game || !warband->cur_game->parties.is_valid_index(value))
		ScriptError("invalid party no %d", value);
}

void WSEOperationContext::ExtractPartyStackNo(int &value, int party_id)
{
	value = GetNextOperand();

	if (value < 0 || value >= warband->cur_game->parties[party_id].num_stacks)
		ScriptError("invalid party stack no %d", value);
}

void WSEOperationContext::ExtractPlayerNo(int &value)
{
	value = GetNextOperand();

	if (!warband->basic_game.game_type || value < 0 || value >= NUM_NETWORK_PLAYERS || warband->multiplayer_data.players[value].status != wb::nps_active)
		ScriptError("invalid player no %d", value);
}

void WSEOperationContext::ExtractProfileNo(int &value)
{
	value = GetNextOperand();

	if (!warband->basic_game.game_type || value < 0 || value >= warband->multiplayer_data.profile_manager.profiles.size())
		ScriptError("invalid profile no %d", value);
}

void WSEOperationContext::ExtractTeamNo(int &value)
{
	value = GetNextOperand();

	if (!warband->basic_game.game_type || value < 0 || value >= 4)
		ScriptError("invalid team no %d", value);
}

void WSEOperationContext::ExtractAgentNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_mission || !warband->cur_mission->agents.is_valid_index(value))
		ScriptError("invalid agent no %d", value);
}

void WSEOperationContext::ExtractMissionObjectNo(int &value)
{
	value = GetNextOperand();

	if (!warband->cur_mission || !warband->cur_mission->mission_objects.is_valid_index(value))
		ScriptError("invalid scene prop instance no %d", value);
}

void WSEOperationContext::ExtractOverlayNo(int &value)
{
#if defined WARBAND
	value = GetNextOperand();

	if (!warband->cur_presentation || value < 0 || value >= warband->cur_presentation->overlays.size())
		ScriptError("invalid overlay no %d", value);
#endif
}

int WSEOperationContext::GetCurrentTrigger() const
{
	if (m_context_flags < 0)
		return m_context_flags;
	else
		return 0;
}

void WSEOperationContext::ScriptError(std::string format, ...) const
{
	char buffer[1024];
	va_list ap;

	format = "WSE ERROR on operation " + m_descriptor->m_name + ": " + format;

	va_start(ap, format);
	vsprintf_s(buffer, 1024, format.c_str(), ap);
	va_end(ap);
	
	wb::script::error(buffer);
	throw 0;
}

void WSEOperationContext::WindowsAPIError(std::string format, ...) const
{
	int error = GetLastError();
	char buffer[1024];
	va_list ap;

	va_start(ap, format);
	vsprintf_s(buffer, 1024, format.c_str(), ap);
	va_end(ap);

	size_t cur_len = strlen(buffer);

	if (error != 0)
	{
		LPTSTR error_msg;

		if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, (LPTSTR)&error_msg, 0, nullptr))
			throw 0;
		
		sprintf_s(buffer + cur_len, 1024 - cur_len, " - error %d: %s", error, error_msg);
		LocalFree(error_msg);
	}

	wb::script::error(buffer);
	throw 0;
}

std::string WSEOperationContext::CreateFile(const std::string &file, const std::string &extension)
{
	std::string path = WSE->SettingsIni.String("wse", "storage_path");

	path.reserve(MAX_PATH);

	if (path == "")
	{
#if defined WARBAND
		char buffer[MAX_PATH];

		if (SHGetFolderPath(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, buffer) != S_OK)
			WindowsAPIError("SHGetFolderPath failed for CSIDL_MYDOCUMENTS");

		path = std::string(buffer) + "\\" + warband->basic_game.name.c_str() + "\\WSE";
#elif defined WARBAND_DEDICATED
		path = "WSE";
#endif
	}

	if (path[path.length() - 1] != '\\')
		path.append("\\");
	
	if (!CreateDirectory(path.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
		WindowsAPIError("CreateDirectory failed for path %s", path.c_str());

	path += warband->cur_module_name.c_str();

	if (!CreateDirectory(path.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
		WindowsAPIError("CreateDirectory failed for path %s", path.c_str());

	path += "\\" + file + "." + extension;

	return path;
}

void WSEOperationContext::DefineOperation(int opcode, const std::string &name, unsigned int flags, short min_operands, short max_operands, std::string description, ...)
{
	std::string operands[16];
	va_list ap;

	va_start(ap, description);

	for (int i = 0; i < max_operands; ++i)
	{
		operands[i] = va_arg(ap, char *);
	}

	va_end(ap);

	WSE->Scripting.AddOperation(this, nullptr, Both, flags, min_operands, max_operands, opcode, name, description, operands);
}

void WSEOperationContext::DisableOperation(int opcode, const std::string &name)
{
	std::string operands[16];

	WSE->Scripting.AddOperation(this, nullptr, Both, Disable, 0, 0, opcode, name, "Disabled by WSE", operands);
}

void WSEOperationContext::ReplaceOperation(int opcode, const std::string &name, void *callback, WSEOperationTarget target, unsigned int flags, short min_operands, short max_operands, std::string description, ...)
{
	std::string operands[16];
	va_list ap;

	va_start(ap, description);

	for (int i = 0; i < max_operands; ++i)
	{
		operands[i] = va_arg(ap, char *);
	}

	va_end(ap);

	WSE->Scripting.AddOperation(this, callback, target, flags, min_operands, max_operands, opcode, name, description, operands);
}

void WSEOperationContext::RegisterOperation(const std::string &name, void *callback, WSEOperationTarget target, unsigned int flags, short min_operands, short max_operands, std::string description, ...)
{
	if (m_opcode_range_cur > m_opcode_range_max)
		WSE->Log.Error("%s operation %s (%d) out of opcode range (%d-%d)", m_name.c_str(), name.c_str(), m_opcode_range_cur, m_opcode_range, m_opcode_range_max);
	
	std::string operands[16];
	va_list ap;

	va_start(ap, description);

	for (int i = 0; i < max_operands; ++i)
	{
		operands[i] = va_arg(ap, char *);
	}

	va_end(ap);

	WSE->Scripting.AddOperation(this, callback, target, flags, min_operands, max_operands, m_opcode_range_cur++, name, description, operands);
}

int WSEOperationContext::GetOperandType(int def)
{
	if (m_cur_operand >= m_descriptor->m_max_operands)
	{
		ScriptError("operand index (%d) is above maximum (%d)", m_cur_operand + 1, m_descriptor->m_max_operands);
	}
	else if (m_cur_operand >= m_operand_count)
	{
		if (m_cur_operand < m_descriptor->m_min_operands)
			ScriptError("operand index (%d) is below minimum (%d)", m_cur_operand + 1, m_descriptor->m_min_operands);
		else
			return def;
	}

	return m_operand_types[m_cur_operand];
}

int WSEOperationContext::GetNextOperand(int def)
{
	return (int)GetNextOperandRaw(def);
}

unsigned __int64 WSEOperationContext::GetNextOperandRaw(unsigned __int64 def)
{
	if (m_cur_operand >= m_descriptor->m_max_operands)
	{
		ScriptError("operand index (%d) is above maximum (%d)", m_cur_operand + 1, m_descriptor->m_max_operands);
	}
	else if (m_cur_operand >= m_operand_count)
	{
		if (m_cur_operand < m_descriptor->m_min_operands)
			ScriptError("operand index (%d) is below minimum (%d)", m_cur_operand + 1, m_descriptor->m_min_operands);
		else
			return def;
	}

	return m_operand_values[m_cur_operand++];
}
