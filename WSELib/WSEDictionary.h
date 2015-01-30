#pragma once

#include <map>
#include <string>

#define WSE_DICT_MAGIC 0x57534544
#define WSE_DICT_VERSION 2

class WSEDictionary
{
public:
	void Load(const std::string &file, const int &mode = 0);
	void Load(const WSEDictionary &dict, const int &mode = 0);
	void Save(const std::string &file) const;
	void Clear();
	bool Has(const std::string &key) const;
	int GetSize() const;
	const std::string &GetString(const std::string &key, const std::string &def = "") const;
	int GetInt(const std::string &key, const int &def = 0) const;
	void SetString(const std::string &key, const std::string &value);
	void SetInt(const std::string &key, const int &value);

public:
	std::map<std::string, std::string> m_values;
};
