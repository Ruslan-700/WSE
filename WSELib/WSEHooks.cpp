#include "WSEHooks.h"

#include "WSE.h"

#define CALL_CONTEXT_FUNC(context, func) \
	_asm	mov ecx, WSE \
	_asm	lea ecx, [ecx]WSE. ## context \
	_asm	call [WSE ## context ## Context:: ## func ##]

void __declspec(naked) OperationManagerExecuteHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+16]
		push [esp+16]
		push [esp+16]
		push [esp+16]
		push ecx
		CALL_CONTEXT_FUNC(Scripting, ExecuteStatementBlock)
		RESTORE_REGS_NO_EAX
		retn 20
	}
}

void __declspec(naked) OperationManagerStartProfilingBlockHook()
{
	_asm
	{
		push ecx
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Profiling, StartProfilingBlock)
		RESTORE_REGS
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF8
		jmp[wb::addresses::operation_manager_StartProfilingBlock_exit]
	}
}

void __declspec(naked) OperationManagerStopProfilingBlockHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Profiling, StopProfilingBlock)
		RESTORE_REGS
		mov esp, ebp
		pop ebp
		retn 20
	}
}

void __declspec(naked) OperationExecuteHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, esp
		mov edx, [ebp+12]
		mov edx, [edx]
		push edx
		push [ebp+8]
		mov edx, eax
#if defined WARBAND
		//add edx, 15
		add edx, 47
		push edx
		mov edx, eax
		add edx, 48
		push edx
		mov edx, eax
		add edx, 376
		push edx
		//push edi
		push ebx
#elif defined WARBAND_DEDICATED
		/*
		add edx, 31
		push edx
		mov edx, eax
		add edx, 40
		push edx
		mov edx, eax
		add edx, 400
		*/
		add edx, 23
		push edx
		mov edx, eax
		add edx, 48
		push edx
		mov edx, eax
		add edx, 376
		push edx
		push ebx
#endif
		CALL_CONTEXT_FUNC(Scripting, OnOperationExecute)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::operation_Execute_exit_1]
continue_exec:
		RESTORE_REGS
#if defined WARBAND_VANILLA
		//mov ecx, [edi]
		mov ecx, [ebx]
		push ecx
		//push 0x008FB428
		push 0x008197D8
#elif defined WARBAND_STEAM
		mov ecx, [edi]
		push ecx
		push 0x008F1AB8
#elif defined WARBAND_DEDICATED
		mov ecx, [ebx]
		push ecx
		//push 0x00697958
		push 0x006A21E0
#endif
		jmp [wb::addresses::operation_Execute_exit_2]
	}
}

void __declspec(naked) LuaOperationJumptableHook() //not lua_operations, the hook is used for lua
{
	_asm
	{
		FREEZE_REGS
			mov eax, esp
			mov edx, [ebp + 12]
			mov edx, [edx]
			push edx
			push[ebp + 8]
			mov edx, eax
#if defined WARBAND
			//add edx, 15
			add edx, 47
			push edx
			mov edx, eax
			add edx, 48
			push edx
			mov edx, eax
			add edx, 376
			push edx
			//push edi
			push ebx
#elif defined WARBAND_DEDICATED
			/*
			add edx, 31
			push edx
			mov edx, eax
			add edx, 40
			push edx
			mov edx, eax
			add edx, 400
			*/
			add edx, 23
			push edx
			mov edx, eax
			add edx, 48
			push edx
			mov edx, eax
			add edx, 376
			push edx
			push ebx
#endif
			CALL_CONTEXT_FUNC(LuaOperations, OnOperationJumptableExecute)
			test eax, eax
			jnz jmp_original
			RESTORE_REGS
			jmp[wb::addresses::operation_Execute_exit_1]
		jmp_original:
			push eax
			RESTORE_REGS
			ret
	}
}

void __declspec(naked) ChatMessageReceivedHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, esp
		sub esp, 848
		push [eax+8]
		mov eax, [eax+4]
		mov eax, [eax]
		push eax
		push 0
		CALL_CONTEXT_FUNC(Mission, OnChatMessageReceived)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		add esp, 848
		retn 8
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::network_server_ReceiveChatEvent_exit]
	}
}

void __declspec(naked) TeamChatMessageReceivedHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, esp
		sub esp, 848
		push [eax+8]
		mov eax, [eax+4]
		mov eax, [eax]
		push eax
		push 1
		CALL_CONTEXT_FUNC(Mission, OnChatMessageReceived)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		add esp, 848
		retn 8
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::network_server_ReceiveTeamChatEvent_exit]
	}
}

void __declspec(naked) ReadModuleFilesHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnReadModuleFiles)
		RESTORE_REGS
#if defined WARBAND_VANILLA || defined WARBAND_STEAM
		add esp, 5684
#elif defined WARBAND_DEDICATED
		add esp, 4088
#endif
		retn
	}
}

void __declspec(naked) GameReadModuleFilesHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnReadGameFiles)
		RESTORE_REGS
		//add esp, 2980
		add esp, 2836
		retn 4
	}
}

void __declspec(naked) ParseConsoleCommandHook()
{
#if defined WARBAND_DEDICATED
	_asm
	{
		FREEZE_REGS
		mov ebx, esp
		push [ebx+600080]
		push [ebx+600084]
		CALL_CONTEXT_FUNC(Game, OnConsoleCommandReceived)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		//push 0x0069DA84
		push 0x006A8C0C
		jmp [wb::addresses::ParseConsoleCommand_exit_2]
continue_exec:
		RESTORE_REGS
		mov esi, [esp+600080]
		jmp [wb::addresses::ParseConsoleCommand_exit_1]
	}
#endif
}

void __declspec(naked) SaveHook()
{
#if defined WARBAND
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnSave)
		RESTORE_REGS
		add esp, 2756
		retn
	}
#endif
}

void __declspec(naked) LoadSaveHook()
{
#if defined WARBAND
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnLoadSave)
		RESTORE_REGS
		push 0x008277FC
		jmp[wb::addresses::LoadSave_exit]
	}
#endif
}

void __declspec(naked) CheckUrlRepliesHook()
{
	_asm
	{
		//call ds:LeaveCriticalSection
		call [wb::addresses::CheckUrlReplies_call]
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Network, OnCheckUrlReplies)
		test eax, eax
		RESTORE_REGS
		jmp [wb::addresses::CheckUrlReplies_exit]
	}
}

void __declspec(naked) ClientNetworkMessageReceivedHook()
{
	_asm
	{
		FREEZE_REGS
		mov edx, esp
		mov ebx, esp
		//mov edx, [ebx+7092]
		mov edx, [ebx + 7040]
		mov edx, [edx]
		push edx
		push edi
		push esi
		//mov ebx, [ebx+7084]
		mov ebx, [ebx + 7032]
		mov ebx, [ebx]
		push ebx
		push eax
		CALL_CONTEXT_FUNC(Network, OnClientNetworkMessageReceived)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::network_client_ReceiveMessage_exit_1]
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::network_client_ReceiveMessage_exit_2]
	}
}

void __declspec(naked) ServerNetworkMessageReceivedHook()
{
	_asm
	{
		FREEZE_REGS
		mov edx, esp
		mov ebx, esp
#if defined WARBAND_VANILLA
		mov edx, [edx+4296]
#elif defined WARBAND_STEAM
		mov edx, [edx+4292]
#elif defined WARBAND_DEDICATED
		mov edx, [edx+4300]
#endif
		mov edx, [edx]
		push edx
		push edi
		push esi
#if defined WARBAND_VANILLA
		mov ebx, [ebx+4288]
#elif defined WARBAND_STEAM
		mov ebx, [ebx+4284]
#elif defined WARBAND_DEDICATED
		mov ebx, [ebx+4292]
#endif
		mov ebx, [ebx]
		push ebx
		push eax
		CALL_CONTEXT_FUNC(Network, OnServerNetworkMessageReceived)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::network_server_ReceiveMessage_exit_1]
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::network_server_ReceiveMessage_exit_2]
	}
}

void __declspec(naked) ServerNetworkMessageReceivedServerJoinRequestHook()
{
	_asm
	{
#if defined WARBAND
		mov dword ptr[esp + 36], 4
#elif defined WARBAND_DEDICATED
		mov dword ptr[esp + 48], 4
#endif
		jmp[wb::addresses::network_server_ReceiveMessageServerJoinRequest_exit]
	}
}

void __declspec(naked) AgentApplyAttackRecHook()
{
	_asm
	{
		FREEZE_REGS
		push edi
		CALL_CONTEXT_FUNC(Mission, OnAgentApplyAttackRec)
		RESTORE_REGS
		jmp [wb::addresses::agent_ApplyAttackRecord_exit]
	}
}

void __declspec(naked) CreateMbnetHostHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Network, OnCreateMbnetHost)
		RESTORE_REGS
		pop edi
		mov [esi+20], cx
		mov eax, esi
		pop esi
		retn
	}
}

void __declspec(naked) DestroyMbnetHostHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Network, OnDestroyMbnetHost)
		RESTORE_REGS
		add esp, 8
		pop esi
		retn
	}
}

void __declspec(naked) MissionCheckCollisionHook()
{
	_asm
	{
		mov ecx, 0x208106
		cmp byte ptr [edx], 0
		jz edx_is_zero
		mov ecx, 0x20810E
edx_is_zero:
		jmp [wb::addresses::mission_CheckCollision_exit]
	}
}

void __declspec(naked) AgentSetGroundQuad()
{
	_asm
	{
		FREEZE_REGS
		//push ecx
		push edx
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentSetGroundQuad)
		RESTORE_REGS
		jmp [wb::addresses::agent_SetGroundQuad_exit]
	}
}

void __declspec(naked) MissionSpawnMissileHook()
{
	_asm
	{
		FREEZE_REGS
#if defined WARBAND_VANILLA || defined WARBAND_STEAM
		push ebp
#elif defined WARBAND_DEDICATED
		push esi
#endif
		CALL_CONTEXT_FUNC(Mission, OnMissionSpawnMissile)
		RESTORE_REGS
		cmp byte ptr [esp+19], 0
		jmp [wb::addresses::mission_SpawnMissile_exit]
	}
}

void __declspec(naked) NetworkManagerPopulatePlayerInfoServerEventHook()
{
	_asm
	{
		mov dword ptr [esp+28], 4
		jmp [wb::addresses::network_manager_PopulatePlayerInfoServerEvent_exit]
	}
}

void __declspec(naked) NetworkManagerPopulatePlayerInfoClientEventHook()
{
	_asm
	{
		mov dword ptr [esp+32], 4
		jmp[wb::addresses::network_manager_PopulatePlayerInfoClientEvent_exit]
	}
}

void __declspec(naked) NetworkManagerPopulateServerOptionsServerEventHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+4]
		CALL_CONTEXT_FUNC(Network, OnPopulateServerOptionsServerEvent)
		RESTORE_REGS
		retn 16
	}
}

void __declspec(naked) ConfigManagerChooseNumberOfEffectiveCorpsesHook()
{
#if defined WARBAND
	__asm
	{
		push esi
		mov esi, ecx
		mov eax, [esi]
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnConfigManagerChooseNumberOfEffectiveCorpses)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		pop esi
		retn
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit]
	}
#endif
}

void __declspec(naked) AgentReceiveShieldHitHook()
{
	_asm
	{
		FREEZE_REGS
		mov ebx, esp
		push [ebx+10308]
		push [ebx+10312]
		push eax
		push [ebx+260]
		push edi
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentShieldHit)
		RESTORE_REGS_NO_EAX
		mov ecx, eax
		mov [esp+12], ecx
		jmp [wb::addresses::agent_ReceiveShieldHit_exit]
	}
}

void __declspec(naked) UpdateHorseAgentEntityBodyHook()
{
	_asm
	{
		FREEZE_REGS
		mov ebx, esp
		push eax
		//push [ebx+480]
		push[ebx + 492]
		//mov ebx, [ebx+496]
		mov ebx, [ebx + 508]
		mov ebx, [ebx]
		push ebx
		CALL_CONTEXT_FUNC(Mission, OnUpdateHorseAgentEntityBody)
		RESTORE_REGS
		//mov ecx, [esp+464]
		mov ecx, [ebp+724]
		jmp [wb::addresses::UpdateHorseAgentEntityBody_exit]
	}
}

void __declspec(naked) TacticalWindowShowUseTooltipHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+20]
		CALL_CONTEXT_FUNC(Mission, OnShowUseTooltip)
		RESTORE_REGS
		mov esi, [esp+144]
		jmp [wb::addresses::tactical_window_ShowUseTooltip_exit]
	}
}

void __declspec(naked) AgentAttackCollidesWithAllyHumanHook()
{
	_asm
	{
		FREEZE_REGS
		//push [esi]
		push[eax]
		CALL_CONTEXT_FUNC(Mission, OnAgentAttackCollidesWithAllyHuman)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Human_exit_1]
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Human_exit_2]
	}
}

void __declspec(naked) AgentAttackCollidesWithAllyHorseHook()
{
	_asm
	{
		FREEZE_REGS
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnAgentAttackCollidesWithAllyHorse)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Horse_exit_1]
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Horse_exit_2]
	}
}

void __declspec(naked) AgentAttackCollidesWithPropHook()
{
	_asm
	{
		FREEZE_REGS
		//mov ecx, [esp+10028]
		mov ecx, [ebp + 8]
		mov ecx, [ecx]
		//push [esp+68]
		push[esp + 52]
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnAgentAttackCollidesWithProp)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Prop_exit_1]
continue_exec:
		RESTORE_REGS
		jmp [wb::addresses::mission_CheckHit_Prop_exit_2]
	}
}

void __declspec(naked) AgentHorseChargedHook()
{
	_asm
	{
		FREEZE_REGS
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnAgentHorseCharged)
		RESTORE_REGS_NO_EAX
		jmp [wb::addresses::agent_HorseCharged_exit]
	}
}

void __declspec(naked) TacticalWindowShowCrosshairHook()
{
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Mission, OnShowCrosshair)
		RESTORE_REGS
		add esp, 168
		retn
	}
}

void __declspec(naked) MissionApplyBlowHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+12]
		CALL_CONTEXT_FUNC(Mission, OnMissionApplyBlow)
		RESTORE_REGS
		push ecx
		mov eax, [esp+12]
		jmp [wb::addresses::mission_ApplyBlow_exit]
	}
}

void __declspec(naked) AgentGetScaleHumanHook()
{
	typedef wb::agent agent;

	_asm
	{
		fstp dword ptr [esi]agent.scale
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentGetScale)
		RESTORE_REGS
		jmp [wb::addresses::agent_GetScaleHuman_exit]
	}
}

void __declspec(naked) AgentGetScaleHorseHook()
{
	typedef wb::agent agent;

	_asm
	{
		fstp dword ptr [esi]agent.scale
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentGetScale)
		RESTORE_REGS
		jmp [wb::addresses::agent_GetScaleHorse_exit]
	}
}

void __declspec(naked) MissionObjectHitHook()
{
	typedef wb::scene_prop scene_prop;

	_asm
	{
		FREEZE_REGS
		mov ebx, esp
		push [ebx+10040]
		push [ebx+10036]
		mov ebx, [ebx+10032]
		push [ebx]
		CALL_CONTEXT_FUNC(Mission, OnMissionObjectHit)
		RESTORE_REGS
		lea ecx, [ecx]scene_prop.simple_triggers
		jmp [wb::addresses::mission_object_Hit_exit]
	}
}

void __declspec(naked) OpenWindowHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+4]
		CALL_CONTEXT_FUNC(Game, OnOpenWindow)
		RESTORE_REGS
		retn 4
	}
}

void __declspec(naked) NewProfileLoadSkinListHook()
{
#if defined WARBAND
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnLoadSkinList)
		RESTORE_REGS
		mov ecx, 0xE0BB58
		jmp[wb::addresses::game_screen_NewProfileLoadSkinList_exit]
	}
#endif
}

void __declspec(naked) EditProfileLoadSkinListHook()
{
#if defined WARBAND
	_asm
	{
		FREEZE_REGS
		CALL_CONTEXT_FUNC(Game, OnLoadSkinList)
		RESTORE_REGS
		mov ecx, 0xE0BB58
		jmp[wb::addresses::game_screen_EditProfileLoadSkinList_exit]
	}
#endif
}

void __declspec(naked) ItemKindTransformHoldPositionHook()
{
	_asm
	{
		FREEZE_REGS
		push [esp+4]
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnItemKindTransformHoldPosition)
		RESTORE_REGS
		mov eax, [esp+4]
		retn 4
	}
}

void __declspec(naked) AgentGetItemForUnbalancedCheckHook()
{
	_asm
	{
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentGetItemForUnbalancedCheck)
		RESTORE_REGS_NO_EAX
		mov edi, eax
		jmp [wb::addresses::agent_CancelSwing_exit]
	}
}

void __declspec(naked) UpdateAgentEntityBodyHook()
{
	_asm
	{
		add esp, 444
		FREEZE_REGS
		mov ebx, esp
		mov eax, [ebx+32]
		push eax
		mov eax, [ebx+24]
		push eax
		mov eax, [ebx+40]
		mov eax, [eax]
		push eax
		mov eax, [ebx+36]
		mov eax, [eax]
		push eax
		mov eax, [ebx+12]
		mov eax, [eax]
		push eax
		mov eax, [ebx+8]
		mov eax, [eax]
		push eax
		mov eax, [ebx+4]
		push eax
		CALL_CONTEXT_FUNC(Mission, OnUpdateAgentEntityBody)
		RESTORE_REGS
		retn
	}
}

void __declspec(naked) AgentDropItemHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, [esp+4]
		push [eax]
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnAgentDropItem)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		retn 8
continue_exec:
		RESTORE_REGS
		sub esp, 924
		jmp [wb::addresses::agent_DropItem_exit]
	}
}

void __declspec(naked) AgentStartReloadingHook()
{
	_asm
	{
		FREEZE_REGS
		push ebp
		CALL_CONTEXT_FUNC(Mission, OnAgentStartReloading)
		RESTORE_REGS
		pop ebp
		pop ebx
#if defined WARBAND
		//add esp, 800
		add esp, 804
#elif defined WARBAND_DEDICATED
		//add esp, 796
		add esp, 800
#endif
		retn 4
	}
}

void __declspec(naked) AgentStartReloadingClientHook()
{
	_asm
	{
		FREEZE_REGS
		push ebp
		CALL_CONTEXT_FUNC(Mission, OnAgentStartReloading)
		RESTORE_REGS
		mov dword ptr[ebp+24060], 5
		jmp[wb::addresses::agent_StartReloadingClient_exit]
	}
}

void __declspec(naked) AgentEndReloadingHook()
{
	_asm
	{
		FREEZE_REGS
		push ebp
		CALL_CONTEXT_FUNC(Mission, OnAgentEndReloading)
		RESTORE_REGS
		pop ebp
		pop ebx
#if defined WARBAND
		//add esp, 800
		add esp, 804
#elif defined WARBAND_DEDICATED
		//add esp, 796
		add esp, 800
#endif
		retn 4
	}
}

void __declspec(naked) MissileDiveHook()
{
	_asm
	{
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnMissileDive)
		RESTORE_REGS
		jmp [wb::addresses::missile_Dive_exit]
	}
}

void __declspec(naked) NewSerialKeyCheckHook()
{
	_asm
	{
		cmp ecx, 1
		jz end
		cmp ecx, 2
		jz end
		cmp ecx, 9
		jz end
		cmp ecx, 10
		jz end
		mov eax, 0x16
end:
		pop ecx
		retn
	}
}

void __declspec(naked) ItemDifficultyCheckHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, esp
		push[eax + 8]
		push[eax + 4]
		push ecx
		CALL_CONTEXT_FUNC(Mission, OnItemDifficultyCheck)
		RESTORE_REGS
		retn 8
	}
}

void _declspec(naked) MissionObjectWeaponKnockBackHook()
{
	_asm
	{
		FREEZE_REGS
		mov eax, [edx + 432]
		imul eax, 240
#if defined WARBAND
		add eax, dword ptr ds:0x008EA5C4
#elif defined WARBAND_DEDICATED
		add eax, dword ptr ds:0x00729BCC
#endif
		push eax
		CALL_CONTEXT_FUNC(Mission, OnMissionObjectWeaponKnockBack)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		mov[esp + 27], 1
		jmp[wb::addresses::mission_object_WeaponKnockBack_exit]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::mission_object_WeaponKnockBack_exit]
	}
}

void __declspec(naked) ItemKindShieldNoParryHook()
{
	_asm
	{
		FREEZE_REGS
		push dword ptr [eax]
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParry_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParry_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParrySoundHook()
{
	_asm
	{
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParrySound_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParrySound_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParryDamageHook()
{
	_asm
	{
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryDamage_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryDamage_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParryMissilesHook()
{
	_asm
	{
		FREEZE_REGS
		push eax
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryMissiles_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryMissiles_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParrySpeedHook()
{
	_asm
	{
		FREEZE_REGS
		push dword ptr [eax]
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParrySpeed_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParrySpeed_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParryCarryHook()
{
	_asm
	{
		FREEZE_REGS
		push edi
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParryCarry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryCarry_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryCarry_exit_2]
	}
}

void __declspec(naked) ItemKindShieldNoParryCouchedLanceHook()
{
	_asm
	{
		FREEZE_REGS
		push dword ptr[eax]
		CALL_CONTEXT_FUNC(Mission, OnItemKindShieldNoParry)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_ShieldNoParryCouchedLance_exit_2]
	}
}

void __declspec(naked) ItemKindDisableAgentSoundsHorseShortHook()
{
	_asm
	{
		FREEZE_REGS
		push [ebx + 668]
		CALL_CONTEXT_FUNC(Mission, OnItemKindDisableAgentSounds)
		test al, al
		jnz continue_exec
		RESTORE_REGS
		mov eax, 108
		jmp[wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_1]
	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::item_kind_DisableAgentSoundsHorseShort_exit_2]
	}
}

void __declspec(naked) AgentBlockedAttackHook()
{
	_asm
	{
		FREEZE_REGS
		push[esp + 28]
		mov eax, [ebp + 16]
		push eax
		mov eax, [ebp + 12]
		push[eax]
		mov edi, [ebp + 8]
		push[edi]
		CALL_CONTEXT_FUNC(Mission, OnAgentBlockedAttack)
		RESTORE_REGS
		mov eax, [esp+104]
#if defined WARBAND
		mov [esp+116], eax
#elif defined WARBAND_DEDICATED
		mov [esp+112], eax
#endif
		jmp[wb::addresses::agent_BlockedAttack_exit]
	}
}

void __declspec(naked) AgentTurnHook()
{
	_asm
	{
		FREEZE_REGS
		mov edx, esp
		add edx, 20
		push edx
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentTurn)
		RESTORE_REGS
		fstp[esp + 24]
		fld[esp + 20]
		jmp[wb::addresses::agent_Turn_exit]
	}
}

void __declspec(naked) AgentBloodParticlesHook()
{
#if defined WARBAND
	_asm
	{
		FREEZE_REGS
		push esi
		CALL_CONTEXT_FUNC(Mission, OnAgentGetBloodParticles)
		RESTORE_REGS
		mov edi, ds:0x00AA13FC
		mov esi, ds:0x00AA1400
		jmp[wb::addresses::agent_BloodParticles_exit]
	}
#endif
}

void __declspec(naked) RglLogHook()
{
	_asm
	{
		mov eax, [esi]
		push[edi + eax]

		FREEZE_REGS
		mov eax, [wb::addresses::write_to_rgl_log_file_handle_base_var]
		mov eax, [eax]
		mov eax, [eax + 0x10]

		cmp eax, [ebp+8]
		jne continue_exec

		push [esp+8]
		push [esp+8]
		push [esp+8]
		CALL_CONTEXT_FUNC(Game, OnRglLogWrite)

	continue_exec:
		RESTORE_REGS
		jmp[wb::addresses::write_to_rgl_log_exit]
	}
}

void __declspec(naked) XmlGetServerInfoHook()
{
	_asm
	{
		FREEZE_REGS
		push[esp + 4]
		CALL_CONTEXT_FUNC(Network, XmlGetServerInfo)
		RESTORE_REGS
		retn 4
	}
}