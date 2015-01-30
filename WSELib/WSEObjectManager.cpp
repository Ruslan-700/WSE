#include "WSEObjectManager.h"

WSEObjectManager::WSEObjectManager()
{
	IncreaseStorageIfNeeded();
}

WSEObjectManager::~WSEObjectManager()
{
	for (std::list<WSEObjectType *>::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
	{
		delete *it;
	}

	for (std::vector<WSEObjectContainer *>::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		delete *it;
	}
}

WSEObjectType *WSEObjectManager::AllocType(const std::string &name)
{
	WSEObjectType *type = new WSEObjectType;

	type->m_name = name;
	m_types.push_back(type);
	return type;
}

void WSEObjectManager::FreeType(WSEObjectType *type)
{
	m_types.remove(type);
	delete type;
}

int WSEObjectManager::AllocObject(WSEObjectType *type, void *object)
{
	IncreaseStorageIfNeeded();

	int id = m_free_objects.back();

	m_objects[id]->m_type = type;
	m_objects[id]->m_object = object;
	m_free_objects.pop_back();
	return id;
}

void WSEObjectManager::FreeObject(WSEObjectType *type, int id)
{
	if (!IsValidId(id) || m_objects[id]->m_type != type)
		return;
	
	m_objects[id]->m_type = nullptr;
	m_objects[id]->m_object = nullptr;
	m_free_objects.push_back(id);
}

void *WSEObjectManager::FetchObject(WSEObjectType *type, int id)
{
	if (!IsValidId(id) || m_objects[id]->m_type != type)
		return nullptr;

	return m_objects[id]->m_object;
}

bool WSEObjectManager::IsValidId(int id) const
{
	return id >= 0 && (size_t)id < m_objects.size();
}

void WSEObjectManager::IncreaseStorageIfNeeded()
{
	if (!m_free_objects.empty())
		return;

	int cur_size = m_objects.size();

	m_objects.reserve(16);
	m_free_objects.reserve(16);

	for (int i = 0; i < 16; ++i)
	{
		WSEObjectContainer *container = new WSEObjectContainer;
		
		container->m_type = nullptr;
		container->m_object = nullptr;
		m_objects.push_back(container);
		m_free_objects.push_back(cur_size + i);
	}
}
