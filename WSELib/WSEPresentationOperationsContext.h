#pragma once

#include "WSEOperationContext.h"

class WSEPresentationOperationsContext : public WSEOperationContext
{
public:
	WSEPresentationOperationsContext();

protected:
	virtual void OnLoad();
};
