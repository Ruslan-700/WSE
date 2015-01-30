#pragma once

#include <map>
#include <string>
#include <vector>
#include "WSEBitStream.h"
#include "WSEContext.h"
#include "warband.h"

#define PROFILING_MAGIC 0xDEADCAFE
#define PROFILING_VERSION 1

struct WSEProfilingInfo
{
	__int64 outside;
	LARGE_INTEGER start;
	LARGE_INTEGER rec_start;
	LARGE_INTEGER rec_end;
};

class WSEProfilingContext : public WSEContext
{
public:
	WSEProfilingContext();

protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

public:
	void StartProfilingBlock(wb::operation_manager *manager);
	void StopProfilingBlock();

private:
	void Start();
	void Stop();

private:
	bool m_enabled;
	WSEBitStream m_profile_stream;
	std::map<rgl::string *, int> m_profile_types;
	WSEProfilingInfo m_infos[256];
	int m_cur_info;
	int m_cur_profile_type;
	__int64 m_flush_interval;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_last_flush;
};
