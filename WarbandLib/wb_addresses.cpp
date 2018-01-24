#include "wb_addresses.h"

using namespace wb;

#if defined WARBAND_VANILLA
//unsigned int addresses::operation_Execute_jumptable = 0x0057A510;
//unsigned int addresses::operation_Execute_entry = 0x0057A47C;
//unsigned int addresses::operation_Execute_exit_1 = 0x0057A4A9;
unsigned int addresses::operation_Execute_jumptable = 0x00535558;
unsigned int addresses::operation_Execute_entry = 0x005354C4;
unsigned int addresses::operation_Execute_exit_1 = 0x005354F1;
unsigned int addresses::operation_Execute_exit_2 = operation_Execute_entry + 8;

//unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x004C3910;
unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x0048BA40;
unsigned int addresses::network_server_ReceiveTeamChatEvent_exit = network_server_ReceiveTeamChatEvent_entry + 6;

//unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004C3BA0;
unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x0048BCD0;
unsigned int addresses::network_server_ReceiveChatEvent_exit = network_server_ReceiveChatEvent_entry + 6;

unsigned int addresses::activate_entry = 0x005A68F2;

//unsigned int addresses::ReadModuleFiles_entry = 0x006669E1;
unsigned int addresses::ReadModuleFiles_entry = 0x00615111;

//unsigned int addresses::game_ReadModuleFiles_entry = 0x006688CE;
unsigned int addresses::game_ReadModuleFiles_entry = 0x0061702E;

unsigned int addresses::ParseConsoleCommand_entry = 0;
unsigned int addresses::ParseConsoleCommand_exit_1 = 0;
unsigned int addresses::ParseConsoleCommand_exit_2 = 0;

unsigned int addresses::ExecuteConsoleCommand_entry = 0;

//unsigned int addresses::Save_entry = 0x00606612;
unsigned int addresses::Save_entry = 0x0054D8F6;

unsigned int addresses::LoadSave_entry = 0x0061747C;
unsigned int addresses::LoadSave_exit = LoadSave_entry + 5;

//unsigned int addresses::CheckUrlReplies_entry = 0x005B293C;
//unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;
unsigned int addresses::CheckUrlReplies_entry = 0x0056F7AE;
unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 5;
unsigned int addresses::CheckUrlReplies_call = 0x00469E10;

//unsigned int addresses::network_client_ReceiveMessage_entry = 0x0049F8A3;
//unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x004A38F9;
unsigned int addresses::network_client_ReceiveMessage_entry = 0x0046E363;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x00472389;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = network_client_ReceiveMessage_entry + 6;

//unsigned int addresses::network_server_ReceiveMessage_entry = 0x004C4B41;
//unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x004C61D8;
unsigned int addresses::network_server_ReceiveMessage_entry = 0x0048CB61;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0048E1FA;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_entry = 0x0048DB22;
unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_exit = network_server_ReceiveMessageServerJoinRequest_entry + 8;

//unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004F8850;
//unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004F8870;
unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004BBF94;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004BBFB4;

//unsigned int addresses::CreateMbnetHost_entry = 0x004CE215;
//unsigned int addresses::DestroyMbnetHost_entry = 0x004CE243;
unsigned int addresses::CreateMbnetHost_entry = 0x004958D5;
unsigned int addresses::DestroyMbnetHost_entry = 0x00495903;

unsigned int addresses::XmlGetServerInfo = 0x0047CF50;
		
//unsigned int addresses::mission_CheckCollision_entry = 0x005206F1;
unsigned int addresses::mission_CheckCollision_entry = 0x004E2E91;
unsigned int addresses::mission_CheckCollision_exit = mission_CheckCollision_entry + 15;
		
//unsigned int addresses::agent_SetGroundQuad_entry = 0x004F5D5E;
unsigned int addresses::agent_SetGroundQuad_entry = 0x004B981E;
unsigned int addresses::agent_SetGroundQuad_exit = agent_SetGroundQuad_entry + 6;

//unsigned int addresses::agent_ReceiveShieldHit_entry = 0x00509A63;
unsigned int addresses::agent_ReceiveShieldHit_entry = 0x004CBE21;
unsigned int addresses::agent_ReceiveShieldHit_exit = agent_ReceiveShieldHit_entry + 6;

//unsigned int addresses::mission_SpawnMissile_entry = 0x0052C26F;
unsigned int addresses::mission_SpawnMissile_entry = 0x004EECC7;
unsigned int addresses::mission_SpawnMissile_exit = mission_SpawnMissile_entry + 5;

//unsigned int addresses::missile_Dive_entry = 0x0051D9D1;
//unsigned int addresses::missile_Dive_exit = 0x0051DA15;
unsigned int addresses::missile_Dive_entry = 0x004DFD91;
unsigned int addresses::missile_Dive_exit = 0x004DFDE1;

//unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x004B43DD;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0047E52D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_exit = network_manager_PopulatePlayerInfoServerEvent_entry + 8;

unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_entry = 0x00482858;
unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_exit = network_manager_PopulatePlayerInfoClientEvent_entry + 8;

//unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x004B40D0;
unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0047E220;

unsigned int addresses::network_manager_GoldNumBits_entry = 0x00814B5C;

//unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0x00405360;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0x00405500;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit = config_manager_ChooseNumberOfEffectiveCorpses_entry + 5;

//unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0x00530D21;
//unsigned int addresses::UpdateHorseAgentEntityBody_exit = UpdateHorseAgentEntityBody_entry + 7;
unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0x004F3599;
unsigned int addresses::UpdateHorseAgentEntityBody_exit = UpdateHorseAgentEntityBody_entry + 6;

//unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0x005F4B86;
unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0x005A4A96;
unsigned int addresses::tactical_window_ShowUseTooltip_exit = tactical_window_ShowUseTooltip_entry + 7;

//unsigned int addresses::tactical_window_ShowCrosshair_entry = 0x005F5E26;
unsigned int addresses::tactical_window_ShowCrosshair_entry = 0x005A5D36;

//unsigned int addresses::mission_CheckHit_Human_entry = 0x0052732D;
//unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x005291CB;
//unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x0052738B;
unsigned int addresses::mission_CheckHit_Human_entry = 0x004EA37D;
unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x004EC2AE;
unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x004EA3DC;

//unsigned int addresses::mission_CheckHit_Horse_entry = 0x00527974;
//unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x005279CB;
//unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x005279C0;
unsigned int addresses::mission_CheckHit_Horse_entry = 0x004EA9C5;
unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x004EAA1F;
unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x004EAA11;

//unsigned int addresses::mission_CheckHit_Prop_entry = 0x00526AEF;
//unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x00526CF7;
//unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x00526B15;
unsigned int addresses::mission_CheckHit_Prop_entry = 0x004E9ABC;
unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x004E9D0D;
unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x004E9AE2;

//unsigned int addresses::agent_HorseCharged_entry = 0x0050627A;
unsigned int addresses::agent_HorseCharged_entry = 0x004C8618;
unsigned int addresses::agent_HorseCharged_exit = agent_HorseCharged_entry + 5;

//unsigned int addresses::mission_ApplyBlow_entry = 0x005241E0;
unsigned int addresses::mission_ApplyBlow_entry = 0x004E7100;
unsigned int addresses::mission_ApplyBlow_exit = mission_ApplyBlow_entry + 5;

//unsigned int addresses::agent_GetScaleHuman_entry = 0x0055B53D;
unsigned int addresses::agent_GetScaleHuman_entry = 0x0051326D;
unsigned int addresses::agent_GetScaleHuman_exit = agent_GetScaleHuman_entry + 6;
//unsigned int addresses::agent_GetScaleHorse_entry = 0x0055B64D;
unsigned int addresses::agent_GetScaleHorse_entry = 0x0051337D;
unsigned int addresses::agent_GetScaleHorse_exit = agent_GetScaleHorse_entry + 6;

//unsigned int addresses::mission_object_Hit_entry = 0x0052977E;
unsigned int addresses::mission_object_Hit_entry = 0x004EC8B3;
unsigned int addresses::mission_object_Hit_exit = mission_object_Hit_entry + 6;

//unsigned int addresses::game_screen_OpenWindow_entry = 0x0058E7A0;
unsigned int addresses::game_screen_OpenWindow_entry = 0x0054AD70;

unsigned int addresses::game_screen_ProfileNumSkins_entry = 0x006122DF;

unsigned int addresses::game_screen_NewProfileLoadSkinList_entry = 0x00612A74;
unsigned int addresses::game_screen_NewProfileLoadSkinList_exit = game_screen_NewProfileLoadSkinList_entry + 5;

unsigned int addresses::game_screen_NewProfileAllocateMemoryForSkins_entry = 0x006128CC;

unsigned int addresses::game_screen_EditProfileLoadSkinList_entry = 0x00612C8F;
unsigned int addresses::game_screen_EditProfileLoadSkinList_exit = game_screen_EditProfileLoadSkinList_entry + 5;

unsigned int addresses::game_screen_EditProfileAllocateMemoryForSkins_entry = 0x00612AB8;

//unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x004D4F10;
unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x0049B150;

//unsigned int addresses::agent_CancelSwing_entry = 0x004F0D1F;
//unsigned int addresses::agent_CancelSwing_exit = 0x004F0D65;
unsigned int addresses::agent_CancelSwing_entry = 0x004B488F;
unsigned int addresses::agent_CancelSwing_exit = 0x004B48D5;

//unsigned int addresses::UpdateAgentEntityBody = 0x00530E58;
unsigned int addresses::UpdateAgentEntityBody = 0x00570280;
		
//unsigned int addresses::agent_DropItem_entry = 0x0050E296;
unsigned int addresses::agent_DropItem_entry = 0x004CF616;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

//unsigned int addresses::agent_StartReloading_entry = 0x0050AC78;
unsigned int addresses::agent_StartReloading_entry = 0x004CD0A6;
unsigned int addresses::agent_StartReloadingClient_entry = 0x004CD07C;
unsigned int addresses::agent_StartReloadingClient_exit = agent_StartReloadingClient_entry + 10;

//unsigned int addresses::agent_EndReloading_entry_1 = 0x0050B83E;
//unsigned int addresses::agent_EndReloading_entry_2 = 0x0050B7DA;
unsigned int addresses::agent_EndReloading_entry_1 = 0x004CDC80;
unsigned int addresses::agent_EndReloading_entry_2 = 0x004CDC25;

unsigned int addresses::item_Difficulty_entry = 0x004DC120;

unsigned int addresses::mission_object_WeaponKnockBack_entry = 0x004EC44F;
unsigned int addresses::mission_object_WeaponKnockBack_exit = mission_object_WeaponKnockBack_entry + 5;

unsigned int addresses::item_kind_ShieldNoParry_entry = 0x004A3EBD;
unsigned int addresses::item_kind_ShieldNoParry_exit_1 = item_kind_ShieldNoParry_entry + 5;
unsigned int addresses::item_kind_ShieldNoParry_exit_2 = 0x004A3ED9;

unsigned int addresses::item_kind_ShieldNoParrySound_entry = 0x004EB75F;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_1 = item_kind_ShieldNoParrySound_entry + 8;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_2 = 0x004EB8A4;

unsigned int addresses::item_kind_ShieldNoParryDamage_entry = 0x004EB411;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_1 = item_kind_ShieldNoParryDamage_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_2 = 0x004EB6E6;

unsigned int addresses::item_kind_ShieldNoParryMissiles_entry = 0x004E74EF;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_1 = item_kind_ShieldNoParryMissiles_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_2 = 0x004E7EF9;

unsigned int addresses::item_kind_ShieldNoParrySpeed_entry = 0x004A7389;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_1 = item_kind_ShieldNoParrySpeed_entry + 5;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_2 = 0x004A739D;

unsigned int addresses::item_kind_ShieldNoParryCarry_entry = 0x004EE450;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_1 = item_kind_ShieldNoParryCarry_entry + 9;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_2 = 0x004EE4DC;

unsigned int addresses::item_kind_ShieldNoParryCouchedLance_entry = 0x004E81A0;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_1 = item_kind_ShieldNoParryCouchedLance_entry + 9;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_2 = 0x004E7EF9;

unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_entry = 0x004CAD08;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_1 = item_kind_DisableAgentSoundsHorseShort_entry + 5;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_2 = 0x004CAD39;

unsigned int addresses::agent_BlockedAttack_entry = 0x004EB9B9;
unsigned int addresses::agent_BlockedAttack_exit = agent_BlockedAttack_entry + 8;

unsigned int addresses::agent_Turn_entry = 0x0049EEEC;
unsigned int addresses::agent_Turn_exit = agent_Turn_entry + 8;

unsigned int addresses::agent_BloodParticles_entry = 0x004A6F23;
unsigned int addresses::agent_BloodParticles_exit = agent_BloodParticles_entry + 59;

unsigned int addresses::fixBotsBlocking_entry = 0;

unsigned int addresses::makeScreenShot = 0x005B3730;

//unsigned int addresses::operation_Execute = 0x0055FF90;
unsigned int addresses::operation_Execute = 0x00517AE0;

//unsigned int addresses::operation_manager_Execute = 0x0057CA40;
unsigned int addresses::operation_manager_Execute = 0x00538060;

unsigned int addresses::operation_manager_StartProfilingBlock_entry = operation_manager_Execute;
unsigned int addresses::operation_manager_StartProfilingBlock_exit = operation_manager_StartProfilingBlock_entry + 6;
unsigned int addresses::operation_manager_StopProfilingBlock = 0x00538A94;

//unsigned int addresses::basic_game_ParseString = 0x00517150;
unsigned int addresses::basic_game_ParseString = 0x004D91B0;

//unsigned int addresses::particle_system_manager_Create = 0x00550EE0;
unsigned int addresses::particle_system_manager_Create = 0x0050A740;

//unsigned int addresses::DXUtil_Timer = 0x0040F660;
unsigned int addresses::DXUtil_Timer = 0x0040E9C0;

//unsigned int addresses::agent_SetAiBehavior = 0x0050EFC0;
unsigned int addresses::agent_SetAiBehavior = 0x004D0340;

unsigned int addresses::temp_ban_time_dbl_ptr = 0x008150E8;

unsigned int addresses::write_to_rgl_log_file_handle_base_var = 0x00868564;
unsigned int addresses::write_to_rgl_log = 0x00636C0E;
unsigned int addresses::write_to_rgl_log_exit = write_to_rgl_log + 5;

//unsigned int addresses::game_data_ptr = 0x0094F118;
unsigned int addresses::game_data_ptr = 0x008681C0;
#elif defined WARBAND_STEAM
unsigned int addresses::operation_Execute_jumptable = 0x0057E410;
unsigned int addresses::operation_Execute_entry = 0x0057E37C;
unsigned int addresses::operation_Execute_exit_1 = 0x0057E3A9;
unsigned int addresses::operation_Execute_exit_2 = operation_Execute_entry + 8;

unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x004C3E00;
unsigned int addresses::network_server_ReceiveTeamChatEvent_exit = network_server_ReceiveTeamChatEvent_entry + 6;

unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004C4090;
unsigned int addresses::network_server_ReceiveChatEvent_exit = network_server_ReceiveChatEvent_entry + 6;

unsigned int addresses::activate_entry = 0;

unsigned int addresses::ReadModuleFiles_entry = 0x0066B511;

unsigned int addresses::game_ReadModuleFiles_entry = 0x0066D3FE;

unsigned int addresses::ParseConsoleCommand_entry = 0;
unsigned int addresses::ParseConsoleCommand_exit_1 = 0;
unsigned int addresses::ParseConsoleCommand_exit_2 = 0;

unsigned int addresses::ExecuteConsoleCommand_entry = 0;

unsigned int addresses::Save_entry = 0x0060AC12;

unsigned int addresses::LoadSave_entry = 0x0061747C;
unsigned int addresses::LoadSave_exit = LoadSave_entry + 5;

unsigned int addresses::CheckUrlReplies_entry = 0x005B712C;
unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;

unsigned int addresses::network_client_ReceiveMessage_entry = 0x0049F8A3;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x004A38F9;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = network_client_ReceiveMessage_entry + 6;

unsigned int addresses::network_server_ReceiveMessage_entry = 0x004C5031;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x004C66E8;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_entry = 0x0048DB22;
unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_exit = network_server_ReceiveMessageServerJoinRequest_entry + 8;

unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004FC270;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004FC290;

unsigned int addresses::CreateMbnetHost_entry = 0x004CE725;
unsigned int addresses::DestroyMbnetHost_entry = 0x004CE753;

unsigned int addresses::XmlGetServerInfo = 0x0047CF50;
		
unsigned int addresses::mission_CheckCollision_entry = 0x00524491;
unsigned int addresses::mission_CheckCollision_exit = mission_CheckCollision_entry + 15;
		
unsigned int addresses::agent_SetGroundQuad_entry = 0x004F977E;
unsigned int addresses::agent_SetGroundQuad_exit = agent_SetGroundQuad_entry + 6;

unsigned int addresses::agent_ReceiveShieldHit_entry = 0x0050D803;
unsigned int addresses::agent_ReceiveShieldHit_exit = agent_ReceiveShieldHit_entry + 6;

unsigned int addresses::mission_SpawnMissile_entry = 0x0053000F;
unsigned int addresses::mission_SpawnMissile_exit = mission_SpawnMissile_entry + 5;

unsigned int addresses::missile_Dive_entry = 0x00521771;
unsigned int addresses::missile_Dive_exit = 0x005217B5;

unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x004B44BD;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_exit = network_manager_PopulatePlayerInfoServerEvent_entry + 8;

unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_entry = 0x00482858;
unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_exit = network_manager_PopulatePlayerInfoClientEvent_entry + 8;

unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x004B41B0;

unsigned int addresses::network_manager_GoldNumBits_entry = 0x00814B5C;

unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0x00405360;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit = config_manager_ChooseNumberOfEffectiveCorpses_entry + 5;

unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0x00534AC1;
unsigned int addresses::UpdateHorseAgentEntityBody_exit = UpdateHorseAgentEntityBody_entry + 7;

unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0x005F9186;
unsigned int addresses::tactical_window_ShowUseTooltip_exit = tactical_window_ShowUseTooltip_entry + 7;

unsigned int addresses::tactical_window_ShowCrosshair_entry = 0x005FA426;

unsigned int addresses::mission_CheckHit_Human_entry = 0x0052B0CD;
unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x0052CF6B;
unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x0052B12B;

unsigned int addresses::mission_CheckHit_Horse_entry = 0x0052B714;
unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x0052B76B;
unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x00524868;

unsigned int addresses::mission_CheckHit_Prop_entry = 0x0052A88F;
unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x0052AA97;
unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x0052A8B5;

unsigned int addresses::agent_HorseCharged_entry = 0x0050A01A;
unsigned int addresses::agent_HorseCharged_exit = agent_HorseCharged_entry + 5;

unsigned int addresses::mission_ApplyBlow_entry = 0x00527F80;
unsigned int addresses::mission_ApplyBlow_exit = mission_ApplyBlow_entry + 5;

unsigned int addresses::agent_GetScaleHuman_entry = 0x0055F2DD;
unsigned int addresses::agent_GetScaleHuman_exit = agent_GetScaleHuman_entry + 6;
unsigned int addresses::agent_GetScaleHorse_entry = 0x0055F3ED;
unsigned int addresses::agent_GetScaleHorse_exit = agent_GetScaleHorse_entry + 6;

unsigned int addresses::mission_object_Hit_entry = 0x0052D51E;
unsigned int addresses::mission_object_Hit_exit = mission_object_Hit_entry + 6;

unsigned int addresses::game_screen_OpenWindow_entry = 0x005926D0;

unsigned int addresses::game_screen_ProfileNumSkins_entry = 0x006122DF;

unsigned int addresses::game_screen_NewProfileLoadSkinList_entry = 0x00612A74;
unsigned int addresses::game_screen_NewProfileLoadSkinList_exit = game_screen_NewProfileLoadSkinList_entry + 5;

unsigned int addresses::game_screen_NewProfileAllocateMemoryForSkins_entry = 0x006128CC;

unsigned int addresses::game_screen_EditProfileLoadSkinList_entry = 0x00612C8F;
unsigned int addresses::game_screen_EditProfileLoadSkinList_exit = game_screen_EditProfileLoadSkinList_entry + 5;

unsigned int addresses::game_screen_EditProfileAllocateMemoryForSkins_entry = 0x00612AB8;

unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x004D88A0;

unsigned int addresses::agent_CancelSwing_entry = 0x004F473F;
unsigned int addresses::agent_CancelSwing_exit = 0x004F4785;

unsigned int addresses::UpdateAgentEntityBody = 0x00534BF8;
		
unsigned int addresses::agent_DropItem_entry = 0x00512036;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

unsigned int addresses::agent_StartReloading_entry = 0x0050EA18;
unsigned int addresses::agent_StartReloadingClient_entry = 0x004CE8AC;
unsigned int addresses::agent_StartReloadingClient_exit = agent_StartReloadingClient_entry + 10;

unsigned int addresses::agent_EndReloading_entry_1 = 0x0050F5DE;
unsigned int addresses::agent_EndReloading_entry_2 = 0x0050F57A;

unsigned int addresses::item_Difficulty_entry = 0x004DD930;

unsigned int addresses::mission_object_WeaponKnockBack_entry = 0x004EDC5F;
unsigned int addresses::mission_object_WeaponKnockBack_exit = mission_object_WeaponKnockBack_entry + 5;

unsigned int addresses::item_kind_ShieldNoParry_entry = 0x004A564D;
unsigned int addresses::item_kind_ShieldNoParry_exit_1 = item_kind_ShieldNoParry_entry + 5;
unsigned int addresses::item_kind_ShieldNoParry_exit_2 = 0x004A5669;

unsigned int addresses::item_kind_ShieldNoParrySound_entry = 0x004ECF6F;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_1 = item_kind_ShieldNoParrySound_entry + 8;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_2 = 0x004ED0B4;

unsigned int addresses::item_kind_ShieldNoParryDamage_entry = 0x004ECC21;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_1 = item_kind_ShieldNoParryDamage_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_2 = 0x004ECEF6;

unsigned int addresses::item_kind_ShieldNoParryMissiles_entry = 0x004E8CFF;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_1 = item_kind_ShieldNoParryMissiles_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_2 = 0x004E9709;

unsigned int addresses::item_kind_ShieldNoParrySpeed_entry = 0x004A8B19;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_1 = item_kind_ShieldNoParrySpeed_entry + 5;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_2 = 0x004A8B2D;

unsigned int addresses::item_kind_ShieldNoParryCarry_entry = 0x004EFC60;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_1 = item_kind_ShieldNoParryCarry_entry + 9;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_2 = 0x004EFCEC;

unsigned int addresses::item_kind_ShieldNoParryCouchedLance_entry = 0x004E99B0;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_1 = item_kind_ShieldNoParryCouchedLance_entry + 9;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_2 = 0x004E9709;

unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_entry = 0x004CC538;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_1 = item_kind_DisableAgentSoundsHorseShort_entry + 5;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_2 = 0x004CC569;

unsigned int addresses::agent_BlockedAttack_entry = 0x004ED1C9;
unsigned int addresses::agent_BlockedAttack_exit = agent_BlockedAttack_entry + 8;

unsigned int addresses::agent_Turn_entry = 0x004A06BC;
unsigned int addresses::agent_Turn_exit = agent_Turn_entry + 8;

unsigned int addresses::agent_BloodParticles_entry = 0x004A6F23;
unsigned int addresses::agent_BloodParticles_exit = agent_BloodParticles_entry + 59;

unsigned int addresses::fixBotsBlocking_entry = 0;

unsigned int addresses::makeScreenShot = 0x005B0A40;

unsigned int addresses::operation_Execute = 0x00563D30;

unsigned int addresses::operation_manager_Execute = 0x00580940;

unsigned int addresses::operation_manager_StartProfilingBlock_entry = operation_manager_Execute;
unsigned int addresses::operation_manager_StartProfilingBlock_exit = operation_manager_StartProfilingBlock_entry + 6;
unsigned int addresses::operation_manager_StopProfilingBlock = 0x00538A94;

unsigned int addresses::basic_game_ParseString = 0x0051AEF0;

unsigned int addresses::particle_system_manager_Create = 0x00554C80;

unsigned int addresses::DXUtil_Timer = 0x0040F660;

unsigned int addresses::agent_SetAiBehavior = 0x00512D60;

unsigned int addresses::game_data_ptr = 0x009463D0;
#elif defined WARBAND_DEDICATED
//unsigned int addresses::operation_Execute_jumptable = 0x004C2FC8;
//unsigned int addresses::operation_Execute_entry = 0x004C2F36;
//unsigned int addresses::operation_Execute_exit_1 = 0x004C2F63;
unsigned int addresses::operation_Execute_jumptable = 0x004C743C;
unsigned int addresses::operation_Execute_entry = 0x004C73A9; //fallback for warbands "opcode out of range"
unsigned int addresses::operation_Execute_exit_1 = 0x004C73D6; //dont continue (got wse operation)
unsigned int addresses::operation_Execute_exit_2 = operation_Execute_entry + 8;

//unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x00438340;
unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x00439680;
unsigned int addresses::network_server_ReceiveTeamChatEvent_exit = network_server_ReceiveTeamChatEvent_entry + 6;

//unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004385A0;
unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004398E0;
unsigned int addresses::network_server_ReceiveChatEvent_exit = network_server_ReceiveChatEvent_entry + 6;

unsigned int addresses::activate_entry = 0;

//unsigned int addresses::ReadModuleFiles_entry = 0x004EDF33;
unsigned int addresses::ReadModuleFiles_entry = 0x004F6993;

//unsigned int addresses::game_ReadModuleFiles_entry = 0x004EF38E;
unsigned int addresses::game_ReadModuleFiles_entry = 0x004F7E4E;

//unsigned int addresses::ParseConsoleCommand_entry = 0x004E1A4F;
//unsigned int addresses::ParseConsoleCommand_exit_1 = 0x004E61B6;
unsigned int addresses::ParseConsoleCommand_entry = 0x004E9EAF;
unsigned int addresses::ParseConsoleCommand_exit_1 = 0x004EE66B;
unsigned int addresses::ParseConsoleCommand_exit_2 = ParseConsoleCommand_entry + 5;

unsigned int addresses::ExecuteConsoleCommand_entry = 0x004E9DF0;

unsigned int addresses::Save_entry = 0;

unsigned int addresses::LoadSave_entry = 0;
unsigned int addresses::LoadSave_exit = 0;

//unsigned int addresses::CheckUrlReplies_entry = 0x004DBEDC;
//unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;
unsigned int addresses::CheckUrlReplies_entry = 0x004E3FBE;
unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 5;
unsigned int addresses::CheckUrlReplies_call = 0x00425330;

unsigned int addresses::network_client_ReceiveMessage_entry = 0;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = 0;

//unsigned int addresses::network_server_ReceiveMessage_entry = 0x00439561;
//unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0043AC51;
unsigned int addresses::network_server_ReceiveMessage_entry = 0x0043A771;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0043BE71;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_entry = 0x0043B732;
unsigned int addresses::network_server_ReceiveMessageServerJoinRequest_exit = network_server_ReceiveMessageServerJoinRequest_entry + 8;

//unsigned int addresses::agent_ApplyAttackRecord_entry = 0x0046D5B5;
//unsigned int addresses::agent_ApplyAttackRecord_exit = 0x0046D5D5;
unsigned int addresses::agent_ApplyAttackRecord_entry = 0x0046F219;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x0046F239;

//unsigned int addresses::CreateMbnetHost_entry = 0x00443835;
//unsigned int addresses::DestroyMbnetHost_entry = 0x00443863;
unsigned int addresses::CreateMbnetHost_entry = 0x004449D5;
unsigned int addresses::DestroyMbnetHost_entry = 0x00444A03;

unsigned int addresses::XmlGetServerInfo = 0x0042D270;
		
//unsigned int addresses::mission_CheckCollision_entry = 0x00489FA1;
unsigned int addresses::mission_CheckCollision_entry = 0x0048D0B1;
unsigned int addresses::mission_CheckCollision_exit = mission_CheckCollision_entry + 15;
		
//unsigned int addresses::agent_SetGroundQuad_entry = 0x0046A12B;
unsigned int addresses::agent_SetGroundQuad_entry = 0x0046BC0B;
unsigned int addresses::agent_SetGroundQuad_exit = agent_SetGroundQuad_entry + 6;

//unsigned int addresses::agent_ReceiveShieldHit_entry = 0x0047DBE3;
unsigned int addresses::agent_ReceiveShieldHit_entry = 0x0047FD61;
unsigned int addresses::agent_ReceiveShieldHit_exit = agent_ReceiveShieldHit_entry + 6;

//unsigned int addresses::mission_SpawnMissile_entry = 0x00492B27;
unsigned int addresses::mission_SpawnMissile_entry = 0x00497320;
unsigned int addresses::mission_SpawnMissile_exit = mission_SpawnMissile_entry + 5;

//unsigned int addresses::missile_Dive_entry = 0x00486F56;
//unsigned int addresses::missile_Dive_exit = 0x00486F96;
unsigned int addresses::missile_Dive_entry = 0x00489EA6;
unsigned int addresses::missile_Dive_exit = 0x00489EF2;

//unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0042D18D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0042E80D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_exit = network_manager_PopulatePlayerInfoServerEvent_entry + 8;

unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_entry = 0;
unsigned int addresses::network_manager_PopulatePlayerInfoClientEvent_exit = 0;

//unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0042CE80;
unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0042E500;

unsigned int addresses::network_manager_GoldNumBits_entry = 0x0069F0F4;

unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit = 0;

unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0;
unsigned int addresses::UpdateHorseAgentEntityBody_exit = 0;

unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0;
unsigned int addresses::tactical_window_ShowUseTooltip_exit = 0;

unsigned int addresses::tactical_window_ShowCrosshair_entry = 0;

//unsigned int addresses::mission_CheckHit_Human_entry = 0x0048FC6D;
//unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x00491AF6;
//unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x0048FCCB;
unsigned int addresses::mission_CheckHit_Human_entry = 0x0049363D;
unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x00495548;
unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x0049369C;

//unsigned int addresses::mission_CheckHit_Horse_entry = 0x004902B7;
//unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x0049030E;
//unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x00490303;
unsigned int addresses::mission_CheckHit_Horse_entry = 0x00493C85;
unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x00493CDF;
unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x00493CD1;

//unsigned int addresses::mission_CheckHit_Prop_entry = 0x0048F438;
//unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x0048F63D;
//unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x0048F45E;
unsigned int addresses::mission_CheckHit_Prop_entry = 0x00492D92;
unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x00492FDA;
unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x00492DB8;

//unsigned int addresses::agent_HorseCharged_entry = 0x0047AEFA;
unsigned int addresses::agent_HorseCharged_entry = 0x0047CD68;
unsigned int addresses::agent_HorseCharged_exit = agent_HorseCharged_entry + 5;

//unsigned int addresses::mission_ApplyBlow_entry = 0x0048CDB0;
unsigned int addresses::mission_ApplyBlow_entry = 0x00490660;
unsigned int addresses::mission_ApplyBlow_exit = mission_ApplyBlow_entry + 5;

//unsigned int addresses::agent_GetScaleHuman_entry = 0x004A53B2;
unsigned int addresses::agent_GetScaleHuman_entry = 0x004A7742;
unsigned int addresses::agent_GetScaleHuman_exit = agent_GetScaleHuman_entry + 6;
//unsigned int addresses::agent_GetScaleHorse_entry = 0x004A54C2;
unsigned int addresses::agent_GetScaleHorse_entry = 0x004A7852;
unsigned int addresses::agent_GetScaleHorse_exit = agent_GetScaleHorse_entry + 6;

//unsigned int addresses::mission_object_Hit_entry = 0x004920A6;
unsigned int addresses::mission_object_Hit_entry = 0x00495B4D;
unsigned int addresses::mission_object_Hit_exit = mission_object_Hit_entry + 6;

unsigned int addresses::game_screen_OpenWindow_entry = 0;

unsigned int addresses::game_screen_ProfileNumSkins_entry = 0;

unsigned int addresses::game_screen_NewProfileLoadSkinList_entry = 0;
unsigned int addresses::game_screen_NewProfileLoadSkinList_exit = 0;

unsigned int addresses::game_screen_NewProfileAllocateMemoryForSkins_entry = 0;

unsigned int addresses::game_screen_EditProfileLoadSkinList_entry = 0;
unsigned int addresses::game_screen_EditProfileLoadSkinList_exit = 0;

unsigned int addresses::game_screen_EditProfileAllocateMemoryForSkins_entry = 0;

unsigned int addresses::item_kind_TransformHoldPosition_entry = 0;

//unsigned int addresses::agent_CancelSwing_entry = 0x0046569F;
//unsigned int addresses::agent_CancelSwing_exit = 0x004656E5;
unsigned int addresses::agent_CancelSwing_entry = 0x0046713F;
unsigned int addresses::agent_CancelSwing_exit = 0x00467185;

unsigned int addresses::UpdateAgentEntityBody = 0;
		
//unsigned int addresses::agent_DropItem_entry = 0x0047E420;
unsigned int addresses::agent_DropItem_entry = 0x004805B0;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

//unsigned int addresses::agent_StartReloading_entry = 0x00465F55;
unsigned int addresses::agent_StartReloading_entry = 0x00467A19;
unsigned int addresses::agent_StartReloadingClient_entry = 0;
unsigned int addresses::agent_StartReloadingClient_exit = 0;

//unsigned int addresses::agent_EndReloading_entry_1 = 0x004669C2;
//unsigned int addresses::agent_EndReloading_entry_2 = 0x00466A03;
unsigned int addresses::agent_EndReloading_entry_1 = 0x00468492;
unsigned int addresses::agent_EndReloading_entry_2 = 0x004684D3;

unsigned int addresses::item_Difficulty_entry = 0x004874D0;

unsigned int addresses::mission_object_WeaponKnockBack_entry = 0x004956E9;
unsigned int addresses::mission_object_WeaponKnockBack_exit = mission_object_WeaponKnockBack_entry + 5;

unsigned int addresses::item_kind_ShieldNoParry_entry = 0x004542FD;
unsigned int addresses::item_kind_ShieldNoParry_exit_1 = item_kind_ShieldNoParry_entry + 5;
unsigned int addresses::item_kind_ShieldNoParry_exit_2 = 0x00454319;

unsigned int addresses::item_kind_ShieldNoParrySound_entry = 0x00494A1F;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_1 = item_kind_ShieldNoParrySound_entry + 8;
unsigned int addresses::item_kind_ShieldNoParrySound_exit_2 = 0x00494B64;

unsigned int addresses::item_kind_ShieldNoParryDamage_entry = 0x004946D1;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_1 = item_kind_ShieldNoParryDamage_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryDamage_exit_2 = 0x004949A6;

unsigned int addresses::item_kind_ShieldNoParryMissiles_entry = 0x00490A4F;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_1 = item_kind_ShieldNoParryMissiles_entry + 8;
unsigned int addresses::item_kind_ShieldNoParryMissiles_exit_2 = 0x00491459;

unsigned int addresses::item_kind_ShieldNoParrySpeed_entry = 0x004574B0;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_1 = item_kind_ShieldNoParrySpeed_entry + 5;
unsigned int addresses::item_kind_ShieldNoParrySpeed_exit_2 = 0x004574C4;

unsigned int addresses::item_kind_ShieldNoParryCarry_entry = 0;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_1 = 0;
unsigned int addresses::item_kind_ShieldNoParryCarry_exit_2 = 0;

unsigned int addresses::item_kind_ShieldNoParryCouchedLance_entry = 0x00491700;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_1 = item_kind_ShieldNoParryCouchedLance_entry + 9;
unsigned int addresses::item_kind_ShieldNoParryCouchedLance_exit_2 = 0x00491459;

unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_entry = 0x00465362;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_1 = item_kind_DisableAgentSoundsHorseShort_entry + 5;
unsigned int addresses::item_kind_DisableAgentSoundsHorseShort_exit_2 = 0x00465393;

unsigned int addresses::agent_BlockedAttack_entry = 0x00494C79;
unsigned int addresses::agent_BlockedAttack_exit = agent_BlockedAttack_entry + 8;

unsigned int addresses::agent_Turn_entry = 0x0044E7D9;
unsigned int addresses::agent_Turn_exit = agent_Turn_entry + 8;

unsigned int addresses::agent_BloodParticles_entry = 0;
unsigned int addresses::agent_BloodParticles_exit = 0;

unsigned int addresses::fixBotsBlocking_entry = 0x00460675;

unsigned int addresses::makeScreenShot = 0;

//unsigned int addresses::operation_Execute = 0x004AD270;
unsigned int addresses::operation_Execute = 0x004AF660;

//unsigned int addresses::operation_manager_Execute = 0x004C54E0;
unsigned int addresses::operation_manager_Execute = 0x004C9F30;

unsigned int addresses::operation_manager_StartProfilingBlock_entry = operation_manager_Execute;
unsigned int addresses::operation_manager_StartProfilingBlock_exit = operation_manager_StartProfilingBlock_entry + 6;
unsigned int addresses::operation_manager_StopProfilingBlock = 0x004CA683;

//unsigned int addresses::basic_game_ParseString = 0x00481800;
unsigned int addresses::basic_game_ParseString = 0x00484170;

unsigned int addresses::particle_system_manager_Create = 0;

unsigned int addresses::DXUtil_Timer = 0x00401040;

//unsigned int addresses::agent_SetAiBehavior = 0x0046F3C0;
unsigned int addresses::agent_SetAiBehavior = 0x00470FF0;

unsigned int addresses::temp_ban_time_dbl_ptr = 0x0069F688;

unsigned int addresses::write_to_rgl_log_file_handle_base_var = 0x006E5F14;
unsigned int addresses::write_to_rgl_log = 0x0050C9BD;
unsigned int addresses::write_to_rgl_log_exit = write_to_rgl_log + 5;

//unsigned int addresses::game_data_ptr = 0x006D8428;
unsigned int addresses::game_data_ptr = 0x006E4430;
#endif
