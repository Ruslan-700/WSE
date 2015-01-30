#include "WSEPluginWrapperContext.h"

#include "WSEPlugin.h"

WSEPluginWrapperContext::WSEPluginWrapperContext(WSEPluginInfo *info)
{
	m_info = info;
}

void WSEPluginWrapperContext::OnLoad()
{
	if (m_info->m_load_func)
		m_info->m_load_func();
}

void WSEPluginWrapperContext::OnUnload()
{
	if (m_info->m_unload_func)
		m_info->m_unload_func();
}

void WSEPluginWrapperContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	if (m_info->m_event_func)
		m_info->m_event_func(sender, evt);
}
