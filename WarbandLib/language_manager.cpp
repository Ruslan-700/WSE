#include "language_manager.h"

using namespace wb;

int language_manager::get_hash(const rgl::string &str) const
{
	int hash = 0;

	for (int i = 0; i < str.length(); ++i)
	{
		hash = (str[i] + 59 * hash) % 3571;
	}

	return hash;
}

const rgl::string &language_manager::translate(const rgl::string &str)
{
	const rgl::string *translated = this->try_translate(str);

	if (!translated)
	{
		this->not_found_text = rgl::string("STR NOT FOUND: ") + str;
		return this->not_found_text;
	}

	return *translated;
}

const rgl::string *language_manager::try_translate(const rgl::string &str)
{
	int hash = this->get_hash(str);
	stl::vector<rgl::string> *key_vec = &this->keys[hash];

	for (int i = 0; i < key_vec->size(); ++i)
	{
		if ((*key_vec)[i] == str)
			return &this->values[hash][i];
	}

	return nullptr;
}
