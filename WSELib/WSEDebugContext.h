#pragma once

#include "WSEContext.h"
#include "warband.h"

class WSEDebugContext : public WSEContext
{
public:
	WSEDebugContext();
	virtual void OnLoad();
};
