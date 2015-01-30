#pragma once

#include <cassert>
#include <cstdlib>
#include <memory>
#include "rgl_memory.h"

namespace stl
{
	template <typename T>
	struct vector
	{
	public:
		vector()
		{
			this->start_ptr = nullptr;
			this->end_ptr = nullptr;
			this->max_ptr = nullptr;
		};

		vector(const vector<T> &obj)
		{
			*this = obj;
		};

		~vector()
		{
			this->clear();
		}

		int size() const
		{
			return this->end_ptr - this->start_ptr;
		};

		int max_size() const
		{
			return UINT_MAX / sizeof(T);
		};

		int capacity() const
		{
			return this->max_ptr - this->start_ptr;
		};

		bool empty() const
		{
			return this->size() == 0;
		};

		T &front()
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return *this->start_ptr;
		};

		const T &front() const
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return *this->start_ptr;
		};

		T &back()
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return *(this->end_ptr - 1);
		};

		const T &back() const
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return *(this->end_ptr - 1);
		};

		void push_back(const T &value)
		{
			this->resize(this->size() + 1);
			*(end_ptr - 1) = value;
		};

		void pop_back()
		{
			if (this->size() == 0)
				return;

			(*(this->end_ptr - 1)).~T();
			this->end_ptr--;
		};

		void resize(const int &new_size)
		{
#if defined DEBUG
			assert(new_size < this->max_size());
#endif

			int size = this->size();

			if (new_size == size)
			{
				return;
			}
			else if (new_size < size)
			{
				for (int i = new_size; i < size; ++i)
				{
					this->start_ptr[i].~T();
				}

				this->end_ptr = this->start_ptr + new_size;
				return;
			}
			
			this->reserve(new_size);

			for (int i = size; i < new_size; ++i)
			{
				new(&this->start_ptr[i]) T;
			}

			this->end_ptr = this->start_ptr + new_size;
		};

		void reserve(int new_size)
		{
#if defined DEBUG
			assert(new_size < this->max_size());
#endif
			
			int size = this->size();
			int capacity = this->capacity();

			if (new_size <= capacity)
				return;

			T *new_ptr = rgl::_new<T>(new_size);

			if (start_ptr)
			{
				for (int i = 0; i < size; ++i)
				{
					new_ptr[i] = this->start_ptr[i];
				}
				
				rgl::_delete(this->start_ptr, size);
			}
		
			this->start_ptr = new_ptr;
			this->end_ptr = new_ptr + size;
			this->max_ptr = new_ptr + new_size;
		};

		void clear()
		{
			if (start_ptr)
				rgl::_delete(this->start_ptr, this->size());
		
			this->start_ptr = nullptr;
			this->end_ptr = nullptr;
			this->max_ptr = nullptr;
		};

		void insert(const int &index, const T &value)
		{
			int size = this->size();

			if (index < 0 || index > size)
				return;

			if (index == size)
			{
				this->push_back(value);
				return;
			}

			this->resize(size + 1);

			for (int i = size; i > index; --i)
			{
				this->start_ptr[i] = this->start_ptr[i - 1];
			}

			this->start_ptr[index] = value;
		}

		void remove_at(const int &index)
		{
			if (index < 0 || index >= this->size())
				return;

			for (T *i = this->start_ptr + index; i < this->end_ptr - 1; ++i)
			{
				*i = *(i + 1);
			}

			this->pop_back();
		}

		void remove(const T &value)
		{
			int index;

			while ((index = this->find(value)) >= 0)
			{
				this->remove_at(index);
			}
		}

		void remove_first(const T &value)
		{
			this->remove_at(this->find(value));
		}

		void remove_last(const T &value)
		{
			this->remove_at(this->rfind(value));
		}

		int find(const T &value)
		{
			int size = this->size();

			for (int i = 0; i < size; ++i)
			{
				if (this->start_ptr[i] == value)
					return i;
			}

			return -1;
		};

		int rfind(const T &value)
		{
			int size = this->size();

			for (int i = size - 1; i >= 0; --i)
			{
				if (this->start_ptr[i] == value)
					return i;
			}

			return -1;
		};

		bool contains(const T &value)
		{
			return this->find(value) >= 0;
		}

		T &operator[](const int &index)
		{
#if defined DEBUG
			assert(index >= 0 && index < this->size());
#endif
		
			return this->start_ptr[index];
		};

		const T &operator[](const int &index) const
		{
#if defined DEBUG
			assert(index >= 0 && index < this->size());
#endif
					
			return this->start_ptr[index];
		};

		vector<T> &operator=(const vector<T> &obj)
		{
			int size = obj.size();

			this->resize(size);

			for (int i = 0; i < size; ++i)
			{
				this->start_ptr[i] = obj.start_ptr[i];
			}

			return *this;
		};
	
	private:
		vector<T> **self_ptr;
		T *start_ptr;
		T *end_ptr;
		T *max_ptr;
	};
}
