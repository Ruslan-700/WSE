#pragma once

#include <list>

enum WSEEvent
{
	ForceUnload,
	LoadOperations,
	ModuleLoad,
	GameLoad,
	OnFrame,
	OnRglLogMsg,
	OnWSEScriptEvent,
	OnChatMessageReceived,
};

class WSEContext
{
public:
	WSEContext();
	virtual ~WSEContext();
	void Load();
	void Unload();
	void Event(WSEContext *sender, WSEEvent evt, void *data);
	bool IsLoaded() const;

protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

private:
	bool m_loaded;
};
