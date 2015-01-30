#include "WSEHookManager.h"

#include <Windows.h>
#include <algorithm>
#include "WSE.h"

WSEHook::WSEHook(WSEContext *context, void *address, int num_bytes)
{
	m_context = context;
	m_address = address;
	m_num_bytes = num_bytes;

	memcpy_s(m_backup, sizeof(m_backup), m_address, num_bytes);
}

WSEHook::~WSEHook()
{
	Restore();
}

void WSEHook::Restore() const
{
	Write(m_backup);
}

bool WSEHook::IsOwnedBy(WSEContext *context) const
{
	return m_context == context;
}

void WSEHook::Write(const unsigned char *data) const
{
	DWORD prot_old, prot_new;

	VirtualProtect(m_address, m_num_bytes, PAGE_EXECUTE_READWRITE, &prot_old);
	memcpy_s(m_address, m_num_bytes, data, m_num_bytes);
	VirtualProtect(m_address, m_num_bytes, prot_old, &prot_new);
}


WSEFunctionHook::WSEFunctionHook(WSEContext *context, unsigned int address, WSEHookCallback callback) : WSEHook(context, (void *)address, 5)
{
	m_asm[0] = 0xE9;
	*(unsigned int *)(m_asm + 1) = ((unsigned int)callback - (address + 5));
	Write(m_asm);
}

WSEJumptableHook::WSEJumptableHook(WSEContext *context, unsigned int address, WSEHookCallback callback) : WSEHook(context, (void *)address, 4)
{
	*(unsigned int *)m_asm = (unsigned int)callback;
	Write(m_asm);
}

WSEMemoryHook::WSEMemoryHook(WSEContext *context, unsigned int address, unsigned int value, unsigned int size) : WSEHook(context, (void *)address, size)
{
	*(unsigned int *)m_asm = value;
	Write(m_asm);
}

WSEHookManager::~WSEHookManager()
{
	if (m_hooks.size())
		WSE->Log.Error("not all context hooks were unloaded.");
}

void WSEHookManager::HookFunction(WSEContext *context, unsigned int address, WSEHookCallback callback)
{
	if (!address)
		return;

	if (m_hooks.find(address) != m_hooks.end())
	{
		WSE->Log.Debug("trying to override function hook at %p. Skipped.", address);
		return;
	}

	m_hooks[address] = new WSEFunctionHook(context, address, callback);
}

void WSEHookManager::HookJumptable(WSEContext *context, unsigned int address, unsigned int index, WSEHookCallback callback)
{
	if (!address)
		return;

	address = address + (4 * index);

	if (m_hooks.find(address) != m_hooks.end())
	{
		WSE->Log.Debug("%p trying to override jumptable hook at %p. Skipped.", this, address);
		return;
	}

	m_hooks[address] = new WSEJumptableHook(context, address, callback);
}

void WSEHookManager::HookMemory(WSEContext *context, unsigned int address, unsigned int value, unsigned int size)
{
	if (!address)
		return;
	
	if (m_hooks.find(address) != m_hooks.end())
	{
		WSE->Log.Debug("%p trying to override memory hook at %p. Skipped.", this, address);
		return;
	}

	m_hooks[address] = new WSEMemoryHook(context, address, value, size);
}

void WSEHookManager::UnhookFunction(WSEContext *context, unsigned int address)
{
	UnhookMemory(context, address);
}

void WSEHookManager::UnhookJumptable(WSEContext *context, unsigned int address, unsigned int index)
{
	UnhookFunction(context, address + (4 * index));
}

void WSEHookManager::UnhookMemory(WSEContext *context, unsigned int address)
{
	WSEHookMap::const_iterator it = m_hooks.find(address);

	if (m_hooks.find(address) != m_hooks.end() && it->second->IsOwnedBy(context))
	{
		delete it->second;
		m_hooks.erase(it);
	}
}

void WSEHookManager::HookFunctionConditional(WSEContext *context, bool condition, unsigned int address, WSEHookCallback callback)
{
	if (condition)
		HookFunction(context, address, callback);
	else
		UnhookFunction(context, address);
}

void WSEHookManager::HookJumptableConditional(WSEContext *context, bool condition, unsigned int address, unsigned int index, WSEHookCallback callback)
{
	if (condition)
		HookJumptable(context, address, index, callback);
	else
		UnhookJumptable(context, address, index);
}

void WSEHookManager::UnhookAll(WSEContext *context)
{
	std::vector<unsigned int> hooks;
	WSEHookMap::const_iterator end = m_hooks.end();

	for (WSEHookMap::const_iterator it = m_hooks.begin(); it != end; ++it)
	{
		if (!context || it->second->IsOwnedBy(context))
			hooks.push_back(it->first);
	}

	for (size_t i = 0; i < hooks.size(); ++i)
	{
		delete m_hooks[hooks[i]];
		m_hooks.erase(hooks[i]);
	}
}
