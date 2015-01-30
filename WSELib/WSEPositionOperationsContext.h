#pragma once

#include "WSEOperationContext.h"

class WSEPositionOperationsContext : public WSEOperationContext
{
public:
	WSEPositionOperationsContext();

protected:
	virtual void OnLoad();
};
