#include "rgl_addresses.h"

using namespace rgl;

#if defined WARBAND_VANILLA
//unsigned int addresses::New = 0x00682814;
unsigned int addresses::New = 0x0062BA96;
//unsigned int addresses::Delete = 0x00682194;
unsigned int addresses::Delete = 0x0062B2B9;

//unsigned int addresses::window_manager_DisplayMessage = 0x00490700;
unsigned int addresses::window_manager_DisplayMessage = 0x00462430;

//unsigned int addresses::mesh_CreateVertexAnimMorph = 0x00556420;
//unsigned int addresses::mesh_CreateCopy = 0x00422B30;
//unsigned int addresses::mesh_CreateUniqueVertexData = 0x00422DB0;
unsigned int addresses::mesh_CreateVertexAnimMorph = 0x0050F540;
unsigned int addresses::mesh_CreateCopy = 0x0041BE60;
unsigned int addresses::mesh_CreateUniqueVertexData = 0x0041C0E0;

//unsigned int addresses::skeleton_GetBonePosition = 0x004E35B0;
unsigned int addresses::skeleton_GetBonePosition = 0x004A92E0;

//unsigned int addresses::scene_GetGroundData = 0x00437660;
unsigned int addresses::scene_GetGroundData = 0x00429D90;

//unsigned int addresses::CheckIntersection = 0x00417CF0;
unsigned int addresses::CheckIntersection = 0x00413D80;

//unsigned int addresses::scene_GetBodyPartsInPath = 0x00448740;
unsigned int addresses::scene_GetBodyPartsInPath = 0x004359F0;

//unsigned int addresses::capsule_Ctor = 0x00447E00;
unsigned int addresses::capsule_Ctor = 0x004350A0;
#elif defined WARBAND_STEAM
unsigned int addresses::New = 0x00678FE4;
unsigned int addresses::Delete = 0x00678964;

unsigned int addresses::window_manager_DisplayMessage = 0x00490700;

unsigned int addresses::mesh_CreateVertexAnimMorph = 0x0055A1C0;
unsigned int addresses::mesh_CreateCopy = 0x00422B30;
unsigned int addresses::mesh_CreateUniqueVertexData = 0x00422DB0;

unsigned int addresses::skeleton_GetBonePosition = 0x004E6FD0;

unsigned int addresses::scene_GetGroundData = 0x00437580;

unsigned int addresses::CheckIntersection = 0x00417CF0;

unsigned int addresses::scene_GetBodyPartsInPath = 0x00448720;

unsigned int addresses::capsule_Ctor = 0x00447DE0;
#elif defined WARBAND_DEDICATED
//unsigned int addresses::New = 0x004F1C0D;
unsigned int addresses::New = 0x004FB19D;
//unsigned int addresses::Delete = 0x004F1B0D;
unsigned int addresses::Delete = 0x004FB09D;

unsigned int addresses::window_manager_DisplayMessage = 0;

unsigned int addresses::mesh_CreateVertexAnimMorph = 0;
unsigned int addresses::mesh_CreateCopy = 0;
unsigned int addresses::mesh_CreateUniqueVertexData = 0;

//unsigned int addresses::skeleton_GetBonePosition = 0x00456740;
unsigned int addresses::skeleton_GetBonePosition = 0x00458180;

//unsigned int addresses::scene_GetGroundData = 0x0040F6A0;
unsigned int addresses::scene_GetGroundData = 0x00410CF0;

unsigned int addresses::CheckIntersection = 0x004068E0;

//unsigned int addresses::scene_GetBodyPartsInPath = 0x0044F270;
unsigned int addresses::scene_GetBodyPartsInPath = 0x00450810;

//unsigned int addresses::capsule_Ctor = 0x0044E720;
unsigned int addresses::capsule_Ctor = 0x0044FCB0;
#endif
