#pragma once

#include <cassert>
#include <cstdlib>
#include <memory>
#include "rgl_memory.h"

namespace stl
{
	template <typename T>
	struct list_node
	{
		list_node<T> *next;
		list_node<T> *prev;
		T data;
	};

	template <typename T>
	struct list
	{
		int size() const
		{
			return cur_size;
		};

		bool empty() const
		{
			return cur_size == 0;
		};

		T &front()
		{
#if defined DEBUG
			assert(cur_size >= 0);
#endif

			return items->next->data;
		};

		const T &front() const
		{
#if defined DEBUG
			assert(cur_size >= 0);
#endif

			return items->next->data;
		};

		T &back()
		{
#if defined DEBUG
			assert(cur_size >= 0);
#endif

			return items->prev->data;
		};

		const T &back() const
		{
#if defined DEBUG
			assert(cur_size >= 0);
#endif

			return items->prev->data;
		};

		void push_back(const T &value)
		{
			list_node<T> *new_node = rgl::_new<list_node<T> >();

			new_node->prev = items->prev;
			new_node->prev->next = new_node;
			new_node->next = items;
			items->prev = new_node;
			new_node->data = value;
			cur_size++;
		};

		void pop_back()
		{
			if (cur_size == 0)
				return;

			list_node<T> *last_node = items->prev;

			last_node->prev->next = items;
			items->prev = last_node->prev;
			last_node->data.~T();
			rgl::_delete(last_node);
			cur_size--;
		};

		void remove(const T &value)
		{
			for (list_node<T> *i = this->items->next; i != this->items; i = i->next)
			{
				if (i->data == value)
				{
					list_node<T> *item = i;
					
					i = i->prev;
					remove_node(item);
				}
			}
		}

		void remove_first(const T &value)
		{
			for (list_node<T> *i = this->items->next; i != this->items; i = i->next)
			{
				if (i->data == value)
				{
					list_node<T> *item = i;
					
					i = i->prev;
					remove_node(item);
					return;
				}
			}
		}

		void remove_last(const T &value)
		{
			for (list_node<T> *i = this->items->prev; i != this->items; i = i->prev)
			{
				if (i->data == value)
				{
					list_node<T> *item = i;
					
					i = i->next;
					remove_node(item);
					return;
				}
			}
		}

		bool contains(const T &value)
		{
			for (list_node<T> *i = this->items->next; i != this->items; i = i->next)
			{
				if (i->data == value)
					return true;
			}

			return false;
		}

		T &operator[](const size_t &index)
		{
#if defined DEBUG
			assert(index < cur_size);
#endif
			list_node<T> *cur_node;

			if (index < cur_size / 2)
			{
				cur_node = items->prev;

				for (size_t i = cur_size - 1; i > index; --i)
				{
					cur_node = cur_node->prev;
				}
			}
			else
			{
				cur_node = items->next;

				for (size_t i = 0; i < index; ++i)
				{
					cur_node = cur_node->next;
				}
			}
		
			return cur_node->data;
		};

		const T &operator[](const size_t &index) const
		{
#if defined DEBUG
			assert(index < cur_size);
#endif
			list_node<T> *cur_node;

			if (index < cur_size / 2)
			{
				cur_node = items->prev;

				for (size_t i = cur_size - 1; i > index; -i)
				{
					cur_node = cur_node->prev;
				}
			}
			else
			{
				cur_node = items->next;

				for (size_t i = 0; i < index; ++i)
				{
					cur_node = cur_node->next;
				}
			}
		
			return cur_node->data;
		};

	private:
		list() // TODO: Don't allow instantiation - not ready.
		{
			self_ptr = rglnew(list<T> *);
			*self_ptr = this;
			items = nullptr;
			size = 0;
		}

		void remove_node(list_node<T> *node)
		{
			if (node == this->items)
				return;

			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->data.~T();
			rgl::_delete(node);
			cur_size--;
		}
	
	private:
		list<T> **self_ptr;
		list_node<T> *items;
		size_t cur_size;
	};
}
