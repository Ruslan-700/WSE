#pragma once

namespace rgl
{
	struct addresses
	{
		static unsigned int New;
		static unsigned int Delete;

		static unsigned int window_manager_DisplayMessage;

		static unsigned int mesh_CreateVertexAnimMorph;
		static unsigned int mesh_CreateCopy;
		static unsigned int mesh_CreateUniqueVertexData;

		static unsigned int skeleton_GetBonePosition;

		static unsigned int scene_GetGroundData;

		static unsigned int CheckIntersection;

		static unsigned int scene_GetBodyPartsInPath;

		static unsigned int capsule_Ctor;
	};
}
