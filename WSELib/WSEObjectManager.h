#pragma once

#include <list>
#include <string>
#include <vector>

struct WSEObjectType
{
	std::string m_name;
};

struct WSEObjectContainer
{
	WSEObjectType *m_type;
	void *m_object;
};

class WSEObjectManager
{
public:
	WSEObjectManager();
	~WSEObjectManager();
	WSEObjectType *AllocType(const std::string &name);
	void FreeType(WSEObjectType *type);
	int AllocObject(WSEObjectType *type, void *object);
	void FreeObject(WSEObjectType *type, int id);
	void *FetchObject(WSEObjectType *type, int id);

private:
	bool IsValidId(int id) const;
	void IncreaseStorageIfNeeded();

private:
	std::list<WSEObjectType *> m_types;
	std::vector<WSEObjectContainer *> m_objects;
	std::vector<int> m_free_objects;
};
