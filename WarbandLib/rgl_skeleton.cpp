#include "rgl_skeleton.h"

using namespace rgl;

const matrix &skeleton::get_bone_position(const int &bone_no)
{
	THISCALL1(rgl::addresses::skeleton_GetBonePosition, this, bone_no);
}

void skeleton::remove_meta_mesh(meta_mesh *meta_mesh)
{
	this->meta_meshes.remove(meta_mesh);

	for (int i = 0; i < num_bones; ++i)
	{
		this->bones[i].remove_meta_mesh(meta_mesh);
	}
}
