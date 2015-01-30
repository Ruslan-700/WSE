#pragma once

#include "WSEOperationContext.h"

class WSEMissionOperationsContext : public WSEOperationContext
{
public:
	WSEMissionOperationsContext();

protected:
	virtual void OnLoad();
};
