#include "WSEContext.h"

#include "WSE.h"

WSEContext::WSEContext()
{
	m_loaded = false;
}

WSEContext::~WSEContext()
{
	Unload();
}

void WSEContext::Load()
{
	if (m_loaded)
		Unload();

	OnLoad();
	m_loaded = true;
}

void WSEContext::Unload()
{
	if (!m_loaded)
		return;

	WSE->Hooks.UnhookAll(this);
	OnUnload();
	m_loaded = false;
}

void WSEContext::Event(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ForceUnload:
		Unload();
		break;
	}

	OnEvent(sender, evt);
}

bool WSEContext::IsLoaded() const
{
	return m_loaded;
}

void WSEContext::OnLoad()
{
}

void WSEContext::OnUnload()
{
}

void WSEContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
}
