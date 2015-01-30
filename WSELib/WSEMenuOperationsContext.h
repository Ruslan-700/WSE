#pragma once

#include "WSEOperationContext.h"

class WSEMenuOperationsContext : public WSEOperationContext
{
public:
	WSEMenuOperationsContext();

protected:
	virtual void OnLoad();

public:
	int m_num_base_menus;
};
