#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "WSEBitStream.h"
#include "WSEContext.h"
#include "warband.h"

#define PROFILING_MAGIC 0xDEADCAFE
#define PROFILING_VERSION 2

class WSEProfilingContext : public WSEContext
{
public:
	WSEProfilingContext();

protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

public:
	void Start();
	void Stop();
	void StartProfilingBlock(wb::operation_manager *manager);
	void StopProfilingBlock(int depth);
	bool IsRecording();
	void AddMarker(const rgl::string& text);

private:
	void IntroduceStr(const rgl::string& id);

private:
	bool m_is_recording;
	WSEBitStream m_profile_stream;
	std::unordered_map<rgl::string, unsigned int> m_str_ids;
	unsigned int m_cur_str_id;
	__int64 m_flush_interval;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_last_flush;
};
