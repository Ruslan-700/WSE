#include "WSELog.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include "WSE.h"

WSELog::WSELog()
{
	m_timestamp = false;
}

WSELog::WSELog(const std::string &path, bool timestamp, bool append)
{
	m_timestamp = timestamp;
	Open(path, timestamp, append);
}

WSELog::~WSELog()
{
	Close();
}

void WSELog::Open(const std::string &path, bool timestamp, bool append)
{
	Close();
	m_timestamp = timestamp;
	m_stream.open(path, append ? std::fstream::app : std::fstream::trunc);
}

void WSELog::Close()
{
	if (m_stream.is_open())
		m_stream.close();
}

void WSELog::Info(const std::string format, ...)
{
	va_list ap;

	va_start(ap, format);
	Log(format, ap);
	va_end(ap);
}

void WSELog::Error(const std::string format, ...)
{
	va_list ap;

	va_start(ap, format);
	Log("ERROR: " + format, ap);
	va_end(ap);
}

void WSELog::Warning(const std::string format, ...)
{
	va_list ap;

	va_start(ap, format);
	Log("WARNING: " + format, ap);
	va_end(ap);
}

void WSELog::Debug(const std::string format, ...)
{
#if defined DEBUG
	va_list ap;

	va_start(ap, format);
	Log("DEBUG: " + format, ap);
	va_end(ap);
#endif
}

void WSELog::Log(const std::string &format, va_list ap)
{
	if (!m_stream.is_open())
		return;

	if (m_timestamp)
	{
		std::ostringstream sstream;
		time_t cur_time_raw;
		tm cur_time;

		time(&cur_time_raw);
		localtime_s(&cur_time, &cur_time_raw);
	
		sstream << std::setw(2) << std::setfill('0') << cur_time.tm_hour << ":";
		sstream << std::setw(2) << std::setfill('0') << cur_time.tm_min << ":";
		sstream << std::setw(2) << std::setfill('0') << cur_time.tm_sec;

		m_stream << "[" << sstream.str() << "] ";
	}

	char message[1024];

	vsprintf_s(message, format.c_str(), ap);
	
	m_stream << std::string(message) << std::endl;
	m_stream.flush();
}
