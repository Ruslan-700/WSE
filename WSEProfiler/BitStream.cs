using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class BitStream
	{
		private FileStream _stream;
		private bool _opened;
		private uint _cursor;
		private uint _buffer;
		private ulong _position;
		private uint[] _maskTable = new uint[33];

		public BitStream(FileStream stream)
		{
			_opened = false;
			_cursor = 0;
			_buffer = 0;
			_maskTable[0] = 0;

			for (int i = 0; i < 32; ++i)
			{
				_maskTable[i + 1] = _maskTable[i] | (1u << i);
			}

			_stream = stream;
		}

		private void Read()
		{
			if (_opened && _cursor != 32)
				return;

			byte[] bytes = new byte[4];

			_stream.Read(bytes, 0, 4);
			_buffer = BitConverter.ToUInt32(bytes, 0);
			_cursor = 0;
			_opened = true;
		}

		public ulong Length()
		{
			return (ulong)_stream.Length * 8;
		}

		public ulong Position()
		{
			return _position;
		}

		public bool HasEnough(uint size)
		{
			return Length() - Position() >= size;
		}

		public void Seek(long offset, SeekOrigin origin)
		{
			_stream.Seek(offset, origin);
			_cursor = 0;
			_buffer = 0;
			_position = 0;
			_opened = false;
		}

		public uint ReadU32(uint size)
		{
			Read();

			if (_cursor + size > 32)
			{
				uint size1 = 32 - _cursor;
				uint size2 = size - size1;

				return ReadU32(size1) | (ReadU32(size2) << (int)size1);
			}

			uint value = (_buffer >> (int)_cursor) & _maskTable[size];
			_cursor += size;
			_position += size;

			return value;
		}

		public ulong ReadU64(uint size)
		{
			if (size <= 32)
			{
				return (ulong)ReadU32(size);
			}
			else
			{
				return (ulong)ReadU32(32) | ((ulong)ReadU32(size - 32) << 32);
			}
		}

		public uint ReadBCI15()
		{
			uint value;
			uint result = 0;
			uint det = 1;
			int tries = 0;

			while (tries++ < 20 && (value = ReadU32(4)) != 15)
			{
				result += value * det;
				det *= 15;
			}

			return result;
		}

		public string ReadString()
		{
			uint length = ReadU32(12);
			string value = "";

			for (int i = 0; i < length; ++i)
			{
				value += (char)ReadU32(8);
			}

			return value;
		}
	}
}
