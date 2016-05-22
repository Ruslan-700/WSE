#include "WSEDictionary.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

rgl::matrix WSEDictionary::GetPos(const std::string &key, const rgl::matrix &def) const
{
	std::map<std::string, std::string>::const_iterator it = m_values.find(key);
	std::string valStr = it->second;

	if (it == m_values.end())
		return def;

	std::stringstream ss(valStr);
	std::string item;

	float pos[9];
	int i = 0;

	while (std::getline(ss, item, ',')) {
		int valInt = std::stoi(item);
		pos[i] = reinterpret_cast<float &>(valInt);
		i++;
	}
	
	rgl::matrix posMatr;

	posMatr.o = rgl::vector4(pos[0], pos[1], pos[2]);
	posMatr.rot.f = rgl::vector4(pos[3], pos[4], pos[5]);
	posMatr.rot.u = rgl::vector4(pos[6], pos[7], pos[8]);

	posMatr.orthonormalize();

	return posMatr;
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

void WSEDictionary::SetPos(const std::string &key, const rgl::matrix pos)
{
	float posData[9] = {pos.o.x, pos.o.y, pos.o.z, 
		pos.rot.f.x, pos.rot.f.y, pos.rot.f.z,
		pos.rot.u.x, pos.rot.u.y, pos.rot.u.z};

	std::string s = "";
	for (int i = 0; i < 9; i++){
		int val = reinterpret_cast<int &>(posData[i]);
		s += std::to_string(val);

		if (i < 8) 
			s += ',';
	}

	m_values[key] = s;
}

const std::string &WSEDictionary::GetKeyByIterator(const int &iterator, const std::string &def) const
{
	std::map<std::string, std::string>::const_iterator it = m_values.begin();

	for (int i = 0; i < iterator; i++){
		it++;
	}

	if (it == m_values.end())
		return def;

	return it->first;
}