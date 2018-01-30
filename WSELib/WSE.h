#pragma once

#include <list>
#include <string>

#include "WSEArray.h"
#include "WSEIniFile.h"
#include "WSELog.h"
#include "WSEHookManager.h"
#include "WSEObjectManager.h"

#include "WSEHooks.h"
#include "WSEUtils.h"

#include "WSEPluginWrapperContext.h"

#include "WSESignatureScannerContext.h"
#include "WSEDebugContext.h"
#include "WSEPerformanceContext.h"
#include "WSEGameContext.h"
#include "WSEMissionContext.h"
#include "WSENetworkContext.h"
#include "WSEScriptingContext.h"
#include "WSEProfilingContext.h"
#include "WSEScreenContext.h"

#include "WSEAgentOperationsContext.h"
#include "WSECoreOperationsContext.h"
#include "WSEDebugOperationsContext.h"
#include "WSEDictionaryOperationsContext.h"
#include "WSEEditModeOperationsContext.h"
#include "WSEEngineOperationsContext.h"
#include "WSEFloatingPointOperationsContext.h"
#include "WSEInputOperationsContext.h"
#include "WSEItemOperationsContext.h"
#include "WSEMathOperationsContext.h"
#include "WSEMenuOperationsContext.h"
#include "WSEMissionOperationsContext.h"
#include "WSEMultiplayerOperationsContext.h"
#include "WSENetworkOperationsContext.h"
#include "WSEOptionsOperationsContext.h"
#include "WSEPartyOperationsContext.h"
#include "WSEPlayerOperationsContext.h"
#include "WSEPositionOperationsContext.h"
#include "WSEPresentationOperationsContext.h"
#include "WSESiteOperationsContext.h"
#include "WSESkinOperationsContext.h"
#include "WSEStringOperationsContext.h"
#include "WSETroopOperationsContext.h"
#include "WSEArrayOperationsContext.h"
#include "WSELuaOperationsContext.h"

#define VAL(X) #X
#define TOSTR(X) VAL(X)
#define WSE_VERSION TOSTR(WSE_VERSION_MAJOR) ## "." ## TOSTR(WSE_VERSION_MINOR) ## "." ## TOSTR(WSE_VERSION_BUILD)
#define WSE_VERSION_MAJOR 4
#define WSE_VERSION_MINOR 6
#define WSE_VERSION_BUILD 0
#define WSE_NETCODE_VERSION 9

#if defined WARBAND_VANILLA
#define WSE_TYPE 0
#elif defined WARBAND_STEAM
#define WSE_TYPE 1
#elif defined WARBAND_DEDICATED
#define WSE_TYPE 2
#endif

#define RAD2DEG(rad) (rad) * 57.2957795f
#define DEG2RAD(deg) (deg) * 0.0174532925f

void DumpStackTrace(LPEXCEPTION_POINTERS ptrs);

struct WSEPluginInfo;

class WSEEnvironment
{
public:
	WSEIniFile SettingsIni;
	WSEIniFile ModuleSettingsIni;
	WSEIniFile GameVariablesIni;
	WSELog Log;
	WSEHookManager Hooks;
	WSEObjectManager Objects;
	
#if defined DEBUG
	WSESignatureScannerContext SignatureScanner;
	WSEDebugContext Debug;
#endif
	WSEPerformanceContext Performance;
	WSEGameContext Game;
	WSEProfilingContext Profiling;
	WSEScreenContext Screen;
	WSENetworkContext Network;
	WSEMissionContext Mission;
	WSEScriptingContext Scripting;

	WSEAgentOperationsContext AgentOperations;
	WSECoreOperationsContext CoreOperations;
#if defined DEBUG
	WSEDebugOperationsContext DebugOperations;
#endif
	WSEDictionaryOperationsContext DictionaryOperations;
	WSEEditModeOperationsContext EditModeOperations;
	WSEEngineOperationsContext EngineOperations;
	WSEFloatingPointOperationsContext FloatingPointOperations;
	WSEInputOperationsContext InputOperations;
	WSEItemOperationsContext ItemOperations;
	WSEMathOperationsContext MathOperations;
	WSEMenuOperationsContext MenuOperations;
	WSEMissionOperationsContext MissionOperations;
	WSEMultiplayerOperationsContext MultiplayerOperations;
	WSENetworkOperationsContext NetworkOperations;
	WSEOptionsOperationsContext OptionsOperations;
	WSEPartyOperationsContext PartyOperations;
	WSEPlayerOperationsContext PlayerOperations;
	WSEPositionOperationsContext PositionOperations;
	WSEPresentationOperationsContext PresentationOperations;
	WSESiteOperationsContext SiteOperations;
	WSESkinOperationsContext SkinOperations;
	WSEStringOperationsContext StringOperations;
	WSETroopOperationsContext TroopOperations;
	WSEArrayOperationsContext ArrayOperations;
	WSELuaOperationsContext LuaOperations;
	
	const std::string &GetPath() const;
	void Load(HINSTANCE module_handle);
	void Unload();
	void AddContext(WSEContext *context, bool load);
	void SendContextEvent(WSEContext *sender, WSEEvent evt, void *data = NULL);

private:
	HINSTANCE m_instance;
	std::string m_path;
	std::list<WSEContext *> m_contexts;
};

extern __declspec(dllexport) WSEEnvironment *WSE;
