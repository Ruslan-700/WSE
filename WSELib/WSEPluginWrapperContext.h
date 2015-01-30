#pragma once

#include "WSE.h"

struct WSEPluginInfo;

class WSEPluginWrapperContext : public WSEContext
{
public:
	WSEPluginWrapperContext(WSEPluginInfo *info);
	
protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

private:
	WSEPluginInfo *m_info;
};
