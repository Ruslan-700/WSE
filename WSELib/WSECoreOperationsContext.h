#pragma once

#include "WSEOperationContext.h"
#include <random>

#define NUM_RETURN_VALUES 16

class WSECoreOperationsContext : public WSEOperationContext
{
public:
	WSECoreOperationsContext();

protected:
	virtual void OnLoad();

public:
	__int64 m_return_values[NUM_RETURN_VALUES];
	int m_num_return_values;
	std::mt19937 m_mersenne_twister;
	rgl::timer m_timer_registers[NUM_REGISTERS];
};
