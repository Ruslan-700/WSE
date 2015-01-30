#pragma once

#include "WSEOperationContext.h"

class WSEMultiplayerOperationsContext : public WSEOperationContext
{
public:
	WSEMultiplayerOperationsContext();
	wb::network_buffer *GetCurrentNetworkBuffer();

protected:
	virtual void OnLoad();

public:
	wb::network_buffer m_network_buffer_registers[NUM_REGISTERS];
};
