#pragma once

#include "WSEContext.h"
#include "warband.h"

class WSEPerformanceContext : public WSEContext
{
public:
	WSEPerformanceContext();
	
protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

private:
	HANDLE m_optimizer_thread_handle;
};
