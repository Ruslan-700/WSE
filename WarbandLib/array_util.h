#pragma once

#include "rgl_memory.h"
#include <unordered_map>

/*
	Lets you add and remove from static game arrays
	It will double the buffer size when it's full, and keep track of the capacity.
*/

static std::unordered_map<void*, int> capacities;

template <typename T>
int array_add_elem(T* &buff, int& elem_count, const T &new_elem)
{
	//In my tests the elem_count was always 0 for NULL buff anyway, but still good to make sure
	if (buff == NULL) elem_count = 0;

	int cap;

	auto it = capacities.find((void*)buff);
	cap = (it == capacities.end() ? elem_count : it->second);

	if (elem_count >= cap)
	{
		cap = max(cap * 2, 8);

		T* new_buff = rgl::_new<T>(cap);
		capacities[new_buff] = cap;

		for (int i = 0; i < elem_count; ++i)
		{
			new_buff[i] = buff[i];
		}
		rgl::_free(buff);
		capacities.erase((void*)buff);

		buff = new_buff;
	}

	buff[elem_count] = new_elem;

	return elem_count++;
}

template <typename T>
bool array_remove_elem(T* &buff, int& elem_count, int index)
{
	if (buff == NULL) return false;
	if (index < 0) index += elem_count;
	if (index < 0 || index >= elem_count) return false;

	auto it = capacities.find((void*)buff);
	if (it == capacities.end()) capacities[(void*)buff] = elem_count; //this array was untracked so far, we need to store the capacity now.

	for (int i = index; i < elem_count - 1; ++i)
	{
		buff[i] = buff[i + 1];
	}

	elem_count--;
	return true;
}