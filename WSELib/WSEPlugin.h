#pragma once

#include "WSE.h"

#define WSE_PLUGIN_API_VERSION 2

typedef void (*WSEPluginLoadFunc)();
typedef void (*WSEPluginUnloadFunc)();
typedef void (*WSEPluginEventFunc)(WSEContext *, WSEEvent);

struct WSEPluginInfo
{
	char m_name[256];
	char m_author[256];
	int m_wse_debug;
	int m_wse_type;
	int m_wse_version_major;
	int m_wse_version_minor;
	int m_version_major;
	int m_version_minor;
	WSEPluginLoadFunc m_load_func;
	WSEPluginUnloadFunc m_unload_func;
	WSEPluginEventFunc m_event_func;
};

typedef bool (*WSEPluginInitFunc)(int, WSEPluginInfo *, WSEEnvironment *);
