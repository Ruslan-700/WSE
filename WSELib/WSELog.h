#pragma once

#include <cstdarg>
#include <fstream>
#include <string>

class WSELog
{
public:
	WSELog();
	WSELog(const std::string &path, bool timestamp = true, bool append = false);
	~WSELog();
	void Open(const std::string &path, bool timestamp = true, bool append = false);
	void Close();
	virtual void Info(const std::string format, ...);
	virtual void Error(const std::string format, ...);
	virtual void Warning(const std::string format, ...);
	virtual void Debug(const std::string format, ...);

private:
	std::ofstream m_stream;
	bool m_timestamp;
	void Log(const std::string &format, va_list ap);
};
