#pragma once

#include "WSEOperationContext.h"
#include "warband.h"

class WSESkinOperationsContext : public WSEOperationContext
{
public:
	WSESkinOperationsContext();

protected:
	virtual void OnLoad();

public:
	wb::face_keys m_face_key_registers[NUM_REGISTERS];
};
