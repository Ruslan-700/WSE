#pragma once

namespace wb
{
	struct addresses
	{
		static unsigned int operation_Execute_jumptable;
		static unsigned int operation_Execute_entry;
		static unsigned int operation_Execute_exit_1;
		static unsigned int operation_Execute_exit_2;

		static unsigned int network_server_ReceiveTeamChatEvent_entry;
		static unsigned int network_server_ReceiveTeamChatEvent_exit;
	
		static unsigned int network_server_ReceiveChatEvent_entry;
		static unsigned int network_server_ReceiveChatEvent_exit;

		static unsigned int activate_entry;

		static unsigned int ReadModuleFiles_entry;

		static unsigned int game_ReadModuleFiles_entry;
	
		static unsigned int ParseConsoleCommand_entry;
		static unsigned int ParseConsoleCommand_exit_1;
		static unsigned int ParseConsoleCommand_exit_2;

		static unsigned int ExecuteConsoleCommand_entry;
	
		static unsigned int Save_entry;

		static unsigned int LoadSave_entry;
		static unsigned int LoadSave_exit;

		static unsigned int CheckUrlReplies_entry;
		static unsigned int CheckUrlReplies_exit;
		static unsigned int CheckUrlReplies_call;
	
		static unsigned int network_client_ReceiveMessage_entry;
		static unsigned int network_client_ReceiveMessage_exit_1;
		static unsigned int network_client_ReceiveMessage_exit_2;

		static unsigned int network_server_ReceiveMessage_entry;
		static unsigned int network_server_ReceiveMessage_exit_1;
		static unsigned int network_server_ReceiveMessage_exit_2;

		static unsigned int network_server_ReceiveMessageServerJoinRequest_entry;
		static unsigned int network_server_ReceiveMessageServerJoinRequest_exit;
		
		static unsigned int agent_ApplyAttackRecord_entry;
		static unsigned int agent_ApplyAttackRecord_exit;
		
		static unsigned int CreateMbnetHost_entry;
		static unsigned int DestroyMbnetHost_entry;

		static unsigned int XmlGetServerInfo;
		
		static unsigned int mission_CheckCollision_entry;
		static unsigned int mission_CheckCollision_exit;
		
		static unsigned int agent_SetGroundQuad_entry;
		static unsigned int agent_SetGroundQuad_exit;
		
		static unsigned int agent_ReceiveShieldHit_entry;
		static unsigned int agent_ReceiveShieldHit_exit;
		
		static unsigned int mission_SpawnMissile_entry;
		static unsigned int mission_SpawnMissile_exit;
		
		static unsigned int missile_Dive_entry;
		static unsigned int missile_Dive_exit;
		
		static unsigned int network_manager_PopulatePlayerInfoServerEvent_entry;
		static unsigned int network_manager_PopulatePlayerInfoServerEvent_exit;

		static unsigned int network_manager_PopulatePlayerInfoClientEvent_entry;
		static unsigned int network_manager_PopulatePlayerInfoClientEvent_exit;
		
		static unsigned int network_manager_PopulateServerOptionsServerEvent_entry;

		static unsigned int network_manager_GoldNumBits_entry;

		static unsigned int network_manager_ConnectToServer;
		
		static unsigned int config_manager_ChooseNumberOfEffectiveCorpses_entry;
		static unsigned int config_manager_ChooseNumberOfEffectiveCorpses_exit;
		
		static unsigned int UpdateHorseAgentEntityBody_entry;
		static unsigned int UpdateHorseAgentEntityBody_exit;

		static unsigned int tactical_window_ShowUseTooltip_entry;
		static unsigned int tactical_window_ShowUseTooltip_exit;

		static unsigned int tactical_window_ShowCrosshair_entry;
		
		static unsigned int mission_CheckHit_Human_entry;
		static unsigned int mission_CheckHit_Human_exit_1;
		static unsigned int mission_CheckHit_Human_exit_2;

		static unsigned int mission_CheckHit_Horse_entry;
		static unsigned int mission_CheckHit_Horse_exit_1;
		static unsigned int mission_CheckHit_Horse_exit_2;

		static unsigned int mission_CheckHit_Prop_entry;
		static unsigned int mission_CheckHit_Prop_exit_1;
		static unsigned int mission_CheckHit_Prop_exit_2;
		
		static unsigned int agent_HorseCharged_entry;
		static unsigned int agent_HorseCharged_exit;
		
		static unsigned int mission_ApplyBlow_entry;
		static unsigned int mission_ApplyBlow_exit;
		
		static unsigned int agent_GetScaleHuman_entry;
		static unsigned int agent_GetScaleHuman_exit;
		static unsigned int agent_GetScaleHorse_entry;
		static unsigned int agent_GetScaleHorse_exit;

		static unsigned int mission_object_Hit_entry;
		static unsigned int mission_object_Hit_exit;
		
		static unsigned int game_screen_OpenWindow_entry;

		static unsigned int game_screen_ProfileNumSkins_entry;

		static unsigned int game_screen_NewProfileLoadSkinList_entry;
		static unsigned int game_screen_NewProfileLoadSkinList_exit;

		static unsigned int game_screen_NewProfileAllocateMemoryForSkins_entry;

		static unsigned int game_screen_EditProfileLoadSkinList_entry;
		static unsigned int game_screen_EditProfileLoadSkinList_exit;

		static unsigned int game_screen_EditProfileAllocateMemoryForSkins_entry;
		
		static unsigned int item_kind_TransformHoldPosition_entry;
		
		static unsigned int agent_CancelSwing_entry;
		static unsigned int agent_CancelSwing_exit;

		static unsigned int UpdateAgentEntityBody;
		
		static unsigned int agent_DropItem_entry;
		static unsigned int agent_DropItem_exit;
		
		static unsigned int agent_StartReloading_entry;
		static unsigned int agent_StartReloadingClient_entry;
		static unsigned int agent_StartReloadingClient_exit;
		
		static unsigned int agent_EndReloading_entry_1;
		static unsigned int agent_EndReloading_entry_2;

		static unsigned int item_Difficulty_entry;

		static unsigned int mission_object_WeaponKnockBack_entry;
		static unsigned int mission_object_WeaponKnockBack_exit;

		static unsigned int item_kind_ShieldNoParry_entry;
		static unsigned int item_kind_ShieldNoParry_exit_1;
		static unsigned int item_kind_ShieldNoParry_exit_2;

		static unsigned int item_kind_ShieldNoParrySound_entry;
		static unsigned int item_kind_ShieldNoParrySound_exit_1;
		static unsigned int item_kind_ShieldNoParrySound_exit_2;

		static unsigned int item_kind_ShieldNoParryDamage_entry;
		static unsigned int item_kind_ShieldNoParryDamage_exit_1;
		static unsigned int item_kind_ShieldNoParryDamage_exit_2;

		static unsigned int item_kind_ShieldNoParryMissiles_entry;
		static unsigned int item_kind_ShieldNoParryMissiles_exit_1;
		static unsigned int item_kind_ShieldNoParryMissiles_exit_2;

		static unsigned int item_kind_ShieldNoParrySpeed_entry;
		static unsigned int item_kind_ShieldNoParrySpeed_exit_1;
		static unsigned int item_kind_ShieldNoParrySpeed_exit_2;

		static unsigned int item_kind_ShieldNoParryCarry_entry;
		static unsigned int item_kind_ShieldNoParryCarry_exit_1;
		static unsigned int item_kind_ShieldNoParryCarry_exit_2;

		static unsigned int item_kind_ShieldNoParryCouchedLance_entry;
		static unsigned int item_kind_ShieldNoParryCouchedLance_exit_1;
		static unsigned int item_kind_ShieldNoParryCouchedLance_exit_2;

		static unsigned int item_kind_DisableAgentSoundsHorseShort_entry;
		static unsigned int item_kind_DisableAgentSoundsHorseShort_exit_1;
		static unsigned int item_kind_DisableAgentSoundsHorseShort_exit_2;

		static unsigned int agent_BlockedAttack_entry;
		static unsigned int agent_BlockedAttack_exit;

		static unsigned int agent_Turn_entry;
		static unsigned int agent_Turn_exit;

		static unsigned int agent_BloodParticles_entry;
		static unsigned int agent_BloodParticles_exit;

		static unsigned int agent_Initialize;

		static unsigned int agent_SetupSoundSample_entry;

		static unsigned int agent_MakeHandsParallelToGround_entry;

		static unsigned int fixBotsBlocking_entry;

		static unsigned int fixSetCursor_entry;

		static unsigned int makeScreenShot;

		static unsigned int operation_Execute;

		static unsigned int operation_manager_Execute;

		static unsigned int basic_game_ParseString;
		
		static unsigned int particle_system_manager_Create;
		
		static unsigned int DXUtil_Timer;

		static unsigned int agent_SetAiBehavior;

		static unsigned int agent_AddStun;

		static unsigned int agent_PlaySoundAtPosition;
		
		static unsigned int temp_ban_time_dbl_ptr;

		static unsigned int write_to_rgl_log_file_handle_base_var;
		static unsigned int write_to_rgl_log;
		static unsigned int write_to_rgl_log_exit;

		static unsigned int game_data_ptr;
	};
}
