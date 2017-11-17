#pragma once

#include <cassert>
#include <cstdlib>
#include <memory>
#include "rgl_memory.h"

#define DEQUE_MAP_SIZE 8
#define DEQUE_SIZE (sizeof(T) <= 1 ? 16 : sizeof(T) <= 2 ? 8 : sizeof(T) <= 4 ? 4 : sizeof(T) <= 8 ? 2 : 1)

namespace stl
{
	template <typename T>
	struct deque
	{
	public:
		deque()
		{
			this->self_ptr = rgl::_new<deque<T> *>();
			*this->self_ptr = this;
			this->map = nullptr;
			this->map_size = 0;
			this->offset = 0;
			this->count = 0;
		}

		deque(const deque<T> &obj)
		{
			this->self_ptr = rgl::_new<deque<T> *>();
			*this->self_ptr = this;
			this->map = nullptr;
			this->map_size = 0;
			this->offset = 0;
			this->count = 0;
			*this = obj;
		}

		~deque()
		{
			this->clear();
			rgl::_delete(this->self_ptr);
		}

		int size() const
		{
			return this->count;
		}

		int max_size() const
		{
			return UINT_MAX / sizeof(T);
		}

		bool empty() const
		{
			return !this->count;
		}

		T &front()
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return (*this)[0];
		};

		const T &front() const
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return (*this)[0];
		};

		T &back()
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return (*this)[this->size() - 1];
		};

		const T &back() const
		{
#if defined DEBUG
			assert(this->size() >= 0);
#endif
			return (*this)[this->size() - 1];
		};

		void push_back(const T &item)
		{
			if ((this->offset + this->count) % DEQUE_SIZE == 0 && this->map_size <= (this->count + DEQUE_SIZE) / DEQUE_SIZE)
				this->grow_map(1);

			size_t new_offset = this->count + this->offset;
			size_t block = new_offset / DEQUE_SIZE;

			if (this->map_size <= block)
				block -= this->map_size;

			if (!this->map[block])
				this->map[block] = rgl::_new<T>(DEQUE_SIZE);

			rgl::_construct(&this->map[block][new_offset % DEQUE_SIZE], item);
			this->count++;
		}

		void push_front(const T &item)
		{
			if (this->offset % DEQUE_SIZE == 0 && this->map_size <= (this->count + DEQUE_SIZE) / DEQUE_SIZE)
				this->grow_map(1);

			size_t new_offset = this->offset != 0 ? this->offset : this->map_size * DEQUE_SIZE;
			size_t block = --new_offset / DEQUE_SIZE;

			if (!this->map[block])
				this->map[block] = rgl::_new<T>(DEQUE_SIZE);
			
			rgl::_construct(&this->map[block][new_offset % DEQUE_SIZE], item);
			this->offset = new_offset;
			this->count++;
		}

		void pop_back()
		{
			if (this->empty())
				return;

			size_t new_offset = this->count + this->offset - 1;
			size_t block = new_offset / DEQUE_SIZE;

			if (this->map_size <= block)
				block -= this->map_size;
			
			rgl::_destroy(&this->map[block][new_offset % DEQUE_SIZE]);

			if (!--this->count)
				this->offset = 0;
		}

		void pop_front()
		{
			if (this->empty())
				return;

			size_t block = this->offset / DEQUE_SIZE;
			
			rgl::_destroy(&this->map[block][this->offset % DEQUE_SIZE]);

			if (this->map_size * DEQUE_SIZE <= ++this->offset)
				this->offset = 0;

			if (!--this->count)
				this->offset = 0;
		}

		void clear()
		{
			while (!this->empty())
				this->pop_back();

			if (this->map)
			{
				for (size_t i = this->map_size - 1; i > 0; --i)
				{
					if (this->map[i])
						rgl::_deallocate(this->map[i]);
				}
				rgl::_deallocate(this->map);
			}
				
			this->map_size = 0;
			this->map = nullptr;
		}

		T &operator[](size_t index)
		{
			size_t block = (this->offset + index) / DEQUE_SIZE;
			size_t block_offset = (this->offset + index) & (DEQUE_SIZE - 1);

			if (this->map_size <= block)
				block -= this->map_size;

			return this->map[block][block_offset];
		}

		const T &operator[](size_t index) const
		{
			size_t block = (this->offset + index) / DEQUE_SIZE;
			size_t block_offset = (this->offset + index) & (DEQUE_SIZE - 1);

			if (this->map_size <= block)
				block -= this->map_size;

			return this->map[block][block_offset];
		}

		deque<T> &operator=(const deque<T> &obj)
		{
			for (size_t i = 0; i < obj.size(); ++i)
			{
				this->push_back(obj[i]);
			}

			return *this;
		};

	private:
		void grow_map(size_t count)
		{
#if defined DEBUG
			assert(this->max_size() / DEQUE_SIZE - this->map_size >= count);
#endif

			size_t increase = this->map_size / 2;

			if (increase < DEQUE_MAP_SIZE)
				increase = DEQUE_MAP_SIZE;

			if (count < increase && this->map_size <= max_size() / DEQUE_SIZE - increase)
				count = increase;

			size_t block_offset = this->offset / DEQUE_SIZE;
			T **new_map = rgl::_new<T *>(this->map_size + count);
			T **new_ptr = new_map + block_offset;

			memmove(new_ptr, map + block_offset, (map_size - block_offset) * sizeof(T *));
			new_ptr += map_size - block_offset;

			if (block_offset <= count)
			{
				memmove(new_ptr, map, block_offset * sizeof(T *));
				new_ptr += block_offset;

				for (size_t i = 0; i < count - block_offset; ++i)
				{
					new_ptr[i] = nullptr;
				}

				for (size_t i = 0; i < block_offset; ++i)
				{
					new_map[i] = nullptr;
				}
			}
			else
			{
				memmove(new_ptr, map, count * sizeof(T *));
				memmove(new_map, map + count, (block_offset - count) * sizeof(T *));
				new_ptr += block_offset - count;

				for (size_t i = 0; i < count; ++i)
				{
					new_ptr[i] = nullptr;
				}
			}

			// _Destroy_range(this->_Map + _Myboff, this->_Map + this->_Mapsize, this->_Almap); TODO: how to do this?

			if (this->map)
				rgl::_delete(this->map);

			this->map = new_map;
			this->map_size += count;
		}

	private:
		deque<T> **self_ptr;
		void *u1;
		void *u2;
		void *u3;
		T **map;
		size_t map_size;
		size_t offset;
		size_t count;
	};
}
