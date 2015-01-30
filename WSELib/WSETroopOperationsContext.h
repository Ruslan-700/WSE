#pragma once

#include "WSEOperationContext.h"

class WSETroopOperationsContext : public WSEOperationContext
{
public:
	WSETroopOperationsContext();

protected:
	virtual void OnLoad();
};
