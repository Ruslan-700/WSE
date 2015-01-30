#pragma once

#include "WSEContext.h"
#include "warband.h"

#define WSE_SCRIPT_CHAT_MESSAGE_RECEIVED 0
#define WSE_SCRIPT_CONSOLE_COMMAND_RECEIVED 1
#define WSE_SCRIPT_GAME_SAVED 2
#define WSE_SCRIPT_MULTIPLAYER_MESSAGE_RECEIVED 3
#define WSE_SCRIPT_GET_AGENT_SCALE 4
#define WSE_SCRIPT_WINDOW_OPENED 5
#define WSE_NUM_SCRIPTS 6

class WSEGameContext : public WSEContext
{
protected:
	virtual void OnLoad();

public:
	bool HasScript(int script_no);
	bool ExecuteScript(int script_no, int num_params, int param_1 = 0, int param_2 = 0, int param_3 = 0, int param_4 = 0);

private:
	void OnReadModuleFiles();
	void OnReadGameFiles();
	void OnSave();
	bool OnConsoleCommandReceived(rgl::string *text, rgl::string *result);
	bool OnConfigManagerChooseNumberOfEffectiveCorpses();
	void OnOpenWindow(int window_no);

private:
	int m_mapped_script_nos[WSE_NUM_SCRIPTS];
};
