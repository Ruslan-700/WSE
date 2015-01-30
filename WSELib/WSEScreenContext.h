#pragma once

#include <string>
#include "WSEContext.h"
#include "warband.h"

class WSEScreenContext : public WSEContext
{
public:
	void Message(unsigned int color, std::string format, ...);
};
