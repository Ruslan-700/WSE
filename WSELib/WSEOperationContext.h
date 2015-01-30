#pragma once

#include <string>
#include "WSEContext.h"
#include "WSEIniFile.h"
#include "WSEScriptingContext.h"
#include "warband.h"

class WSEOperationContext : public WSEContext
{
public:
	WSEOperationContext(const std::string &name, int opcode_range, int opcode_range_max);

protected:
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

public:
	void Prepare(wb::operation *operation, WSEOperationDescriptor *descriptor, int context_flags, int *operand_types, __int64 *operand_values, __int64 *locals);
	bool HasMoreOperands();
	void SetReturnValue(int value);
	void ExtractLhsValue(int &value);
	void ExtractPointer(void *&value, void *def = nullptr);
	void ExtractBigValue(__int64 &value, __int64 def = 0);
	void ExtractValue(int &value, int def = 0);
	void ExtractBoundedValue(int &value, int lower_bound, int upper_bound, int def = 0);
	void ExtractClampedValue(int &value, int lower_bound, int upper_bound, int def = 0);
	void ExtractBoolean(bool &value, bool def = false);
	void ExtractFixedPoint(float &value, float def = 0.0f);
	void ExtractString(rgl::string &value, const char *def = "");
	void ExtractString(std::string &value, const std::string &def = "");
	void ExtractWideString(CStringW &value, const char *def = "");
	void ExtractPath(std::string &path);
	void ExtractRegister(int &value);
	void ExtractScriptNo(int &value);
	void ExtractMenuNo(int &value);
	void ExtractItemKindNo(int &value);
	void ExtractScenePropNo(int &value);
	void ExtractItemModifierNo(int &value);
	void ExtractPartyTemplateNo(int &value);
	void ExtractParticleSystemNo(int &value);
	void ExtractTroopNo(int &value);
	void ExtractFactionNo(int &value);
	void ExtractSkinNo(int &value);
	void ExtractSkillNo(int &value);
	void ExtractQuestNo(int &value);
	void ExtractSiteNo(int &value);
	void ExtractPresentationNo(int &value);
	void ExtractSoundNo(int &value);
	void ExtractPartyNo(int &value);
	void ExtractPartyStackNo(int &value, int party_id);
	void ExtractProfileNo(int &value);
	void ExtractPlayerNo(int &value);
	void ExtractTeamNo(int &value);
	void ExtractAgentNo(int &value);
	void ExtractMissionObjectNo(int &value);
	void ExtractOverlayNo(int &value);
	int GetCurrentTrigger() const;
	void ScriptError(std::string format, ...) const;
	void WindowsAPIError(std::string message, ...) const;
	std::string CreateFile(const std::string &file, const std::string &extension);

protected:
	void DefineOperation(int opcode, const std::string &name, unsigned int flags, short min_operands, short max_operands, std::string description, ...);
	void DisableOperation(int opcode, const std::string &name);
	void ReplaceOperation(int opcode, const std::string &name, void *callback, WSEOperationTarget target, unsigned int flags, short min_operands, short max_operands, std::string description, ...);
	void RegisterOperation(const std::string &name, void *callback, WSEOperationTarget target, unsigned int flags, short min_operands, short max_operands, std::string description, ...);

private:
	int GetOperandType(int def = 0);
	int GetNextOperand(int def = 0);
	unsigned __int64 GetNextOperandRaw(unsigned __int64 def = 0);

public:
	WSEOperationDescriptor *m_descriptor;

private:
	std::string m_name;
	int m_opcode_range;
	int m_opcode_range_max;
	int m_opcode_range_cur;
	wb::operation *m_operation;
	int m_context_flags;
	__int64 *m_locals;
	int m_operand_count;
	int *m_operand_types;
	__int64 *m_operand_values;
	int m_cur_operand;
};
