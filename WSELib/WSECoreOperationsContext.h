#pragma once

#include "WSEOperationContext.h"
#include <random>

class WSECoreOperationsContext : public WSEOperationContext
{
public:
	WSECoreOperationsContext();

protected:
	virtual void OnLoad();

public:
	int m_return_values[16];
	int m_num_return_values;
	std::mt19937 m_mersenne_twister;
	rgl::timer m_timer_registers[NUM_REGISTERS];
};
