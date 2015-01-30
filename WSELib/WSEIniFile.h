#pragma once

#include <map>
#include <string>

class WSEIniFileSection;

struct WSEIniFileSectionData
{
	int m_num_uses;
	WSEIniFileSection *m_section;
};

class WSEIniFile
{
public:
	WSEIniFile();
	WSEIniFile(const std::string &path);
	~WSEIniFile();
	virtual void Open(const std::string &path, bool clear = true);
	virtual void Close();
	virtual WSEIniFileSection *GetSection(const std::string &name);
	virtual void ReleaseSection(const std::string &name);
	virtual bool HasKey(const std::string &section, const std::string &key);
	virtual int Int(const std::string &section, const std::string &key, int def = 0);
	virtual float Float(const std::string &section, const std::string &key, float def = 0.0f);
	virtual double Double(const std::string &section, const std::string &key, double def = 0.0);
	virtual bool Bool(const std::string &section, const std::string &key, bool def = false);
	virtual const std::string &String(const std::string &section, const std::string &key, const std::string &def = "");

private:
	std::map<std::string, std::map<std::string, std::string> > m_settings;
	std::map<std::string, WSEIniFileSectionData> m_sections;
};

class WSEIniFileSection
{
public:
	WSEIniFileSection(WSEIniFile *parent, const std::string &section);
	virtual void Release();
	virtual bool HasKey(const std::string &key);
	virtual int Int(const std::string &key, int def = 0);
	virtual float Float(const std::string &key, float def = 0.0f);
	virtual double Double(const std::string &key, double def = 0.0);
	virtual bool Bool(const std::string &key, bool def = false);
	virtual const std::string &String(const std::string &key, const std::string &def = "");

private:
	WSEIniFile *m_parent;
	std::string m_section;
};
