#include "WSESiteOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void SiteSetFlags(WSESiteOperationsContext *context)
{
	int site_no, flags;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(flags);

	wb::site *site = &warband->cur_game->sites[site_no];

	site->flags = flags;
	site->descriptor_checksum = 0;
}

void SiteSetWaterLevel(WSESiteOperationsContext *context)
{
	int site_no;
	float water_level;

	context->ExtractSiteNo(site_no);
	context->ExtractFixedPoint(water_level);

	wb::site *site = &warband->cur_game->sites[site_no];

	site->water_level = water_level;
	site->descriptor_checksum = 0;
}

void SiteSetBounds(WSESiteOperationsContext *context)
{
	int site_no;
	float min_x, min_y, max_x, max_y;

	context->ExtractSiteNo(site_no);
	context->ExtractFixedPoint(min_x);
	context->ExtractFixedPoint(min_y);
	context->ExtractFixedPoint(max_x);
	context->ExtractFixedPoint(max_y);

	wb::site *site = &warband->cur_game->sites[site_no];
	
	site->min_x = min_x;
	site->min_y = min_y;
	site->max_x = max_x;
	site->max_y = max_y;
	site->descriptor_checksum = 0;
}

void SiteSetOuterTerrain(WSESiteOperationsContext *context)
{
	int site_no;
	rgl::string outer_terrain;

	context->ExtractSiteNo(site_no);
	context->ExtractString(outer_terrain);

	wb::site *site = &warband->cur_game->sites[site_no];

	if (outer_terrain != "0" && !warband->resource_manager.get_mesh(outer_terrain))
		return;

	site->outer_terrain_name = outer_terrain;
	site->descriptor_checksum = 0;
}

void SiteSetTerrainSeed(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setSeed(0, value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetRiverSeed(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setSeed(1, value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetFloraSeed(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setSeed(2, value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetDeepWater(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setDeepWater(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetPlaceRiver(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setPlaceRiver(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetDisableGrass(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setDisableGrass(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetValleySize(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setValley(value / 100.0f);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetHillHeight(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setHillHeight((float)value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetRuggedness(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setRuggedness(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetVegetation(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setVegetation(value / 100.0f);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetSize(WSESiteOperationsContext *context)
{
	int site_no, x, y;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(x);
	context->ExtractValue(y);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setSizeX((float)x);
	context->setSizeY((float)y);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetRegionType(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setRegionType(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

void SiteSetRegionDetail(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	wb::site *site = &warband->cur_game->sites[site_no];

	context->setTerrainCode(site->terrain_code);
	context->setRegionDetail(value);
	site->terrain_code = context->getTerrainCode();
	site->descriptor_checksum = 0;
}

WSESiteOperationsContext::WSESiteOperationsContext() : WSEOperationContext("site", 4500, 4599)
{
}

rgl::string WSESiteOperationsContext::getTerrainCode()
{
	char buf[51];

	sprintf_s(buf, "0x%.8x%.8x%.8x%.8x%.8x%.8x", m_keys[5], m_keys[4], m_keys[3], m_keys[2], m_keys[1], m_keys[0]);
	return buf;
}

void WSESiteOperationsContext::setTerrainCode(const rgl::string &code)
{
	for (int i = 0; i < MB_NUM_TERRAIN_KEYS; ++i)
	{
		m_keys[i] = 0;
	}

	int startPos = 0;
	int key = 0;

	if (code[0] == '0' && (code[1] == 'x' || code[1] == 'X'))
		startPos = 2;

	for (int i = code.length(); i >= startPos && key < MB_NUM_TERRAIN_KEYS; i -= 8)
	{
		rgl::string keyCode = code.substr(rglMax(i - 8, startPos), i);
		unsigned int keyValue = 0;

		for (int j = 0; j < keyCode.length(); ++j)
		{
			char c = keyCode[j];
			int val = 0;

			if (c - '0' <= 9)
				val = c - '0';
			else if (c - 'a' <= 5)
				val = c - 'W';
			else if (c - 'A' <= 5)
				val = c - '7';

			keyValue = keyValue * 16 + val;
		}

		m_keys[key++] = keyValue;
	}
}

void WSESiteOperationsContext::setDisableGrass(int value)
{
	SETFLAGVALUE(m_keys[5], tk5_disable_grass_mask, tk5_disable_grass_shift, rglMin(value, 2));
}

void WSESiteOperationsContext::setPlaceRiver(int value)
{
	SETFLAGVALUE(m_keys[3], tk3_place_river_mask, tk3_place_river_shift, rglMin(value, 1));
}

void WSESiteOperationsContext::setDeepWater(int value)
{
	SETFLAGVALUE(m_keys[1], tk1_deep_water_mask, tk1_deep_water_shift, rglMin(value, 1));
}

void WSESiteOperationsContext::setSizeX(float value)
{
	SETFLAGVALUE(m_keys[3], tk3_size_x_mask, tk3_size_x_shift, rglMin((int)value, 1023));
}

void WSESiteOperationsContext::setSizeY(float value)
{
	SETFLAGVALUE(m_keys[3], tk3_size_y_mask, tk3_size_y_shift, rglMin((int)value, 1023));
}

void WSESiteOperationsContext::setHillHeight(float value)
{
	SETFLAGVALUE(m_keys[4], tk4_hill_height_mask, tk4_hill_height_shift, rglClampExclusive((int)value, 0, 128));
}

void WSESiteOperationsContext::setValley(float value)
{
	SETFLAGVALUE(m_keys[4], tk4_valley_mask, tk4_valley_shift, rglClampExclusive(rglRound(value * 100.0f), 0, 100));
}

void WSESiteOperationsContext::setRuggedness(int value)
{
	SETFLAGVALUE(m_keys[4], tk4_ruggedness_mask, tk4_ruggedness_shift, rglMin(value, 127));
}

void WSESiteOperationsContext::setVegetation(float value)
{
	SETFLAGVALUE(m_keys[4], tk4_vegetation_mask, tk4_vegetation_shift, rglClampExclusive(rglRound(value * 100.0f), 0, 100));
}

void WSESiteOperationsContext::setRegionType(int value)
{
	SETFLAGVALUE(m_keys[4], tk4_region_type_mask, tk4_region_type_shift, rglMin(value, 16));
}

void WSESiteOperationsContext::setRegionDetail(int value)
{
	SETFLAGVALUE(m_keys[5], tk5_region_detail_mask, tk5_region_detail_shift, rglMin(value, 3));
}

void WSESiteOperationsContext::setSeed(int index, int value)
{
	SETFLAGVALUE(m_keys[index], tk_seed_mask, tk_seed_shift, rglMin(value, tk_seed_mask));
}

void WSESiteOperationsContext::OnLoad()
{
	RegisterOperation("scene_set_flags", SiteSetFlags, Both, None, 2, 2,
		"Sets <0>'s flags to <1>",
		"scene_no", "flags");

	RegisterOperation("scene_set_water_level", SiteSetWaterLevel, Both, None, 2, 2,
		"Sets <0>'s water level to <1>",
		"scene_no", "water_level_fixed_point");

	RegisterOperation("scene_set_bounds", SiteSetBounds, Both, None, 5, 5,
		"Sets <0>'s bounds to (<1>, <2>) and (<3>, <4>)",
		"scene_no", "min_x_fixed_point", "min_y_fixed_point", "max_x_fixed_point", "max_y_fixed_point");

	RegisterOperation("scene_set_outer_terrain", SiteSetOuterTerrain, Both, None, 2, 2,
		"Sets <0>'s outer terrain to <1>",
		"scene_no", "outer_terrain_mesh_name");

	RegisterOperation("scene_set_terrain_seed", SiteSetTerrainSeed, Both, None, 2, 2,
		"Sets <0>'s terrain generator terrain seed to <1>",
		"scene_no", "value");

	RegisterOperation("scene_set_river_seed", SiteSetRiverSeed, Both, None, 2, 2,
		"Sets <0>'s terrain generator river seed to <1>",
		"scene_no", "value");

	RegisterOperation("scene_set_flora_seed", SiteSetFloraSeed, Both, None, 2, 2,
		"Sets <0>'s terrain generator flora seed to <1>",
		"scene_no", "value");

	RegisterOperation("scene_set_deep_water", SiteSetDeepWater, Both, None, 2, 2,
		"Enables or disables terrain generator deep water for <0>",
		"scene_no", "value");

	RegisterOperation("scene_set_place_river", SiteSetPlaceRiver, Both, None, 2, 2,
		"Enables or disables terrain generator river placement for <0>",
		"scene_no", "value");

	RegisterOperation("scene_set_disable_grass", SiteSetDisableGrass, Both, None, 2, 2,
		"Enables or disables terrain generator grass placement for <0>",
		"scene_no", "value");

	RegisterOperation("scene_set_valley_size", SiteSetValleySize, Both, None, 2, 2,
		"Sets <0>'s terrain generator valley size to <1> (0-127)",
		"scene_no", "value");

	RegisterOperation("scene_set_hill_height", SiteSetHillHeight, Both, None, 2, 2,
		"Sets <0>'s terrain generator hill height to <1> (0-127)",
		"scene_no", "value");

	RegisterOperation("scene_set_ruggedness", SiteSetRuggedness, Both, None, 2, 2,
		"Sets <0>'s terrain generator ruggedness to <1> (0-127)",
		"scene_no", "value");

	RegisterOperation("scene_set_vegetation", SiteSetVegetation, Both, None, 2, 2,
		"Sets <0>'s terrain generator vegetation to <1> (0-127)",
		"scene_no", "value");

	RegisterOperation("scene_set_size", SiteSetSize, Both, None, 3, 3,
		"Sets <0>'s terrain generator map size to (<1>, <2>) (0-1023)",
		"scene_no", "x", "y");

	RegisterOperation("scene_set_region_type", SiteSetRegionType, Both, None, 2, 2,
		"Sets <0>'s terrain generator region type to <1> (0-15)",
		"scene_no", "value");

	RegisterOperation("scene_set_region_detail", SiteSetRegionDetail, Both, None, 2, 2,
		"Sets <0>'s terrain generator region detail to <1> (0-3)",
		"scene_no", "value");
}
