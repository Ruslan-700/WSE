#include "WSEDictionary.h"

#include <iostream>
#include <fstream>

void WSEDictionary::Load(const std::string &file, const int &mode)
{
	if (mode == 0)
		m_values.clear();

	std::ifstream stream(file, std::ios::binary);

	if (!stream.is_open())
		return;

	int magic, version, count;
	
	stream.read((char *)&magic, sizeof(int));

	if (magic != WSE_DICT_MAGIC)
		return;

	stream.read((char *)&version, sizeof(int));

	if (version > WSE_DICT_VERSION)
		return;

	stream.read((char *)&count, sizeof(int));
	
	for (int i = 0; i < count; ++i)
	{
		int key_len;
		int value_len;

		stream.read((char *)&key_len, sizeof(int));
		stream.read((char *)&value_len, sizeof(int));

		char *key_buf = new char[key_len + 1];
		char *value_buf = new char[value_len + 1];
				
		stream.read(key_buf, key_len);
		stream.read(value_buf, value_len);

		key_buf[key_len] = '\0';
		value_buf[value_len] = '\0';
				
		if (mode != 2 || m_values.find(key_buf) == m_values.end())
			m_values[key_buf] = value_buf;

		delete[] key_buf;
		delete[] value_buf;
	}
}

void WSEDictionary::Load(const WSEDictionary &dict, const int &mode)
{
	if (mode == 0)
		m_values.clear();

	for (std::map<std::string, std::string>::const_iterator it = dict.m_values.begin(); it != dict.m_values.end(); ++it)
	{
		if (mode == 2 && m_values.find(it->first) != m_values.end())
			continue;

		m_values[it->first] = it->second;
	}
}

void WSEDictionary::Save(const std::string &file) const
{
	std::ofstream stream(file, std::ios::binary);

	if (!stream.is_open())
		return;
	
	int magic = WSE_DICT_MAGIC;
	int version = WSE_DICT_VERSION;
	int size = m_values.size();
	
	stream.write((char *)&magic, sizeof(int));
	stream.write((char *)&version, sizeof(int));
	stream.write((char *)&size, sizeof(int));
	
	for (std::map<std::string, std::string>::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
	{
		int key_len = it->first.length();
		int value_len = it->second.length();
		
		stream.write((char *)&key_len, sizeof(int));
		stream.write((char *)&value_len, sizeof(int));
		stream.write(it->first.c_str(), key_len);
		stream.write(it->second.c_str(), value_len);
	}

	stream.close();
}

void WSEDictionary::Clear()
{
	m_values.clear();
}

bool WSEDictionary::Has(const std::string &key) const
{
	return m_values.find(key) != m_values.end();
}

int WSEDictionary::GetSize() const
{
	return m_values.size();
}

const std::string &WSEDictionary::GetString(const std::string &key, const std::string &def) const
{
	std::map<std::string, std::string>::const_iterator it = m_values.find(key);

	if (it == m_values.end())
		return def;

	return it->second;
}

int WSEDictionary::GetInt(const std::string &key, const int &def) const
{
	std::map<std::string, std::string>::const_iterator it = m_values.find(key);

	if (it == m_values.end())
		return def;

	return strtol(it->second.c_str(), nullptr, 0);
}

void WSEDictionary::SetString(const std::string &key, const std::string &value)
{
	m_values[key] = value;
}

void WSEDictionary::SetInt(const std::string &key, const int &value)
{
	char buf[16];

	sprintf_s(buf, "%d", value);
	m_values[key] = buf;
}
