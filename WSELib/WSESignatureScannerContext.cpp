#include "WSESignatureScannerContext.h"

#include "WSE.h"
#include <TlHelp32.h>

void WSESignatureScannerContext::OnLoad()
{
	DWORD TargetId = GetCurrentProcessId();
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	Process32First(handle, &entry);
	do {
		if (TargetId == entry.th32ProcessID){
			CloseHandle(handle);
			TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, false, TargetId);
			break;
		}
	} while (Process32Next(handle, &entry));

	HANDLE hmodule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, TargetId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(hmodule, &mEntry);
	do {
		if (!strcmp(mEntry.szModule, entry.szExeFile)) {
			CloseHandle(hmodule);
			mod = { (DWORD)mEntry.hModule, mEntry.modBaseSize };
			break;
		}
	} while (Module32Next(hmodule, &mEntry));

	FindRglAddresses();
	FindWbAddresses();
	warband = (wb::game_data *)(wb::addresses::game_data_ptr);
	rgl::functions::check_intersection = (rgl::CheckIntersection_func)rgl::addresses::CheckIntersection;
	wb::functions::DXUtil_Timer = (wb::DXUtil_Timer_func) wb::addresses::DXUtil_Timer;
}

bool WSESignatureScannerContext::MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++bData, ++bMask) {
		if (*szMask == 'x' && *bData != *bMask) {
			return false;
		}
	}
	return (*szMask == NULL);
}

char * WSESignatureScannerContext::HexDumpStr(const char *src, size_t size)
{
	static const char hex[] = "0123456789ABCDEF";
	char *result = (char *)malloc((size > 0) ? size * 3 : 1),
		*p = result;

	if (result) {
		while (size-- > 0) {
			*p++ = hex[(*src >> 4) & 0x0f];
			*p++ = hex[*src++ & 0x0f];
			*p++ = ' ';
		}
		p[(p > result) ? -1 : 0] = 0;
	}

	return result;
}

DWORD WSESignatureScannerContext::FindSignature(const char* sig, const char* mask)
{
	BYTE* data = new BYTE[mod.dwSize];
	SIZE_T bytesRead;

	ReadProcessMemory(TargetProcess, (LPVOID)mod.dwBase, data, mod.dwSize, &bytesRead);

	int adress = NULL;
	for (DWORD i = 0; i < mod.dwSize; i++)
	{
		if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
			WSE->Log.Info("Address for signature %s %s: %p", HexDumpStr(sig, strlen(mask)), mask, mod.dwBase + i);
			if (adress != NULL) WSE->Log.Error("Found more than one address!");
			adress = mod.dwBase + i;
		}
	}
	delete[] data;
	if (adress != NULL) return adress;
	WSE->Log.Error("Unable to find signature %s %s", HexDumpStr(sig, strlen(mask)), mask);
	return NULL;
}

void WSESignatureScannerContext::FindRglAddresses()
{
#if defined WARBAND_VANILLA
	rgl::addresses::New = FindSignature("\x8B\xFF\x55\x8B\xEC\x83\xEC\x0C\xEB\x0D", "xxxxxxxxxx");

	rgl::addresses::Delete = FindSignature("\x8B\xFF\x55\x8B\xEC\x5D\xE9\x00\x00\x00\x00\x8B\xFF\x55\x8B\xEC\x56", "xxxxxxx????xxxxxx");

	rgl::addresses::window_manager_DisplayMessage = FindSignature("\x81\xEC\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x56\x8B\xF1\x0F\x84\x00\x00\x00\x00\x8D\x4C\x24\x2C", "xx????xx?????xxxxx????xxxx");

	rgl::addresses::mesh_CreateVertexAnimMorph = FindSignature("\x8B\x44\x24\x04\x53\x56\x57\x50", "xxxxxxxx");
	rgl::addresses::mesh_CreateCopy = FindSignature("\x53\x55\x56\x57\x68\x00\x00\x00\x00\x8B\xD9", "xxxxx????xx");
	rgl::addresses::mesh_CreateUniqueVertexData = FindSignature("\x56\x8B\xF1\x80\xBE\xE9\x01\x00\x00\x00\x75\x7D", "xxxxxxxxxxxx");

	rgl::addresses::skeleton_GetBonePosition = FindSignature("\x56\x8B\xF1\x80\x7E\x60\x00", "xxxxxxx");

	rgl::addresses::scene_GetGroundData = FindSignature("\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xD9\x05\x00\x00\x00\x00\x53\x55\x8B\xAC\x24\x00\x00\x00\x00\x56\xD9\x5D\x00", "x????x????xx????xxxxx????xxxx");
	
	rgl::addresses::CheckIntersection = FindSignature("\x53\x55\x56\x57\x8B\x7C\x24\x14\x8B\x07\x8B\x50\x04", "xxxxxxxxxxxxx");
	
	rgl::addresses::scene_GetBodyPartsInPath = FindSignature("\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x94\x24\x00\x00\x00\x00", "x????x????xxx????");
	
	rgl::addresses::capsule_Ctor = FindSignature("\x56\x8B\xF1\xE8\x00\x00\x00\x00\xD9\x44\x24\x08", "xxxx????xxxx");
#elif defined WARBAND_DEDICATED
	rgl::addresses::New = FindSignature("\x8B\xFF\x55\x8B\xEC\x83\xEC\x0C\xEB\x0D", "xxxxxxxxxx");

	rgl::addresses::Delete = FindSignature("\x8B\xFF\x55\x8B\xEC\x5D\xE9\x00\x00\x00\x00\x8B\xFF\x55\x8B\xEC\x56\x8B\x75\x14", "xxxxxxx????xxxxxxxxx");

	rgl::addresses::skeleton_GetBonePosition = FindSignature("\x56\x8B\xF1\x80\x7E\x60\x00", "xxxxxxx");

	rgl::addresses::scene_GetGroundData = FindSignature("\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xD9\x05\x00\x00\x00\x00\x53\x55\x8B\xAC\x24\x00\x00\x00\x00\x56\xD9\x5D\x00", "x????x????xx????xxxxx????xxxx");

	rgl::addresses::CheckIntersection = FindSignature("\x53\x55\x56\x57\x8B\x7C\x24\x14\x8B\x07\x8B\x50\x04", "xxxxxxxxxxxxx");

	rgl::addresses::scene_GetBodyPartsInPath = FindSignature("\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x94\x24\x00\x00\x00\x00", "x????x????xxx????");

	rgl::addresses::capsule_Ctor = FindSignature("\x56\x8B\xF1\xE8\x00\x00\x00\x00\xD9\x44\x24\x08", "xxxx????xxxx");
#endif
	
}

void WSESignatureScannerContext::FindWbAddresses()
{
#if defined WARBAND_VANILLA
	wb::addresses::operation_Execute_jumptable = FindSignature("\x89\x11\x5B\x8B\xE5\x5D\xC2\x0C\x00\x8D\x49\x00", "xxxxxxxxxxxx") + 12;
	wb::addresses::operation_Execute_entry = FindSignature("\x8B\x13\x52\x68\x00\x00\x00\x00", "xxxx????");
	wb::addresses::operation_Execute_exit_1 = FindSignature("\x83\xBC\x24\xC0\x03\x00\x00\x00", "xxxxxxxx");
	wb::addresses::operation_Execute_exit_2 = wb::addresses::operation_Execute_entry + 8;

	wb::addresses::network_server_ReceiveTeamChatEvent_entry = FindSignature("\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x55\x56", "xxxxxxxxxxxx????xx") + 10;
	wb::addresses::network_server_ReceiveTeamChatEvent_exit = wb::addresses::network_server_ReceiveTeamChatEvent_entry + 6;
	
	wb::addresses::network_server_ReceiveChatEvent_entry = FindSignature("\xC2\x08\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x55\x56\x57\x8B\xE9", "xxxxxxxxxxxxxxxx????xxxxx") + 14;
	wb::addresses::network_server_ReceiveChatEvent_exit = wb::addresses::network_server_ReceiveChatEvent_entry + 6;
	
	wb::addresses::ReadModuleFiles_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00", "xx????xxxxxxxxxxx????xxxx????");

	wb::addresses::game_ReadModuleFiles_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x55", "xx????xxxxxxxxxxxxxx????xxxx????x");

	wb::addresses::Save_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC3\x8B\x15\x00\x00\x00\x00\x8B\x42\x28", "xx????xxx????xxx");

	wb::addresses::CheckUrlReplies_entry = FindSignature("\xE8\x00\x00\x00\x00\x5B\x83\xC4\x1C", "x????xxxx");
	wb::addresses::CheckUrlReplies_exit = wb::addresses::CheckUrlReplies_entry + 5;
	wb::addresses::CheckUrlReplies_call = FindSignature("\x80\x7C\x24\x04\x00\x74\x0A\x89\x4C\x24\x04\xFF\x25\x00\x00\x00\x00\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x83\x79\x04\xFF", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxx");

	wb::addresses::network_client_ReceiveMessage_entry = FindSignature("\x0F\x87\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xx????xxx????x????x????");
	wb::addresses::network_client_ReceiveMessage_exit_1 = FindSignature("\x5F\x5E\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x14\x00\x8B\xFF", "xxxxxx????xxxxx");
	wb::addresses::network_client_ReceiveMessage_exit_2 = wb::addresses::network_client_ReceiveMessage_entry + 6;

	wb::addresses::network_server_ReceiveMessage_entry = FindSignature("\x0F\x87\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\x8D\x54\x24\x10", "xx????xxx????xxxx");
	wb::addresses::network_server_ReceiveMessage_exit_1 = FindSignature("\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x5F\x5E\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x14\x00", "x????x????xxx????x????xxx????x????xxxxxx????xxx") + 34;
	wb::addresses::network_server_ReceiveMessage_exit_2 = wb::addresses::network_server_ReceiveMessage_entry + 6;

	wb::addresses::agent_ApplyAttackRecord_entry = FindSignature("\x8B\x88\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x69\xC9\x00\x00\x00\x00\x6A\xE4", "xx????xx????xx????xx");
	wb::addresses::agent_ApplyAttackRecord_exit = FindSignature("\x83\x3D\x00\x00\x00\x00\x00\x7C\x15\x7F\x09\x83\x3D\x00\x00\x00\x00\x00\x72\x0A\xDF\x2D\x00\x00\x00\x00\xD9\x5C\x24\x20", "xx?????xxxxxx?????xxxx????xxxx");

	wb::addresses::CreateMbnetHost_entry = FindSignature("\x5F\x66\x89\x4E\x14", "xxxxx");
	wb::addresses::DestroyMbnetHost_entry = FindSignature("\x83\xC4\x08\x5E\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56", "xxxxxxxxxxxxxx");

	wb::addresses::mission_CheckCollision_entry = FindSignature("\x80\x3A\x00\xB9\x00\x00\x00\x00", "xxxx????");
	wb::addresses::mission_CheckCollision_exit = wb::addresses::mission_CheckCollision_entry + 15;

	wb::addresses::agent_SetGroundQuad_entry = FindSignature("\x89\x96\x00\x00\x00\x00\x5F\x5E\x5B\x8B\xE5\x5D\xC2\x24\x00", "xx????xxxxxxxxx");
	wb::addresses::agent_SetGroundQuad_exit = wb::addresses::agent_SetGroundQuad_entry + 6;

	wb::addresses::agent_ReceiveShieldHit_entry = FindSignature("\x8B\xC8\x89\x4C\x24\x14\xDB\x44\x24\x14", "xxxxxxxxxx");
	wb::addresses::agent_ReceiveShieldHit_exit = wb::addresses::agent_ReceiveShieldHit_entry + 6;

	wb::addresses::mission_SpawnMissile_entry = FindSignature("\x80\x7C\x24\x13\x00\x0F\x84\x00\x00\x00\x00\x85\xFF", "xxxxxxx????xx");
	wb::addresses::mission_SpawnMissile_exit = wb::addresses::mission_SpawnMissile_entry + 5;

	wb::addresses::missile_Dive_entry = FindSignature("\x89\x0D\x00\x00\x00\x00\x8B\x4C\x24\x24", "xx????xxxx");
	wb::addresses::missile_Dive_exit = FindSignature("\x8B\x8E\x00\x00\x00\x00\x80\xB9\x34\x01\x00\x00\x00", "xx????xxxxxxx");

	wb::addresses::network_manager_PopulatePlayerInfoServerEvent_entry = FindSignature("\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x84\x24\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x50", "xxx?????x????xxx????x????x????x");
	wb::addresses::network_manager_PopulatePlayerInfoServerEvent_exit = wb::addresses::network_manager_PopulatePlayerInfoServerEvent_entry + 8;

	wb::addresses::network_manager_PopulateServerOptionsServerEvent_entry = FindSignature("\x81\xEC\x00\x00\x00\x00\x8B\x94\x24\x00\x00\x00\x00\xD9\xEE\x56\xDD\x9C\x24\x00\x00\x00\x00\x57\x8B\xBC\x24\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x8D\x4C\x24\x1C", "xx????xxx????xxxxxx????xxxx????x????xxxx");

	wb::addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = FindSignature("\x56\x8B\xF1\x8B\x06\x83\xF8\x05", "xxxxxxxx");
	wb::addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit = wb::addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry + 5;

	wb::addresses::UpdateHorseAgentEntityBody_entry = FindSignature("\x8B\x8D\x00\x00\x00\x00\x8B\x51\x44\x83\xC1\x40\x89\x44\x24\x10", "xx????xxxxxxxxxx");
	wb::addresses::UpdateHorseAgentEntityBody_exit = wb::addresses::UpdateHorseAgentEntityBody_entry + 6;

	wb::addresses::tactical_window_ShowUseTooltip_entry = FindSignature("\x8B\xB4\x24\x00\x00\x00\x00\x8B\x4C\x24\x14", "xxx????xxxx");
	wb::addresses::tactical_window_ShowUseTooltip_exit = wb::addresses::tactical_window_ShowUseTooltip_entry + 7;

	wb::addresses::tactical_window_ShowCrosshair_entry = FindSignature("\x5B\x81\xC4\x00\x00\x00\x00\xC3\x8D\x49\x00", "xxx????xxxx") + 1;

	wb::addresses::mission_CheckHit_Human_entry = FindSignature("\x8B\x08\x8B\x15\x00\x00\x00\x00\x8B\x52\x20", "xxxx????xxx");
	wb::addresses::mission_CheckHit_Human_exit_1 = FindSignature("\xFF\x44\x24\x44\x83\x7C\x24\x74\x00", "xxxxxxxxx");
	wb::addresses::mission_CheckHit_Human_exit_2 = FindSignature("\x8B\x44\x24\x30\x8B\x54\x24\x20", "xxxxxxxx");

	wb::addresses::mission_CheckHit_Horse_entry = FindSignature("\x8B\xF1\x83\xE6\x0F\x69\xF6\x00\x00\x00\x00\x8B\xC1\xC1\xE8\x04\x8B\x04\x82\xD9\x84\x30\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxxxx????");
	wb::addresses::mission_CheckHit_Horse_exit_1 = FindSignature("\x8B\x7C\x24\x1C\x8B\x75\x18\xC6\x44\x24\x12\x01", "xxxxxxxxxxxx");
	wb::addresses::mission_CheckHit_Horse_exit_2 = FindSignature("\x8B\x7C\x24\x1C\x8B\x75\x18\xC6\x44\x24\x12\x00", "xxxxxxxxxxxx");

	wb::addresses::mission_CheckHit_Prop_entry = FindSignature("\xD9\x44\x24\x44\xDC\x15\x00\x00\x00\x00", "xxxxxx????");
	wb::addresses::mission_CheckHit_Prop_exit_1 = FindSignature("\xD9\x05\x00\x00\x00\x00\x8B\x44\x24\x70", "xx????xxxx");
	wb::addresses::mission_CheckHit_Prop_exit_2 = FindSignature("\xD9\x44\x24\x14\x51\xDC\x25\x00\x00\x00\x00", "xxxxxxx????");

	wb::addresses::agent_HorseCharged_entry = FindSignature("\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00", "x????xxxx????xx?????xx????xxx????");
	wb::addresses::agent_HorseCharged_exit = wb::addresses::agent_HorseCharged_entry + 5;

	wb::addresses::mission_ApplyBlow_entry = FindSignature("\x51\x8B\x44\x24\x0C\xD9\xEE", "xxxxxxx");
	wb::addresses::mission_ApplyBlow_exit = wb::addresses::mission_ApplyBlow_entry + 5;

	wb::addresses::agent_GetScaleHuman_entry = FindSignature("\xD9\x9E\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x0F\x84\x00\x00\x00\x00\xD9\x86\x00\x00\x00\x00\x8D\x4C\x24\x28", "xx????x????xxxxxxx????xx????xxxx");
	wb::addresses::agent_GetScaleHuman_exit = wb::addresses::agent_GetScaleHuman_entry + 6;
	wb::addresses::agent_GetScaleHorse_entry = FindSignature("\xD9\x9E\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x0F\x84\x00\x00\x00\x00\xD9\x86\x00\x00\x00\x00\x8D\x4C\x24\x38", "xx????x????xxxxxxx????xx????xxxx");
	wb::addresses::agent_GetScaleHorse_exit = wb::addresses::agent_GetScaleHorse_entry + 6;

	wb::addresses::mission_object_Hit_entry = FindSignature("\x81\xC1\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00\x7C\x2B", "xx????x????xx?????xx");
	wb::addresses::mission_object_Hit_exit = wb::addresses::mission_object_Hit_entry + 6;

	wb::addresses::game_screen_OpenWindow_entry = FindSignature("\x56\x57\x6A\x01\x8B\xF1\x6A\x00", "xxxxxxxx");

	wb::addresses::item_kind_TransformHoldPosition_entry = FindSignature("\x83\xEC\x10\x53\x56\x8B\x74\x24\x1C\x57\x56", "xxxxxxxxxxx");

	wb::addresses::agent_CancelSwing_entry = FindSignature("\x8B\x86\x00\x00\x00\x00\x33\xFF\x85\xC0\x7C\x1A", "xx????xxxxxx");
	wb::addresses::agent_CancelSwing_exit = FindSignature("\x85\xFF\x74\x4D\x8B\x07", "xxxxxx");

	wb::addresses::UpdateAgentEntityBody = FindSignature("\x81\xC4\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x53\x33\xDB", "xx????xxxxxxxxxxxx????xxx");

	wb::addresses::agent_DropItem_entry = FindSignature("\x81\xEC\x00\x00\x00\x00\x53\x55\x8B\xAC\x24\x00\x00\x00\x00\x8B\x45\x00\x8B\xD9", "xx????xxxxx????xxxxx");
	wb::addresses::agent_DropItem_exit = wb::addresses::agent_DropItem_entry + 6;

	wb::addresses::agent_StartReloading_entry = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x8B\x45\x08", "xxxx????xxxxxx");
	wb::addresses::agent_StartReloadingClient_entry = FindSignature("\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00\x8B\xCD\xE8\x00\x00\x00\x00\x5F", "xx????????xxx????x");
	wb::addresses::agent_StartReloadingClient_exit = wb::addresses::agent_StartReloadingClient_entry + 10;

	wb::addresses::agent_EndReloading_entry_1 = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x83\xF8\x02", "xxxx????xxxxxx");
	wb::addresses::agent_EndReloading_entry_2 = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x39\x35\x00\x00\x00\x00", "xxxx????xxxxx????");

	wb::addresses::item_Difficulty_entry = FindSignature("\x8B\x54\x24\x08\x56\x8B\x74\x24\x08", "xxxxxxxxx");

	wb::addresses::mission_object_WeaponKnockBack_entry = FindSignature("\xC6\x44\x24\x1B\x01\x8B\xC1", "xxxxxxx");
	wb::addresses::mission_object_WeaponKnockBack_exit = wb::addresses::mission_object_WeaponKnockBack_entry + 5;

	wb::addresses::item_kind_ShieldNoParry_entry = FindSignature("\x83\x38\x00\x7E\x17", "xxxxx");
	wb::addresses::item_kind_ShieldNoParry_exit_1 = wb::addresses::item_kind_ShieldNoParry_entry + 5;
	wb::addresses::item_kind_ShieldNoParry_exit_2 = FindSignature("\x85\xFF\x7F\x17\x5F", "xxxxx");

	wb::addresses::item_kind_ShieldNoParrySound_entry = FindSignature("\x85\xF6\x0F\x8C\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xC6\x69\xC0\x00\x00\x00\x00\x80\xBC\x08\xD0\x0B\x00\x00\x07\x0F\x85\x00\x00\x00\x00\x8B\x45\x10", "xxxx????xx????xxxx????xxxxxxxxxx????xxx");
	wb::addresses::item_kind_ShieldNoParrySound_exit_1 = wb::addresses::item_kind_ShieldNoParrySound_entry + 8;
	wb::addresses::item_kind_ShieldNoParrySound_exit_2 = FindSignature("\x6A\x00\x8D\x8C\x24\x00\x00\x00\x00\x51\x8B\xCF", "xxxxx????xxx");

	wb::addresses::item_kind_ShieldNoParryDamage_entry = FindSignature("\x85\xF6\x0F\x8C\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xC6\x69\xC0\x00\x00\x00\x00\x80\xBC\x08\xD0\x0B\x00\x00\x07\x0F\x85\x00\x00\x00\x00\x8B\x4D\x18", "xxxx????xx????xxxx????xxxxxxxxxx????xxx");
	wb::addresses::item_kind_ShieldNoParryDamage_exit_1 = wb::addresses::item_kind_ShieldNoParryDamage_entry + 8;
	wb::addresses::item_kind_ShieldNoParryDamage_exit_2 = FindSignature("\x83\x7C\x24\x68\x00\x75\x0B", "xxxxxxx");

	wb::addresses::item_kind_ShieldNoParryMissiles_entry = FindSignature("\x85\xC0\x0F\x8C\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x69\xC0\x00\x00\x00\x00\x80\xBC\x10\xD0\x0B\x00\x00\x07", "xxxx????xx????xx????xxxxxxxx");
	wb::addresses::item_kind_ShieldNoParryMissiles_exit_1 = wb::addresses::item_kind_ShieldNoParryMissiles_entry + 8;
	wb::addresses::item_kind_ShieldNoParryMissiles_exit_2 = FindSignature("\x5F\x5E\x5D\x33\xC0\x5B\x81\xC4\x00\x00\x00\x00\xC2\x20\x00\xDD\xD8", "xxxxxxxx????xxxxx");

	wb::addresses::item_kind_ShieldNoParrySpeed_entry = FindSignature("\x83\x38\x00\x7C\x0F", "xxxxx");
	wb::addresses::item_kind_ShieldNoParrySpeed_exit_1 = wb::addresses::item_kind_ShieldNoParrySpeed_entry + 5;
	wb::addresses::item_kind_ShieldNoParrySpeed_exit_2 = FindSignature("\xD9\x05\x00\x00\x00\x00\x8D\x54\x24\x1C\x52", "xx????xxxxx");

	wb::addresses::item_kind_ShieldNoParryCarry_entry = FindSignature("\x83\xF8\x07\x0F\x84\x00\x00\x00\x00\x83\xF8\x09", "xxxxx????xxx");
	wb::addresses::item_kind_ShieldNoParryCarry_exit_1 = wb::addresses::item_kind_ShieldNoParryCarry_entry + 9;
	wb::addresses::item_kind_ShieldNoParryCarry_exit_2 = FindSignature("\x8B\x7C\x24\x18\x83\x44\x24\x1C\x08", "xxxxxxxxx");

	wb::addresses::item_kind_ShieldNoParryCouchedLance_entry = FindSignature("\x83\x38\x00\x0F\x8C\x00\x00\x00\x00\x6A\x01\x8D\x44\x24\x20", "xxxxx????xxxxxx");
	wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_1 = wb::addresses::item_kind_ShieldNoParryCouchedLance_entry + 9;
	wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_2 = FindSignature("\x5F\x5E\x5D\x33\xC0\x5B\x81\xC4\x00\x00\x00\x00\xC2\x20\x00\xDD\xD8", "xxxxxxxx????xxxxx");

	wb::addresses::item_kind_DisableAgentSoundsHorseShort_entry = FindSignature("\xB8\x00\x00\x00\x00\x8D\x4C\x24\x18\xD9\x5C\x24\x18", "x????xxxxxxxx");
	wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_1 = wb::addresses::item_kind_DisableAgentSoundsHorseShort_entry + 5;
	wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_2 = FindSignature("\x8B\x07\x8B\x14\xC5\x00\x00\x00\x00\x89\x57\x08\x8B\x04\xC5\x00\x00\x00\x00\x89\x47\x0C\xD9\x06", "xxxxx????xxxxxx????xxxxx");

	wb::addresses::agent_BlockedAttack_entry = FindSignature("\x8B\x44\x24\x68\x89\x44\x24\x74", "xxxxxxxx");
	wb::addresses::agent_BlockedAttack_exit = wb::addresses::agent_BlockedAttack_entry + 8;

	wb::addresses::agent_Turn_entry = FindSignature("\xD9\x5C\x24\x18\xD9\x44\x24\x14\xD9\x86\x00\x00\x00\x00", "xxxxxxxxxx????");
	wb::addresses::agent_Turn_exit = wb::addresses::agent_Turn_entry + 8;

	wb::addresses::makeScreenShot = FindSignature("\x81\xEC\x00\x00\x00\x00\x80\x3D\x65\xC0\x85\x00\x00\x0F\x84\x00\x00\x00\x00\x8B\x84\x24\x00\x00\x00\x00", "xx????xxxxxxxxx????xxx????");

	wb::addresses::operation_Execute = FindSignature("\x55\x8B\xEC\x83\xE4\xF0\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x53\x8B\xD9", "xxxxxxx????x????xxx");

	wb::addresses::operation_manager_Execute = FindSignature("\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x53\x56\x57\x8B\xF1\x33\xFF", "xxxxxxxx????xxxxxxx");

	wb::addresses::basic_game_ParseString = FindSignature("\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x56\x89\x4C\x24\x0C", "xx????xxxx????xxxxx");

	wb::addresses::particle_system_manager_Create = FindSignature("\x55\x8B\x6C\x24\x08\x8B\x45\x00\x57", "xxxxxxxxx");

	wb::addresses::DXUtil_Timer = FindSignature("\x83\xEC\x10\x55\x33\xED", "xxxxxx");

	wb::addresses::agent_SetAiBehavior = FindSignature("\x83\xEC\x08\x53\x56\x8B\xF1\x8B\x86\x00\x00\x00\x00\x32\xDB", "xxxxxxxxx????xx");

	wb::addresses::temp_ban_time_dbl_ptr = FindSignature("\x00\x00\x00\x00\x00\x20\xAC", "xxxxxxx");

	wb::addresses::game_data_ptr = FindSignature("\x2E\xBA\x00\x00\x00\x00\x00\x3F\x04\xE2\x7B\x00", "xx????xxxxxx") + 8;

#elif defined WARBAND_DEDICATED
	wb::addresses::operation_Execute_jumptable = FindSignature("\x89\x11\x5B\x8B\xE5\x5D\xC2\x0C\x00\x8B\xFF", "xxxxxxxxxxx") + 11;
	wb::addresses::operation_Execute_entry = FindSignature("\x8B\x13\x52\x68\x00\x00\x00\x00", "xxxx????");
	wb::addresses::operation_Execute_exit_1 = FindSignature("\x83\xBC\x24\x9C\x03\x00\x00\x00", "xxxxxxxx");
	wb::addresses::operation_Execute_exit_2 = wb::addresses::operation_Execute_entry + 8;

	wb::addresses::network_server_ReceiveTeamChatEvent_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00", "xx????xxxxxxxxxxxx????") + 16;
	wb::addresses::network_server_ReceiveTeamChatEvent_exit = wb::addresses::network_server_ReceiveTeamChatEvent_entry + 6;

	wb::addresses::network_server_ReceiveChatEvent_entry = FindSignature("\x5D\x81\xC4\x00\x00\x00\x00\xC2\x08\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00", "xxx????xxxxxxxxxxxxxxxxxxxx????") + 25;
	wb::addresses::network_server_ReceiveChatEvent_exit = wb::addresses::network_server_ReceiveChatEvent_entry + 6;
	
	wb::addresses::ReadModuleFiles_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\x53", "xx????xxxxxxxx");

	wb::addresses::game_ReadModuleFiles_entry = FindSignature("\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x81\xEC\x00\x00\x00\x00\x53\x55", "xx????xxxxxxxxxxxxxx????xx");

	wb::addresses::ParseConsoleCommand_entry = FindSignature("\x68\x00\x00\x00\x00\x8B\xCE\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x83\x7C\x24\x10\x01\x0F\x85\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00", "x????xxx????xxxx????xxxxxxx????xxx????");
	wb::addresses::ParseConsoleCommand_exit_1 = FindSignature("\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x5F\x8B\xC6", "xxxxx????xxx");
	wb::addresses::ParseConsoleCommand_exit_2 = wb::addresses::ParseConsoleCommand_entry + 5;

	wb::addresses::ExecuteConsoleCommand_entry = FindSignature("\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x53\x55\x8B\xAC\x24\x00\x00\x00\x00\x56\x89\x4C\x24\x10", "x????x????xxxxx????xxxxx");

	wb::addresses::CheckUrlReplies_entry = FindSignature("\xE8\x00\x00\x00\x00\x5B\x83\xC4\x1C", "x????xxxx");
	wb::addresses::CheckUrlReplies_exit = wb::addresses::CheckUrlReplies_entry + 5;
	wb::addresses::CheckUrlReplies_call = FindSignature("\x80\x7C\x24\x04\x00\x74\x0A\x89\x4C\x24\x04\xFF\x25\x00\x00\x00\x00\xC2\x04\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x83\x79\x04\xFF", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxx");

	wb::addresses::network_server_ReceiveMessage_entry = FindSignature("\x0F\x87\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\x8D\x54\x24\x10", "xx????xxx????xxxx");
	wb::addresses::network_server_ReceiveMessage_exit_1 = FindSignature("\x5F\x5E\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x14\x00\x8B\xFF", "xxxxxx????xxxxx");
	wb::addresses::network_server_ReceiveMessage_exit_2 = wb::addresses::network_server_ReceiveMessage_entry + 6;

	wb::addresses::agent_ApplyAttackRecord_entry = FindSignature("\x8B\x88\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x69\xC9\x00\x00\x00\x00\x6A\xE4", "xx????xx????xx????xx");
	wb::addresses::agent_ApplyAttackRecord_exit = FindSignature("\x83\x3D\x00\x00\x00\x00\x00\x7C\x15\x7F\x09\x83\x3D\x00\x00\x00\x00\x00\x72\x0A\xDF\x2D\x00\x00\x00\x00\xD9\x5C\x24\x20", "xx?????xxxxxx?????xxxx????xxxx");

	wb::addresses::CreateMbnetHost_entry = FindSignature("\x5F\x66\x89\x4E\x14", "xxxxx");
	wb::addresses::DestroyMbnetHost_entry = FindSignature("\x83\xC4\x08\x5E\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56", "xxxxxxxxxxxxxx");

	wb::addresses::mission_CheckCollision_entry = FindSignature("\x80\x3A\x00\xB9\x00\x00\x00\x00", "xxxx????");
	wb::addresses::mission_CheckCollision_exit = wb::addresses::mission_CheckCollision_entry + 15;

	wb::addresses::agent_SetGroundQuad_entry = FindSignature("\x89\x96\x00\x00\x00\x00\x5F\x5E\x5B\x8B\xE5\x5D\xC2\x24\x00", "xx????xxxxxxxxx");
	wb::addresses::agent_SetGroundQuad_exit = wb::addresses::agent_SetGroundQuad_entry + 6;

	wb::addresses::agent_ReceiveShieldHit_entry = FindSignature("\x8B\xC8\x89\x4C\x24\x14\xDB\x44\x24\x14\xD8\x1D\x00\x00\x00\x00", "xxxxxxxxxxxx????");
	wb::addresses::agent_ReceiveShieldHit_exit = wb::addresses::agent_ReceiveShieldHit_entry + 6;

	wb::addresses::mission_SpawnMissile_entry = FindSignature("\x80\x7C\x24\x13\x00\xD9\x9E\x00\x00\x00\x00", "xxxxxxx????");
	wb::addresses::mission_SpawnMissile_exit = wb::addresses::mission_SpawnMissile_entry + 5;

	wb::addresses::missile_Dive_entry = FindSignature("\x89\x0D\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x89\x1D\x00\x00\x00\x00", "xx????xx????xx????");
	wb::addresses::missile_Dive_exit = FindSignature("\x8B\x8E\x00\x00\x00\x00\x80\xB9\xD2\x00\x00\x00\x00", "xx????xxxxxxx");

	wb::addresses::network_manager_PopulatePlayerInfoServerEvent_entry = FindSignature("\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x84\x24\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x50", "xxx?????x????xxx????x????x????x");
	wb::addresses::network_manager_PopulatePlayerInfoServerEvent_exit = wb::addresses::network_manager_PopulatePlayerInfoServerEvent_entry + 8;

	wb::addresses::network_manager_PopulateServerOptionsServerEvent_entry = FindSignature("\x81\xEC\x00\x00\x00\x00\x8B\x94\x24\x00\x00\x00\x00\xD9\xEE\x56\xDD\x9C\x24\x00\x00\x00\x00\x57\x8B\xBC\x24\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x8D\x4C\x24\x1C", "xx????xxx????xxxxxx????xxxx????x????xxxx");

	wb::addresses::mission_CheckHit_Human_entry = FindSignature("\x8B\x08\x8B\x15\x00\x00\x00\x00\x8B\x52\x20", "xxxx????xxx");
	wb::addresses::mission_CheckHit_Human_exit_1 = FindSignature("\xFF\x44\x24\x44\x83\x7C\x24\x70\x00", "xxxxxxxxx");
	wb::addresses::mission_CheckHit_Human_exit_2 = FindSignature("\x8B\x44\x24\x30\x8B\x54\x24\x20", "xxxxxxxx");

	wb::addresses::mission_CheckHit_Horse_entry = FindSignature("\x8B\xF1\x83\xE6\x0F\x69\xF6\x00\x00\x00\x00\x8B\xC1\xC1\xE8\x04\x8B\x04\x82\xD9\x84\x30\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxxxx????");
	wb::addresses::mission_CheckHit_Horse_exit_1 = FindSignature("\x8B\x75\x18\x8B\x7C\x24\x1C\xC6\x44\x24\x12\x01", "xxxxxxxxxxxx");
	wb::addresses::mission_CheckHit_Horse_exit_2 = FindSignature("\x8B\x75\x18\x8B\x7C\x24\x1C\xC6\x44\x24\x12\x00", "xxxxxxxxxxxx");

	wb::addresses::mission_CheckHit_Prop_entry = FindSignature("\xD9\x44\x24\x44\xDC\x15\x00\x00\x00\x00", "xxxxxx????");
	wb::addresses::mission_CheckHit_Prop_exit_1 = FindSignature("\xD9\x05\x00\x00\x00\x00\x8B\x44\x24\x6C\x33\xC9", "xx????xxxxxx");
	wb::addresses::mission_CheckHit_Prop_exit_2 = FindSignature("\xD9\x44\x24\x14\x51\xDC\x25\x00\x00\x00\x00", "xxxxxxx????");

	wb::addresses::agent_HorseCharged_entry = FindSignature("\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00", "x????xxxx????xx?????xx????xxx????");
	wb::addresses::agent_HorseCharged_exit = wb::addresses::agent_HorseCharged_entry + 5;

	wb::addresses::mission_ApplyBlow_entry = FindSignature("\x51\x8B\x44\x24\x0C\xD9\xEE", "xxxxxxx");
	wb::addresses::mission_ApplyBlow_exit = wb::addresses::mission_ApplyBlow_entry + 5;

	wb::addresses::agent_GetScaleHuman_entry = FindSignature("\xD9\x9E\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x0F\x84\x00\x00\x00\x00\xD9\x86\x00\x00\x00\x00\x8D\x4C\x24\x28", "xx????x????xxxxxxx????xx????xxxx");
	wb::addresses::agent_GetScaleHuman_exit = wb::addresses::agent_GetScaleHuman_entry + 6;
	wb::addresses::agent_GetScaleHorse_entry = FindSignature("\xD9\x9E\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x0F\x84\x00\x00\x00\x00\xD9\x86\x00\x00\x00\x00\x8D\x4C\x24\x38", "xx????x????xxxxxxx????xx????xxxx");
	wb::addresses::agent_GetScaleHorse_exit = wb::addresses::agent_GetScaleHorse_entry + 6;

	wb::addresses::mission_object_Hit_entry = FindSignature("\x81\xC1\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00", "xx????x????xx?????");
	wb::addresses::mission_object_Hit_exit = wb::addresses::mission_object_Hit_entry + 6;

	wb::addresses::agent_CancelSwing_entry = FindSignature("\x8B\x86\x00\x00\x00\x00\x33\xFF\x85\xC0\x7C\x1A", "xx????xxxxxx");
	wb::addresses::agent_CancelSwing_exit = FindSignature("\x85\xFF\x74\x4D\x8B\x07", "xxxxxx");

	wb::addresses::agent_DropItem_entry = FindSignature("\x81\xEC\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00", "xx????xx????");
	wb::addresses::agent_DropItem_exit = wb::addresses::agent_DropItem_entry + 6;

	wb::addresses::agent_StartReloading_entry = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x8B\x55\x08", "xxxx????xxxxxx");

	wb::addresses::agent_EndReloading_entry_1 = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x83\x3D\x00\x00\x00\x00\x00", "xxxx????xxxxx?????");
	wb::addresses::agent_EndReloading_entry_2 = FindSignature("\x5D\x5B\x81\xC4\x00\x00\x00\x00\xC2\x04\x00\x83\xF9\x02", "xxxx????xxxxxx");

	wb::addresses::item_Difficulty_entry = FindSignature("\x8B\x54\x24\x08\x56\x8B\x74\x24\x08", "xxxxxxxxx");

	wb::addresses::mission_object_WeaponKnockBack_entry = FindSignature("\xC6\x44\x24\x1B\x01\x8B\xC1", "xxxxxxx");
	wb::addresses::mission_object_WeaponKnockBack_exit = wb::addresses::mission_object_WeaponKnockBack_entry + 5;

	wb::addresses::item_kind_ShieldNoParry_entry = FindSignature("\x83\x38\x00\x7E\x17", "xxxxx");
	wb::addresses::item_kind_ShieldNoParry_exit_1 = wb::addresses::item_kind_ShieldNoParry_entry + 5;
	wb::addresses::item_kind_ShieldNoParry_exit_2 = FindSignature("\x85\xFF\x7F\x17\x5F", "xxxxx");

	wb::addresses::item_kind_ShieldNoParrySound_entry = FindSignature("\x85\xF6\x0F\x8C\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xC6\x69\xC0\x00\x00\x00\x00\x80\xBC\x08\x90\x0B\x00\x00\x07\x0F\x85\x00\x00\x00\x00\x8B\x45\x10", "xxxx????xx????xxxx????xxxxxxxxxx????xxx");
	wb::addresses::item_kind_ShieldNoParrySound_exit_1 = wb::addresses::item_kind_ShieldNoParrySound_entry + 8;
	wb::addresses::item_kind_ShieldNoParrySound_exit_2 = FindSignature("\x6A\x00\x8D\x8C\x24\x00\x00\x00\x00\x51\x8B\xCF", "xxxxx????xxx");

	wb::addresses::item_kind_ShieldNoParryDamage_entry = FindSignature("\x85\xF6\x0F\x8C\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xC6\x69\xC0\x00\x00\x00\x00\x80\xBC\x08\x90\x0B\x00\x00\x07\x0F\x85\x00\x00\x00\x00\x8B\x4D\x18", "xxxx????xx????xxxx????xxxxxxxxxx????xxx");
	wb::addresses::item_kind_ShieldNoParryDamage_exit_1 = wb::addresses::item_kind_ShieldNoParryDamage_entry + 8;
	wb::addresses::item_kind_ShieldNoParryDamage_exit_2 = FindSignature("\x83\x7C\x24\x68\x00\x75\x0B", "xxxxxxx");

	wb::addresses::item_kind_ShieldNoParryMissiles_entry = FindSignature("\x85\xC0\x0F\x8C\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x69\xC0\x00\x00\x00\x00\x80\xBC\x10\x90\x0B\x00\x00\x07", "xxxx????xx????xx????xxxxxxxx");
	wb::addresses::item_kind_ShieldNoParryMissiles_exit_1 = wb::addresses::item_kind_ShieldNoParryMissiles_entry + 8;
	wb::addresses::item_kind_ShieldNoParryMissiles_exit_2 = FindSignature("\x5F\x5E\x5D\x33\xC0\x5B\x81\xC4\x00\x00\x00\x00\xC2\x20\x00\xDD\xD8", "xxxxxxxx????xxxxx");

	wb::addresses::item_kind_ShieldNoParrySpeed_entry = FindSignature("\x83\x38\x00\x7C\x0F", "xxxxx");
	wb::addresses::item_kind_ShieldNoParrySpeed_exit_1 = wb::addresses::item_kind_ShieldNoParrySpeed_entry + 5;
	wb::addresses::item_kind_ShieldNoParrySpeed_exit_2 = FindSignature("\xD9\x05\x00\x00\x00\x00\x8D\x54\x24\x1C\x52", "xx????xxxxx");

	wb::addresses::item_kind_ShieldNoParryCouchedLance_entry = FindSignature("\x83\x38\x00\x0F\x8C\x00\x00\x00\x00\x6A\x01\x8D\x44\x24\x20", "xxxxx????xxxxxx");
	wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_1 = wb::addresses::item_kind_ShieldNoParryCouchedLance_entry + 9;
	wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_2 = FindSignature("\x5F\x5E\x5D\x33\xC0\x5B\x81\xC4\x00\x00\x00\x00\xC2\x20\x00\xDD\xD8", "xxxxxxxx????xxxxx");

	wb::addresses::item_kind_DisableAgentSoundsHorseShort_entry = FindSignature("\xB8\x00\x00\x00\x00\x8D\x4C\x24\x18\xD9\x5C\x24\x18", "x????xxxxxxxx");
	wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_1 = wb::addresses::item_kind_DisableAgentSoundsHorseShort_entry + 5;
	wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_2 = FindSignature("\x8B\x07\x8B\x14\xC5\x00\x00\x00\x00\x89\x57\x08\x8B\x04\xC5\x00\x00\x00\x00\x89\x47\x0C\xD9\x06", "xxxxx????xxxxxx????xxxxx");

	wb::addresses::agent_BlockedAttack_entry = FindSignature("\x8B\x44\x24\x68\x89\x44\x24\x70", "xxxxxxxx");
	wb::addresses::agent_BlockedAttack_exit = wb::addresses::agent_BlockedAttack_entry + 8;

	wb::addresses::agent_Turn_entry = FindSignature("\xD9\x5C\x24\x18\xD9\x44\x24\x14\xD9\x86\x00\x00\x00\x00", "xxxxxxxxxx????");
	wb::addresses::agent_Turn_exit = wb::addresses::agent_Turn_entry + 8;

	wb::addresses::fixBotsBlocking_entry = FindSignature("\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xF8\xFF", "xx????x????xxx") + 1;

	wb::addresses::operation_Execute = FindSignature("\x55\x8B\xEC\x83\xE4\xF0\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x53\x8B\xD9", "xxxxxxx????x????xxx");

	wb::addresses::operation_manager_Execute = FindSignature("\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x53\x56\x57\x8B\xF1", "xxxxxxxx????xxxxx");

	wb::addresses::basic_game_ParseString = FindSignature("\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x56\x89\x4C\x24\x0C", "xx????xxxx????xxxxx");

	wb::addresses::DXUtil_Timer = FindSignature("\x83\xEC\x10\x55\x33\xED", "xxxxxx");

	wb::addresses::agent_SetAiBehavior = FindSignature("\x83\xEC\x08\x53\x56\x8B\xF1\x8B\x86\x00\x00\x00\x00", "xxxxxxxxx????");

	wb::addresses::temp_ban_time_dbl_ptr = FindSignature("\x00\x00\x00\x00\x00\x20\xAC", "xxxxxxx");

	wb::addresses::game_data_ptr = FindSignature("\x00\x00\x00\x00\x00\x00\x80\x40\x01\x00", "xxxxxxxxxx");
#endif
}