#pragma once

#include "WSEOperationContext.h"
#include "mup\muParser.h"

class WSEFloatingPointOperationsContext : public WSEOperationContext
{
public:
	WSEFloatingPointOperationsContext();

protected:
	virtual void OnLoad();
	virtual void OnUnload();

public:
	float m_float_registers[NUM_REGISTERS];
	mu::Parser m_parser;
};
