/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#ifndef __BUFFER_H__
#define __BUFFER_H__

namespace helper
{
	class Buffer
	{
	public:
		enum { BIT_IN_COLOUR = 4 };
		typedef unsigned char byte;

		Buffer() :
			mDirty(false),
			mBuffer(0),
			mSize(0),
			mWidthVisible(0),
			mHeightVisible(0)
		{
			// чтобы не проверять везде буфер
			mBuffer = new byte[0];
		}

		~Buffer()
		{
			if (mBuffer != 0)
			{
				delete[] mBuffer;
				mBuffer = 0;
			}
		}

		void resize(int _width, int _height)
		{
			mDirty = true;

			mWidthVisible = std::max(0, _width);
			mHeightVisible = std::max(0, _height);

			int need_len = mWidthVisible * mHeightVisible;
			int current_len = mSize;

			if (need_len > current_len)
			{
				if (mBuffer != 0)
				{
					delete[] mBuffer;
					mBuffer = 0;
				}

				const int added = 64;

				mSize = (mWidthVisible + added) * (mHeightVisible + added);
				int len = mSize * BIT_IN_COLOUR;

				mBuffer = new byte[len];
				memset(mBuffer, (int)0xFFFFFFFF, len);
			}
			else
			{
				// обнуляем только видимую часть
				byte* source = mBuffer;
				int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;

				for (int y=0; y<mHeightVisible; ++y)
				{
					memset(source, (int)0xFFFFFFFF, width_bytes_visible);
					source += width_bytes_visible;
				}
			}
		}

		void draw(void* _data, int _width)
		{
			mDirty = false;

			byte* data = (byte*)_data;
			byte* source = mBuffer;
			int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;
			int width_bytes_dest = _width * BIT_IN_COLOUR;

			for (int y=0; y<mHeightVisible; ++y)
			{
				memcpy(data, source, width_bytes_visible);
				data += width_bytes_dest;
				source += width_bytes_visible;
			}
		}

		void update(void* _data, int _left, int _top, int _width, int _height)
		{
			mDirty = true;

			byte* original_data = mBuffer;
			byte* data = original_data + (((_top * mWidthVisible) + _left) * BIT_IN_COLOUR);
			byte* source = (byte*)_data;
			int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;
			int width_bytes_dest = _width * BIT_IN_COLOUR;

			for (int y=0; y<_height; ++y)
			{
				memcpy(data, source, width_bytes_dest);
				data += width_bytes_visible;
				source += width_bytes_dest;
			}

			int count = mWidthVisible * mHeightVisible;
			data = original_data + 3;
			for (int index=0; index<count; ++index)
			{
				*data = 0xFF;
				data += BIT_IN_COLOUR;
			}
		}

		bool isDirty() { return mDirty; }

	private:
		bool mDirty;
		byte* mBuffer;
		int mSize;
		int mWidthVisible;
		int mHeightVisible;
	};
}

#endif // __BUFFER_H__
