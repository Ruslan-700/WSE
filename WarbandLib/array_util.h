#pragma once

#include "rgl_memory.h"

//Lets you add and remove from static game arrays

template <typename T>
int array_add_elem(T* &buff, int& buffer_size, const T &new_elem)
{
	if (buff == NULL) return -1;

	T* new_buff = rgl::_new<T>(buffer_size + 1);

	for (int i = 0; i < buffer_size; ++i)
	{
		new_buff[i] = buff[i];
	}
	rgl::_free(buff);
	buff = new_buff;
	buff[buffer_size] = new_elem;

	return buffer_size++;
}

template <typename T>
bool array_remove_elem(T* &buff, int& buffer_size, int index)
{
	if (buff == NULL) return false;
	if (index < 0) index += buffer_size;
	if (index < 0 || index >= buffer_size) return false;

	T* new_buff = rgl::_new<T>(buffer_size - 1);

	for (int i = 0; i < index; ++i)
	{
		new_buff[i] = buff[i];
	}
	for (int i = index; i < buffer_size - 1; ++i)
	{
		new_buff[i] = buff[i + 1];
	}
	rgl::_free(buff);
	buff = new_buff;
	buffer_size--;

	return true;
}