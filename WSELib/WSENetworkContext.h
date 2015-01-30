#pragma once

#include <deque>
#include "WSEContext.h"
#include "WSEOGPServer.h"
#include "warband.h"

#define MM_EVENT_BITS 4
#define MM_SIZE_BITS 12
#define MM_MESSAGE_BITS 8
#define MM_STRING_LEN 300
#define MM_TRANSF_BITS 12
#define MM_TRANSF_OFF -1.01f
#define MM_TRANSF_MULT 0.0005f
#define MM_TRANSL_GLOBAL_BITS 17
#define MM_TRANSL_GLOBAL_OFF -50.0f
#define MM_TRANSL_GLOBAL_MULT 0.01f
#define MM_TRANSL_LOCAL_BITS 10
#define MM_TRANSL_LOCAL_OFF -10.0f
#define MM_TRANSL_LOCAL_MULT 0.02f
#define MM_FACE_KEY_BITS 64

struct HTTPConnection
{
	rgl::string m_url;
	rgl::string m_user_agent;
	rgl::string m_response;
	int m_success_script_no;
	int m_failure_script_no;
	int m_timeout;
	bool m_failed;
	bool m_raw;
};

enum WSENetworkEventType
{
	MultiplayerMessage,
	PlayerSetSkinNoClientMessage,
	RemoteScriptMessage,
	PlayerStopControllingAgentMessage,
};

DWORD WINAPI HTTPRequestThread(LPVOID param);

class WSENetworkContext : public WSEContext
{
public:
	WSENetworkContext();
	
protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

public:
	bool IsNetworkCompatible() const;
	void ReceiveMultiplayerMessage(wb::network_buffer *nbuf, int player_no, int seq = 1, int cur_seq = 0);
	void PopulateServerOptionsServerEvent(wb::multiplayer_event *evt);
	bool GetHorseFriendlyFire() const;
	void SetHorseFriendlyFire(bool value);
	bool GetShowCrosshair() const;
	void SetShowCrosshair(bool value);
	void SendRemoteScript(wb::script *script, int num_params, int *params);
	void ReceiveRemoteScript(wb::network_buffer *nbuf, int player_no, int seq = 1, int cur_seq = 0);
	void HandleHTTPReplies();

private:
	void BroadcastServerOptions();

private:
	void OnCreateMbnetHost();
	void OnDestroyMbnetHost();
	void OnCheckUrlReplies();
	bool OnClientNetworkMessageReceived(int type, int player_no, wb::network_buffer *nbuf, int seq, int cur_seq);
	bool OnServerNetworkMessageReceived(int type, int player_no, wb::network_buffer *nbuf, int seq, int cur_seq);
	void OnPopulateServerOptionsServerEvent(wb::multiplayer_event *evt);

public:
	wb::network_buffer *m_cur_nbuf;
	std::deque<HTTPConnection *> m_http_connections;
	CRITICAL_SECTION m_http_critical_section;

private:
	WSEOGPServer *m_ogp_server;
	bool m_break_compat;
	bool m_remote_scripting;
	bool m_filter_mods;
	bool m_horse_ff;
	bool m_show_xhair;
};
