#pragma once

#define rgl_hashmap_size 10007
#define rgl_sv_chunk_size 16
#define rgl_sv_chunk_bits 4
#define rgl_sv_chunk_mask 0xF

namespace rgl
{
	struct hash
	{
		int id;
		int raw_id;
	};

	template <typename T>
	struct hash_vector
	{
		hash hashes[rgl_hashmap_size];
		int num_hashes;
		int num_ids;
		stl::vector<int> ids;
		stl::vector<T *> items;
		int num_created;
		int u1;

		int get_raw_index(int index)
		{
			if (index < rgl_hashmap_size && hashes[index].id == index)
				return hashes[index].raw_id;

			int hash_key = index % rgl_hashmap_size;

			if (hashes[hash_key].id == index)
				return hashes[hash_key].raw_id;

			for (int i = 0; i < 1000; ++i)
			{
				hash_key = (hash_key + 79) % rgl_hashmap_size;

				if (hashes[hash_key].id == index)
					return hashes[hash_key].raw_id;
			}

			return -1;
		}

		T &raw_data_at(int raw_index)
		{
			return items[raw_index >> rgl_sv_chunk_bits][raw_index & rgl_sv_chunk_mask];
		}

		bool is_valid_index(int index)
		{
			if (index < 0 || index >= num_created)
				return false;

			int raw_index = get_raw_index(index);

			if (raw_index < 0)
				return false;

			return raw_data_at(raw_index).valid == 1;
		}

		int get_first_valid_index()
		{
			for (int index = 0; index < this->num_ids; ++index)
			{
				if (raw_data_at(index).valid == 1)
					return this->raw_data_at(index).no;
			}
			
			return this->num_created;
		}

		int get_next_valid_index(int index)
		{
			for (int i = this->get_raw_index(index) + 1; i < this->num_ids; ++i)
			{
				if (raw_data_at(i).valid == 1)
					return this->raw_data_at(i).no;
			}
			
			return this->num_created;
		}

		T &operator[](int index)
		{
			return raw_data_at(get_raw_index(index));
		}
	};
}
