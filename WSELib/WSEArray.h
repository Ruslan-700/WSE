#pragma once

#include <Windows.h>
#include <vector>
#include "warband.h"

template <class T>
class WSEBaseArray;

template <class T>
class WSEDynamicArray : public WSEBaseArray<T>
{
public:
	WSEDynamicArray(T **address) : WSEBaseArray<T>(address)
	{
	}

	WSEDynamicArray(T **address, int *tracker) : WSEBaseArray<T>(address, tracker)
	{
	}

	virtual T *Allocate(size_t size)
	{
		return rgl::_new_vec<T>(size);
	}

	virtual void Free()
	{
		rgl::_delete_vec<T>(m_data);
	}

	virtual size_t GetSize()
	{
		return m_data ? *((size_t *)m_data - 1) : 0;
	}

	virtual void SetSize(size_t size)
	{
	}
};

template <class T>
class WSEStaticArray : public WSEBaseArray<T>
{
public:
	WSEStaticArray(T **address, size_t size) : WSEBaseArray<T>(address)
	{
		m_size = size;
	}

	WSEStaticArray(T **address, int *tracker) : WSEBaseArray<T>(address, tracker)
	{
		m_size = *tracker;
	}

	virtual T *Allocate(size_t size)
	{
		return rgl::_new<T>(size);
	}

	virtual void Free()
	{
		rgl::_delete<T>(m_data, m_size);
	}

	virtual size_t GetSize()
	{
		return m_size;
	}

	virtual void SetSize(size_t size)
	{
		m_size = size;
	}

private:
	size_t m_size;
};

template <class T>
class WSEBaseArray
{
public:
	WSEBaseArray(T **address)
	{
		m_ptr = address;
		m_data = *m_ptr;
		InitializeCriticalSection(&m_lock);
	}

	WSEBaseArray(T **address, int *tracker)
	{
		m_ptr = address;
		m_data = *m_ptr;
		InitializeCriticalSection(&m_lock);
		AddTracker(tracker);
	}

	void PushBack(const T &item)
	{
		EnterCriticalSection(&m_lock);

		size_t size = GetSize();
		T *new_data = Allocate(size + 1);

		for (size_t i = 0; i < size; ++i)
		{
			new_data[i] = m_data[i];
		}

		new_data[size] = item;
		ReplaceArray(new_data, size + 1);
		LeaveCriticalSection(&m_lock);
	}

	void Clear()
	{
		EnterCriticalSection(&m_lock);
		ReplaceArray(nullptr, 0);
		LeaveCriticalSection(&m_lock);
	}

	void Resize(size_t new_size)
	{
		EnterCriticalSection(&m_lock);

		size_t size = GetSize();

		if (new_size != size)
		{
			T *new_data = Allocate(new_size);

			size = new_size > size ? size : new_size;

			for (size_t i = 0; i < size; ++i)
			{
				new_data[i] = m_data[i];
			}

			ReplaceArray(new_data, new_size);
		}

		LeaveCriticalSection(&m_lock);
	}

	void RemoveAt(size_t index)
	{
		EnterCriticalSection(&m_lock);

		size_t size = GetSize();
#if defined DEBUG
		assert(index < size);
#endif
		if (size == 1)
		{
			Clear();
		}
		else
		{
			T *new_data = Allocate(size - 1);
		
			for (size_t i = 0; i < index; ++i)
			{
				new_data[i] = m_data[i];
			}

			for (size_t i = index + 1; i < size; ++i)
			{
				new_data[i - 1] = m_data[i];
			}

			ReplaceArray(new_data, size - 1);
		}

		LeaveCriticalSection(&m_lock);
	}

	T &operator [](size_t index)
	{
#if defined DEBUG
		assert(index < GetSize());
#endif

		return m_data[index];
	}

	void AddTracker(int *tracker)
	{
		m_trackers.push_back(tracker);
	}
	
	virtual T *Allocate(size_t size) = 0;
	virtual void Free() = 0;
	virtual size_t GetSize() = 0;
	virtual void SetSize(size_t size) = 0;

private:
	void ReplaceArray(T *new_array, size_t new_size)
	{
		Free();
		m_data = new_array;
		*m_ptr = m_data;
		SetSize(new_size);

		for (size_t i = 0; i < m_trackers.size(); ++i)
		{
			*m_trackers[i] = new_size;
		}
	}

protected:
	T *m_data;
	T **m_ptr;
	CRITICAL_SECTION m_lock;
	std::vector<int *> m_trackers;
};
