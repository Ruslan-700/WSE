#include "wb_addresses.h"

using namespace wb;

#if defined WARBAND_VANILLA
//unsigned int addresses::operation_Execute_jumptable = 0x0057A510;
//unsigned int addresses::operation_Execute_entry = 0x0057A47C;
//unsigned int addresses::operation_Execute_exit_1 = 0x0057A4A9;
unsigned int addresses::operation_Execute_jumptable = 0x0053642C;
unsigned int addresses::operation_Execute_entry = 0x00536398;
unsigned int addresses::operation_Execute_exit_1 = 0x005363C5;
unsigned int addresses::operation_Execute_exit_2 = operation_Execute_entry + 8;

//unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x004C3910;
unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x0048BA10;
unsigned int addresses::network_server_ReceiveTeamChatEvent_exit = network_server_ReceiveTeamChatEvent_entry + 6;

//unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004C3BA0;
unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x0048BCA0;
unsigned int addresses::network_server_ReceiveChatEvent_exit = network_server_ReceiveChatEvent_entry + 6;

unsigned int addresses::activate_entry = 0x005A68F2;

//unsigned int addresses::ReadModuleFiles_entry = 0x006669E1;
unsigned int addresses::ReadModuleFiles_entry = 0x00614241;

//unsigned int addresses::game_ReadModuleFiles_entry = 0x006688CE;
unsigned int addresses::game_ReadModuleFiles_entry = 0x0061615E;

unsigned int addresses::ParseConsoleCommand_entry = 0;
unsigned int addresses::ParseConsoleCommand_exit_1 = 0;
unsigned int addresses::ParseConsoleCommand_exit_2 = 0;

//unsigned int addresses::Save_entry = 0x00606612;
unsigned int addresses::Save_entry = 0x0054E276;

//unsigned int addresses::CheckUrlReplies_entry = 0x005B293C;
//unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;
unsigned int addresses::CheckUrlReplies_entry = 0x0056EE6E;
unsigned int addresses::CheckUrlReplies_exit = 0x0056EE73;
unsigned int addresses::CheckUrlReplies_call = 0x00469B50;

//unsigned int addresses::network_client_ReceiveMessage_entry = 0x0049F8A3;
//unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x004A38F9;
unsigned int addresses::network_client_ReceiveMessage_entry = 0x0046E163;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x004721A9;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = network_client_ReceiveMessage_entry + 6;

//unsigned int addresses::network_server_ReceiveMessage_entry = 0x004C4B41;
//unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x004C61D8;
unsigned int addresses::network_server_ReceiveMessage_entry = 0x0048CB31;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0048E1C8;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

//unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004F8850;
//unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004F8870;
unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004BB944;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004BB964;

//unsigned int addresses::CreateMbnetHost_entry = 0x004CE215;
//unsigned int addresses::DestroyMbnetHost_entry = 0x004CE243;
unsigned int addresses::CreateMbnetHost_entry = 0x00495985;
unsigned int addresses::DestroyMbnetHost_entry = 0x004959B3;
		
//unsigned int addresses::mission_CheckCollision_entry = 0x005206F1;
unsigned int addresses::mission_CheckCollision_entry = 0x004E28F1;
unsigned int addresses::mission_CheckCollision_exit = mission_CheckCollision_entry + 15;
		
//unsigned int addresses::agent_SetGroundQuad_entry = 0x004F5D5E;
unsigned int addresses::agent_SetGroundQuad_entry = 0x004B917E;
unsigned int addresses::agent_SetGroundQuad_exit = agent_SetGroundQuad_entry + 6;

//unsigned int addresses::agent_ReceiveShieldHit_entry = 0x00509A63;
unsigned int addresses::agent_ReceiveShieldHit_entry = 0x004CB991;
unsigned int addresses::agent_ReceiveShieldHit_exit = agent_ReceiveShieldHit_entry + 6;

//unsigned int addresses::mission_SpawnMissile_entry = 0x0052C26F;
unsigned int addresses::mission_SpawnMissile_entry = 0x004EE719;
unsigned int addresses::mission_SpawnMissile_exit = mission_SpawnMissile_entry + 5;

//unsigned int addresses::missile_Dive_entry = 0x0051D9D1;
//unsigned int addresses::missile_Dive_exit = 0x0051DA15;
unsigned int addresses::missile_Dive_entry = 0x004DF7F1;
unsigned int addresses::missile_Dive_exit = 0x004DF841;

//unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x004B43DD;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0047E57D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_exit = network_manager_PopulatePlayerInfoServerEvent_entry + 8;

//unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x004B40D0;
unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0047E270;

//unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0x00405360;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry = 0x00405540;
unsigned int addresses::config_manager_ChooseNumberOfEffectiveCorpses_exit = config_manager_ChooseNumberOfEffectiveCorpses_entry + 5;

//unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0x00530D21;
//unsigned int addresses::UpdateHorseAgentEntityBody_exit = UpdateHorseAgentEntityBody_entry + 7;
unsigned int addresses::UpdateHorseAgentEntityBody_entry = 0x004F2F70;
unsigned int addresses::UpdateHorseAgentEntityBody_exit = UpdateHorseAgentEntityBody_entry + 6;

//unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0x005F4B86;
unsigned int addresses::tactical_window_ShowUseTooltip_entry = 0x005A38E6;
unsigned int addresses::tactical_window_ShowUseTooltip_exit = tactical_window_ShowUseTooltip_entry + 7;

//unsigned int addresses::tactical_window_ShowCrosshair_entry = 0x005F5E26;
unsigned int addresses::tactical_window_ShowCrosshair_entry = 0x005A4B86;

//unsigned int addresses::mission_CheckHit_Human_entry = 0x0052732D;
//unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x005291CB;
//unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x0052738B;
unsigned int addresses::mission_CheckHit_Human_entry = 0x004E9DBA;
unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x004EBCF8;
unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x004E9E18;

//unsigned int addresses::mission_CheckHit_Horse_entry = 0x00527974;
//unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x005279CB;
//unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x005279C0;
unsigned int addresses::mission_CheckHit_Horse_entry = 0x004EA401;
unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x004EA458;
unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x004EA44D;

//unsigned int addresses::mission_CheckHit_Prop_entry = 0x00526AEF;
//unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x00526CF7;
//unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x00526B15;
unsigned int addresses::mission_CheckHit_Prop_entry = 0x004E952A;
unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x004E9778;
unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x004E9550;

//unsigned int addresses::agent_HorseCharged_entry = 0x0050627A;
unsigned int addresses::agent_HorseCharged_entry = 0x004C8186;
unsigned int addresses::agent_HorseCharged_exit = agent_HorseCharged_entry + 5;

//unsigned int addresses::mission_ApplyBlow_entry = 0x005241E0;
unsigned int addresses::mission_ApplyBlow_entry = 0x004E6BA0;
unsigned int addresses::mission_ApplyBlow_exit = mission_ApplyBlow_entry + 5;

//unsigned int addresses::agent_GetScaleHuman_entry = 0x0055B53D;
unsigned int addresses::agent_GetScaleHuman_entry = 0x005147AD;
unsigned int addresses::agent_GetScaleHuman_exit = agent_GetScaleHuman_entry + 6;
//unsigned int addresses::agent_GetScaleHorse_entry = 0x0055B64D;
unsigned int addresses::agent_GetScaleHorse_entry = 0x005148BD;
unsigned int addresses::agent_GetScaleHorse_exit = agent_GetScaleHorse_entry + 6;

//unsigned int addresses::mission_object_Hit_entry = 0x0052977E;
unsigned int addresses::mission_object_Hit_entry = 0x004EC30B;
unsigned int addresses::mission_object_Hit_exit = mission_object_Hit_entry + 6;

//unsigned int addresses::game_screen_OpenWindow_entry = 0x0058E7A0;
unsigned int addresses::game_screen_OpenWindow_entry = 0x0054B6F0;

//unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x004D4F10;
unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x0049B1F0;

//unsigned int addresses::agent_CancelSwing_entry = 0x004F0D1F;
//unsigned int addresses::agent_CancelSwing_exit = 0x004F0D65;
unsigned int addresses::agent_CancelSwing_entry = 0x004B41DF;
unsigned int addresses::agent_CancelSwing_exit = 0x004B4225;

//unsigned int addresses::UpdateAgentEntityBody = 0x00530E58;
unsigned int addresses::UpdateAgentEntityBody = 0x0056F930;
		
//unsigned int addresses::agent_DropItem_entry = 0x0050E296;
unsigned int addresses::agent_DropItem_entry = 0x004CF166;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

//unsigned int addresses::agent_StartReloading_entry = 0x0050AC78;
unsigned int addresses::agent_StartReloading_entry = 0x004CCC16;

//unsigned int addresses::agent_EndReloading_entry_1 = 0x0050B83E;
//unsigned int addresses::agent_EndReloading_entry_2 = 0x0050B7DA;
unsigned int addresses::agent_EndReloading_entry_1 = 0x004CD7D4;
unsigned int addresses::agent_EndReloading_entry_2 = 0x004CD779;

//unsigned int addresses::operation_Execute = 0x0055FF90;
unsigned int addresses::operation_Execute = 0x00518F50;

//unsigned int addresses::operation_manager_Execute = 0x0057CA40;
unsigned int addresses::operation_manager_Execute = 0x00538F30;

//unsigned int addresses::basic_game_ParseString = 0x00517150;
unsigned int addresses::basic_game_ParseString = 0x004D8CE0;

//unsigned int addresses::particle_system_manager_Create = 0x00550EE0;
unsigned int addresses::particle_system_manager_Create = 0x0050A730;

//unsigned int addresses::DXUtil_Timer = 0x0040F660;
unsigned int addresses::DXUtil_Timer = 0x0040E670;

//unsigned int addresses::agent_SetAiBehavior = 0x0050EFC0;
unsigned int addresses::agent_SetAiBehavior = 0x004CFE90;

//unsigned int addresses::game_data_ptr = 0x0094F118;
unsigned int addresses::game_data_ptr = 0x00867198;
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

unsigned int addresses::Save_entry = 0x0060AC12;

unsigned int addresses::CheckUrlReplies_entry = 0x005B712C;
unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;

unsigned int addresses::network_client_ReceiveMessage_entry = 0x0049F8A3;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0x004A38F9;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = network_client_ReceiveMessage_entry + 6;

unsigned int addresses::network_server_ReceiveMessage_entry = 0x004C5031;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x004C66E8;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

unsigned int addresses::agent_ApplyAttackRecord_entry = 0x004FC270;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x004FC290;

unsigned int addresses::CreateMbnetHost_entry = 0x004CE725;
unsigned int addresses::DestroyMbnetHost_entry = 0x004CE753;
		
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

unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x004B41B0;

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

unsigned int addresses::item_kind_TransformHoldPosition_entry = 0x004D88A0;

unsigned int addresses::agent_CancelSwing_entry = 0x004F473F;
unsigned int addresses::agent_CancelSwing_exit = 0x004F4785;

unsigned int addresses::UpdateAgentEntityBody = 0x00534BF8;
		
unsigned int addresses::agent_DropItem_entry = 0x00512036;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

unsigned int addresses::agent_StartReloading_entry = 0x0050EA18;

unsigned int addresses::agent_EndReloading_entry_1 = 0x0050F5DE;
unsigned int addresses::agent_EndReloading_entry_2 = 0x0050F57A;

unsigned int addresses::operation_Execute = 0x00563D30;

unsigned int addresses::operation_manager_Execute = 0x00580940;

unsigned int addresses::basic_game_ParseString = 0x0051AEF0;

unsigned int addresses::particle_system_manager_Create = 0x00554C80;

unsigned int addresses::DXUtil_Timer = 0x0040F660;

unsigned int addresses::agent_SetAiBehavior = 0x00512D60;

unsigned int addresses::game_data_ptr = 0x009463D0;
#elif defined WARBAND_DEDICATED
//unsigned int addresses::operation_Execute_jumptable = 0x004C2FC8;
//unsigned int addresses::operation_Execute_entry = 0x004C2F36;
//unsigned int addresses::operation_Execute_exit_1 = 0x004C2F63;
unsigned int addresses::operation_Execute_jumptable = 0x004C5ED8;
unsigned int addresses::operation_Execute_entry = 0x004C5E45;
unsigned int addresses::operation_Execute_exit_1 = 0x004C5E72;
unsigned int addresses::operation_Execute_exit_2 = operation_Execute_entry + 8;

//unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x00438340;
unsigned int addresses::network_server_ReceiveTeamChatEvent_entry = 0x00438F80;
unsigned int addresses::network_server_ReceiveTeamChatEvent_exit = network_server_ReceiveTeamChatEvent_entry + 6;

//unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004385A0;
unsigned int addresses::network_server_ReceiveChatEvent_entry = 0x004391E0;
unsigned int addresses::network_server_ReceiveChatEvent_exit = network_server_ReceiveChatEvent_entry + 6;

unsigned int addresses::activate_entry = 0;

//unsigned int addresses::ReadModuleFiles_entry = 0x004EDF33;
unsigned int addresses::ReadModuleFiles_entry = 0x004F4DC3;

//unsigned int addresses::game_ReadModuleFiles_entry = 0x004EF38E;
unsigned int addresses::game_ReadModuleFiles_entry = 0x004F627E;

//unsigned int addresses::ParseConsoleCommand_entry = 0x004E1A4F;
//unsigned int addresses::ParseConsoleCommand_exit_1 = 0x004E61B6;
unsigned int addresses::ParseConsoleCommand_entry = 0x004E833F;
unsigned int addresses::ParseConsoleCommand_exit_1 = 0x004ECAA6;
unsigned int addresses::ParseConsoleCommand_exit_2 = ParseConsoleCommand_entry + 5;

unsigned int addresses::Save_entry = 0;

//unsigned int addresses::CheckUrlReplies_entry = 0x004DBEDC;
//unsigned int addresses::CheckUrlReplies_exit = CheckUrlReplies_entry + 6;
unsigned int addresses::CheckUrlReplies_entry = 0x004E262E;
unsigned int addresses::CheckUrlReplies_exit = 0x004E2633;
unsigned int addresses::CheckUrlReplies_call = 0x00424AA0;

unsigned int addresses::network_client_ReceiveMessage_entry = 0;
unsigned int addresses::network_client_ReceiveMessage_exit_1 = 0;
unsigned int addresses::network_client_ReceiveMessage_exit_2 = 0;

//unsigned int addresses::network_server_ReceiveMessage_entry = 0x00439561;
//unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0043AC51;
unsigned int addresses::network_server_ReceiveMessage_entry = 0x0043A071;
unsigned int addresses::network_server_ReceiveMessage_exit_1 = 0x0043B761;
unsigned int addresses::network_server_ReceiveMessage_exit_2 = network_server_ReceiveMessage_entry + 6;

//unsigned int addresses::agent_ApplyAttackRecord_entry = 0x0046D5B5;
//unsigned int addresses::agent_ApplyAttackRecord_exit = 0x0046D5D5;
unsigned int addresses::agent_ApplyAttackRecord_entry = 0x0046E419;
unsigned int addresses::agent_ApplyAttackRecord_exit = 0x0046E439;

//unsigned int addresses::CreateMbnetHost_entry = 0x00443835;
//unsigned int addresses::DestroyMbnetHost_entry = 0x00443863;
unsigned int addresses::CreateMbnetHost_entry = 0x00444295;
unsigned int addresses::DestroyMbnetHost_entry = 0x004442C3;
		
//unsigned int addresses::mission_CheckCollision_entry = 0x00489FA1;
unsigned int addresses::mission_CheckCollision_entry = 0x0048C301;
unsigned int addresses::mission_CheckCollision_exit = mission_CheckCollision_entry + 15;
		
//unsigned int addresses::agent_SetGroundQuad_entry = 0x0046A12B;
unsigned int addresses::agent_SetGroundQuad_entry = 0x0046AEDB;
unsigned int addresses::agent_SetGroundQuad_exit = agent_SetGroundQuad_entry + 6;

//unsigned int addresses::agent_ReceiveShieldHit_entry = 0x0047DBE3;
unsigned int addresses::agent_ReceiveShieldHit_entry = 0x0047EF61;
unsigned int addresses::agent_ReceiveShieldHit_exit = agent_ReceiveShieldHit_entry + 6;

//unsigned int addresses::mission_SpawnMissile_entry = 0x00492B27;
unsigned int addresses::mission_SpawnMissile_entry = 0x00496521;
unsigned int addresses::mission_SpawnMissile_exit = mission_SpawnMissile_entry + 5;

//unsigned int addresses::missile_Dive_entry = 0x00486F56;
//unsigned int addresses::missile_Dive_exit = 0x00486F96;
unsigned int addresses::missile_Dive_entry = 0x004890F6;
unsigned int addresses::missile_Dive_exit = 0x00489142;

//unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0042D18D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_entry = 0x0042E00D;
unsigned int addresses::network_manager_PopulatePlayerInfoServerEvent_exit = network_manager_PopulatePlayerInfoServerEvent_entry + 8;

//unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0042CE80;
unsigned int addresses::network_manager_PopulateServerOptionsServerEvent_entry = 0x0042DD00;

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
unsigned int addresses::mission_CheckHit_Human_entry = 0x0049283A;
unsigned int addresses::mission_CheckHit_Human_exit_1 = 0x00494758;
unsigned int addresses::mission_CheckHit_Human_exit_2 = 0x00492898;

//unsigned int addresses::mission_CheckHit_Horse_entry = 0x004902B7;
//unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x0049030E;
//unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x00490303;
unsigned int addresses::mission_CheckHit_Horse_entry = 0x00492E84;
unsigned int addresses::mission_CheckHit_Horse_exit_1 = 0x00492EDB;
unsigned int addresses::mission_CheckHit_Horse_exit_2 = 0x00492ED0;

//unsigned int addresses::mission_CheckHit_Prop_entry = 0x0048F438;
//unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x0048F63D;
//unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x0048F45E;
unsigned int addresses::mission_CheckHit_Prop_entry = 0x00491FB3;
unsigned int addresses::mission_CheckHit_Prop_exit_1 = 0x004921F8;
unsigned int addresses::mission_CheckHit_Prop_exit_2 = 0x00491FD9;

//unsigned int addresses::agent_HorseCharged_entry = 0x0047AEFA;
unsigned int addresses::agent_HorseCharged_entry = 0x0047BF66;
unsigned int addresses::agent_HorseCharged_exit = agent_HorseCharged_entry + 5;

//unsigned int addresses::mission_ApplyBlow_entry = 0x0048CDB0;
unsigned int addresses::mission_ApplyBlow_entry = 0x0048F8B0;
unsigned int addresses::mission_ApplyBlow_exit = mission_ApplyBlow_entry + 5;

//unsigned int addresses::agent_GetScaleHuman_entry = 0x004A53B2;
unsigned int addresses::agent_GetScaleHuman_entry = 0x004A6662;
unsigned int addresses::agent_GetScaleHuman_exit = agent_GetScaleHuman_entry + 6;
//unsigned int addresses::agent_GetScaleHorse_entry = 0x004A54C2;
unsigned int addresses::agent_GetScaleHorse_entry = 0x004A6772;
unsigned int addresses::agent_GetScaleHorse_exit = agent_GetScaleHorse_entry + 6;

//unsigned int addresses::mission_object_Hit_entry = 0x004920A6;
unsigned int addresses::mission_object_Hit_entry = 0x00494D6B;
unsigned int addresses::mission_object_Hit_exit = mission_object_Hit_entry + 6;

unsigned int addresses::game_screen_OpenWindow_entry = 0;

unsigned int addresses::item_kind_TransformHoldPosition_entry = 0;

//unsigned int addresses::agent_CancelSwing_entry = 0x0046569F;
//unsigned int addresses::agent_CancelSwing_exit = 0x004656E5;
unsigned int addresses::agent_CancelSwing_entry = 0x0046642F;
unsigned int addresses::agent_CancelSwing_exit = 0x00466475;

unsigned int addresses::UpdateAgentEntityBody = 0;
		
//unsigned int addresses::agent_DropItem_entry = 0x0047E420;
unsigned int addresses::agent_DropItem_entry = 0x0047F7B0;
unsigned int addresses::agent_DropItem_exit = agent_DropItem_entry + 6;

//unsigned int addresses::agent_StartReloading_entry = 0x00465F55;
unsigned int addresses::agent_StartReloading_entry = 0x00466D09;

//unsigned int addresses::agent_EndReloading_entry_1 = 0x004669C2;
//unsigned int addresses::agent_EndReloading_entry_2 = 0x00466A03;
unsigned int addresses::agent_EndReloading_entry_1 = 0x0046775E;
unsigned int addresses::agent_EndReloading_entry_2 = 0x0046779F;

//unsigned int addresses::operation_Execute = 0x004AD270;
unsigned int addresses::operation_Execute = 0x004AE3C0;

//unsigned int addresses::operation_manager_Execute = 0x004C54E0;
unsigned int addresses::operation_manager_Execute = 0x004C89C0;

//unsigned int addresses::basic_game_ParseString = 0x00481800;
unsigned int addresses::basic_game_ParseString = 0x00483350;

unsigned int addresses::particle_system_manager_Create = 0;

unsigned int addresses::DXUtil_Timer = 0x00401040;

//unsigned int addresses::agent_SetAiBehavior = 0x0046F3C0;
unsigned int addresses::agent_SetAiBehavior = 0x004701F0;

//unsigned int addresses::game_data_ptr = 0x006D8428;
unsigned int addresses::game_data_ptr = 0x006E2420;
#endif
