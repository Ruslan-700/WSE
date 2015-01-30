#pragma once

#include <map>
#include "WSEContext.h"

class WSEHook
{
public:
	WSEHook(WSEContext *context, void *address, int num_bytes);
	~WSEHook();
	void Restore() const;
	bool IsOwnedBy(WSEContext *context) const;

protected:
	unsigned char m_asm[8];
	void Write(const unsigned char *data) const;

private:
	WSEContext *m_context;
	void *m_address;
	int m_num_bytes;
	unsigned char m_backup[8];
};

typedef void (*WSEHookCallback)();

class WSEFunctionHook : public WSEHook
{
public:
	WSEFunctionHook(WSEContext *context, unsigned int address, WSEHookCallback callback);
};

class WSEJumptableHook : public WSEHook
{
public:
	WSEJumptableHook(WSEContext *context, unsigned int address, WSEHookCallback callback);
};

class WSEMemoryHook : public WSEHook
{
public:
	WSEMemoryHook(WSEContext *context, unsigned int address, unsigned int value, unsigned int size);
};

typedef std::map<unsigned int, WSEHook *> WSEHookMap;

class WSEHookManager
{
public:
	~WSEHookManager();
	virtual void HookFunction(WSEContext *context, unsigned int address, WSEHookCallback callback);
	virtual void HookJumptable(WSEContext *context, unsigned int address, unsigned int index, WSEHookCallback callback);
	virtual void HookMemory(WSEContext *context, unsigned int address, unsigned int value, unsigned int size);
	virtual void UnhookFunction(WSEContext *context, unsigned int address);
	virtual void UnhookJumptable(WSEContext *context, unsigned int address, unsigned int index);
	virtual void UnhookMemory(WSEContext *context, unsigned int address);
	virtual void HookFunctionConditional(WSEContext *context, bool condition, unsigned int address, WSEHookCallback callback);
	virtual void HookJumptableConditional(WSEContext *context, bool condition, unsigned int address, unsigned int index, WSEHookCallback callback);
	virtual void UnhookAll(WSEContext *context = nullptr);

private:
	WSEHookMap m_hooks;
};
