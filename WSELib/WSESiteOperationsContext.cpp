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

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 0, 0, 31, value);
}

void SiteSetRiverSeed(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 1, 0, 31, value);
}

void SiteSetFloraSeed(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 2, 0, 31, value);
}

void SiteSetDeepWater(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 1, 31, 1, value);
}

void SiteSetPlaceRiver(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 3, 31, 1, value);
}

void SiteSetDisableGrass(WSESiteOperationsContext *context)
{
	int site_no;
	bool value;

	context->ExtractSiteNo(site_no);
	context->ExtractBoolean(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 5, 2, 2, value);
}

void SiteSetValleySize(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 4, 0, 7, value);
}

void SiteSetHillHeight(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 4, 7, 7, value);
}

void SiteSetRuggedness(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 4, 14, 7, value);
}

void SiteSetVegetation(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 4, 21, 7, value);
}

void SiteSetSize(WSESiteOperationsContext *context)
{
	int site_no, x, y;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(x);
	context->ExtractValue(y);
	
	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 3, 0, 10, x);
	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 3, 10, 10, y);
}

void SiteSetRegionType(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 4, 28, 4, value);
}

void SiteSetRegionDetail(WSESiteOperationsContext *context)
{
	int site_no, value;

	context->ExtractSiteNo(site_no);
	context->ExtractValue(value);

	context->SetSiteTerrainCode(&warband->cur_game->sites[site_no], 5, 0, 2, value);
}

WSESiteOperationsContext::WSESiteOperationsContext() : WSEOperationContext("site", 4500, 4599)
{
}

void WSESiteOperationsContext::SetSiteTerrainCode(wb::site *site, int key_no, int shift, int bits, int value)
{
	std::string str = site->terrain_code;
	unsigned int keys[6];

	if (str[1] == 'x' || str[1] == 'X')
		str = str.substr(2);

	int len = str.length();
	int cur_key = 5;

	for (int i = 5; i >= 0; --i)
	{
		keys[i] = 0;
	}

	while (len > 0 && cur_key >= 0)
	{
		int start = str.length() - 8;

		if (start < 0)
			start = 0;

		size_t length = 8;

		if (length > str.length())
			length = str.length();

		std::string key_str = str.substr(start, length);
		
		keys[cur_key] = strtoul(key_str.c_str(), nullptr, 16);
		str.erase(str.length() - length);
		len -= 8;
		cur_key--;
	}
	
	unsigned int mask = (unsigned int)((((unsigned __int64)1) << bits) - 1);

	keys[key_no] &= ~(mask << shift);
	keys[key_no] |= ((value & mask) << shift);
	
	char buf[51];

	sprintf_s(buf, "0x%.8X%.8X%.8X%.8X%.8X%.8X", keys[0], keys[1], keys[2], keys[3], keys[4], keys[5]);
	site->terrain_code = buf;
	site->descriptor_checksum = 0;
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
