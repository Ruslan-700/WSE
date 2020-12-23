#pragma once

#include "WSEOperationContext.h"

class WSESiteOperationsContext : public WSEOperationContext
{
public:
	WSESiteOperationsContext();
	rgl::string getTerrainCode();
	void setTerrainCode(const rgl::string &code);
	void setDisableGrass(int value);
	void setPlaceRiver(int value);
	void setDeepWater(int value);
	void setSizeX(float value);
	void setSizeY(float value);
	void setHillHeight(float value);
	void setValley(float value);
	void setRuggedness(int value);
	void setVegetation(float value);
	void setRegionType(int value);
	void setRegionDetail(int value);
	void setSeed(int index, int value);

protected:
	virtual void OnLoad();

public:
	unsigned int m_keys[MB_NUM_TERRAIN_KEYS];
};
