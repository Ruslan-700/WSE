#pragma once

#include <string>
#include "WSEContext.h"
#include "warband.h"

class WSEScreenContext : public WSEContext
{
protected:
	virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

public:
	void Message(unsigned int color, std::string format, ...);
};
