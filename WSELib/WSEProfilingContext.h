#pragma once

#include <map>
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

	enum Status{
		stopped     =  0x0,
		recording   =  0x1,
		paused      =  0x2,
		awaitStart  =  0x4,
		awaitStop   =  0x8 | recording,
		awaitPause  = 0x10 | recording,
		awaitResume = 0x20
	};

protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

private:
	void Start();
	void Stop();

public:
	void StartProfilingBlock(int depth, wb::operation_manager *manager);
	void StopProfilingBlock(int depth);
	void SetAwaitStatus(Status s);
	Status GetStatus();

private:
	Status m_status;
	WSEBitStream m_profile_stream;
	std::map<rgl::string *, int> m_profile_types;
	int m_cur_profile_type;
	__int64 m_flush_interval;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_last_flush;
};
