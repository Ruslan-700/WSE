#include "WSEIniFile.h"

#include <fstream>
#include "WSE.h"
	
WSEIniFile::WSEIniFile()
{
}

WSEIniFile::WSEIniFile(const std::string &path)
{
	Open(path);
}
	
WSEIniFile::~WSEIniFile()
{
	for (std::map<std::string, WSEIniFileSectionData>::const_iterator it = m_sections.begin(); it != m_sections.end(); ++it)
	{
		delete it->second.m_section;
	}

	m_sections.clear();
}

void WSEIniFile::Open(const std::string &path, bool clear)
{
	std::ifstream stream(path);
	std::string section = "";

	if (clear)
		m_settings.clear();
	
	while (stream)
	{
		std::string line;

		std::getline(stream, line);

		trim(line);

		if (line == "" || line[0] == '#' || line[0] == '/' || line[0] == ';')
			continue;

		int index = line.find('=');

		if (index == -1)
		{
			if (line.length() >= 2 && line[0] == '[' && line[line.length() - 1] == ']')
				section = line.substr(1, line.length() - 2);

			continue;
		}

		std::string key = spaces_to_underscores(trim(line.substr(0, index)));
		std::string value = trim(line.substr(index + 1));

		m_settings[section][key] = value;
	}
}

void WSEIniFile::Close()
{
	m_settings.clear();
}

WSEIniFileSection *WSEIniFile::GetSection(const std::string &name)
{
	if (m_sections.find(name) == m_sections.end())
	{
		m_sections[name].m_num_uses = 0;
		m_sections[name].m_section = new WSEIniFileSection(this, name);
	}

	m_sections[name].m_num_uses++;
	return m_sections[name].m_section;
}

void WSEIniFile::ReleaseSection(const std::string &name)
{
	if (m_sections.find(name) == m_sections.end())
		return;

	m_sections[name].m_num_uses--;

	if (!m_sections[name].m_num_uses)
	{
		delete m_sections[name].m_section;
		m_sections.erase(name);
	}
}

bool WSEIniFile::HasKey(const std::string &section, const std::string &key)
{
	return String(section, key) != "";
}

int WSEIniFile::Int(const std::string &section, const std::string &key, int def)
{
	std::string value = String(section, key);

	if (value.empty())
		return def;

	return strtol(value.c_str(), nullptr, 0);
}

float WSEIniFile::Float(const std::string &section, const std::string &key, float def)
{
	return (float)Double(section, key, def);
}

double WSEIniFile::Double(const std::string &section, const std::string &key, double def)
{
	std::string value = String(section, key);

	if (value.empty())
		return def;

	return atof(value.c_str());
}

bool WSEIniFile::Bool(const std::string &section, const std::string &key, bool def)
{
	return Int(section, key, def) != 0;
}

const std::string &WSEIniFile::String(const std::string &section, const std::string &key, const std::string &def)
{
	if (m_settings.find(section) == m_settings.end() || m_settings[section].find(key) == m_settings[section].end() || m_settings[section][key].empty())
		return def;

	return m_settings[section][key];
}

WSEIniFileSection::WSEIniFileSection(WSEIniFile *parent, const std::string &section)
{
	m_parent = parent;
	m_section = section;
}

void WSEIniFileSection::Release()
{
	m_parent->ReleaseSection(std::string(m_section));
}

bool WSEIniFileSection::HasKey(const std::string &key)
{
	return String(key) != "";
}

int WSEIniFileSection::Int(const std::string &key, int def)
{
	return m_parent->Int(m_section, key, def);
}

float WSEIniFileSection::Float(const std::string &key, float def)
{
	return m_parent->Float(m_section, key, def);
}

double WSEIniFileSection::Double(const std::string &key, double def)
{
	return m_parent->Double(m_section, key, def);
}

bool WSEIniFileSection::Bool(const std::string &key, bool def)
{
	return m_parent->Bool(m_section, key, def);
}

const std::string &WSEIniFileSection::String(const std::string &key, const std::string &def)
{
	return m_parent->String(m_section, key, def);
}
