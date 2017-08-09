#pragma once

#include <map>
#include <string>
#include <rgl_matrix.h>
#include "json/json.h"
#include "WSEUtils.h"

#define WSE_DICT_MAGIC 0x57534544
#define WSE_DICT_VERSION 2

class WSEDictionary
{
public:
	void Load(const std::string &file, const int &mode = 0);
	void Load(const WSEDictionary &dict, const int &mode = 0);
	void Save(const std::string &file) const;
	void LoadJson(const std::string &file, const int &mode = 0);
	void SaveJson(const std::string &file) const;
	void FromUrlEncodedJson(const std::string &string, const int &mode = 0);
	const std::string ToUrlEncodedJson() const;
	void Clear();
	void Erase(const std::string &key);
	bool Has(const std::string &key) const;
	int GetSize() const;
	const std::string &GetString(const std::string &key, const std::string &def = "") const;
	int GetInt(const std::string &key, const int &def = 0) const;
	rgl::matrix GetPos(const std::string &key, const rgl::matrix &def) const;
	void SetString(const std::string &key, const std::string &value);
	void SetInt(const std::string &key, const int &value);
	void WSEDictionary::SetPos(const std::string &key, const rgl::matrix pos);
	const std::string &GetKeyByIterator(const int &iterator, const std::string &def = "") const;

public:
	std::map<std::string, std::string> m_values;
};
