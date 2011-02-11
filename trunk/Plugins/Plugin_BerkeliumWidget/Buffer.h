/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/
#ifndef __BUFFER_H__
#define __BUFFER_H__

namespace helpers
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
			// ����� �� ��������� ����� �����
			mBuffer = new byte[0];
		}

		~Buffer()
		{
			if (mBuffer != 0)
			{
				delete [] mBuffer;
				mBuffer = 0;
			}
		}

		void resize(int _width, int _height)
		{
			mDirty = true;

			int oldWidth = mWidthVisible;
			int oldHeight = mHeightVisible;
			mWidthVisible = std::max(0, _width);
			mHeightVisible = std::max(0, _height);

			int need_len = mWidthVisible * mHeightVisible;
			int current_len = mSize;

			// ������ �� ����������, ������������ �����
			if (need_len > current_len)
			{
				const int added = 64;
				mSize = (mWidthVisible + added) * (mHeightVisible + added);
				int len = mSize * BIT_IN_COLOUR;

				// ������� ����� ����� � �������� � ���� ������ �������� �����
				byte* new_data = new byte[len];

				int height = std::min(oldHeight, mHeightVisible);
				byte* source = mBuffer;
				byte* dest = new_data;

				int width_bytes_dest = mWidthVisible * BIT_IN_COLOUR;
				int width_bytes_source = oldWidth * BIT_IN_COLOUR;
				int dif = width_bytes_dest - width_bytes_source;

				// ������ �����������, �������� ����� �����
				if (oldWidth < mWidthVisible)
				{
					dest += width_bytes_dest * height;
					source += width_bytes_source * height;
					for (int y = 0; y < height; ++y)
					{
						dest -= width_bytes_dest;
						source -= width_bytes_source;
						memcpy(dest, source, width_bytes_source);
						// ����� ����������, ����������� ����� ���������� �����
						memset(dest + width_bytes_source, (int)0xFFFFFFFF, dif);
					}
				}
				// ������ �����������, �������� ������ ����
				else if (oldWidth > mWidthVisible)
				{
					for (int y = 0; y < height; ++y)
					{
						memcpy(dest, source, width_bytes_source);
						dest += width_bytes_dest;
						source += width_bytes_source;
					}
				}

				// ������ �����������, ����������� ����� ���������� �����
				if (oldHeight < mHeightVisible)
				{
					dest = new_data + (oldHeight * width_bytes_dest);
					for (int y = oldHeight; y < mHeightVisible; ++y)
					{
						memset(dest, (int)0xFFFFFFFF, width_bytes_dest);
						dest += width_bytes_dest;
					}
				}

				// ������� ������ �����
				if (mBuffer != 0)
				{
					delete [] mBuffer;
					mBuffer = 0;
				}
				mBuffer = new_data;
			}
			else
			{
				int height = std::min(oldHeight, mHeightVisible);
				byte* source = mBuffer;
				byte* dest = mBuffer;

				int width_bytes_dest = mWidthVisible * BIT_IN_COLOUR;
				int width_bytes_source = oldWidth * BIT_IN_COLOUR;
				int dif = width_bytes_dest - width_bytes_source;

				// ������ �����������, �������� ����� �����
				// ����� ���� � ��� ��, ���������� memmove
				if (oldWidth < mWidthVisible)
				{
					dest += width_bytes_dest * height;
					source += width_bytes_source * height;
					for (int y = 0; y < height; ++y)
					{
						dest -= width_bytes_dest;
						source -= width_bytes_source;
						memmove(dest, source, width_bytes_source);
						// ����� ����������, ����������� ����� ���������� �����
						memset(dest + width_bytes_source, (int)0xFFFFFFFF, dif);
					}
				}
				// ������ �����������, �������� ������ ����
				// ����� ���� � ��� ��, ���������� memmove
				else if (oldWidth > mWidthVisible)
				{
					for (int y = 0; y < height; ++y)
					{
						memmove(dest, source, width_bytes_source);
						dest += width_bytes_dest;
						source += width_bytes_source;
					}
				}

				// ������ �����������, ����������� ����� ���������� �����
				if (oldHeight < mHeightVisible)
				{
					dest = mBuffer + (oldHeight * width_bytes_dest);
					for (int y = oldHeight; y < mHeightVisible; ++y)
					{
						memset(dest, (int)0xFFFFFFFF, width_bytes_dest);
						dest += width_bytes_dest;
					}
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

			for (int y = 0; y < mHeightVisible; ++y)
			{
				memcpy(data, source, width_bytes_visible);
				data += width_bytes_dest;
				source += width_bytes_visible;
			}
		}

		void scroll(int _dx, int _dy, int _left, int _top, int _width, int _height)
		{
			if (_width > 0 && _height > 0)
			{
				// ���� �� �� �� ������� ������ ������ �� �������������� �������
				_width -= std::abs(_dx);
				_height -= std::abs(_dy);

				if (_width > 0 && _height > 0)
				{
					if (_dx < 0) _left -= _dx;
					if (_dy < 0) _top -= _dy;

					byte* data = mBuffer + (((_top * mWidthVisible) + _left) * BIT_IN_COLOUR);
					if (_dy > 0)
						update_scroll_down(data, _left + _dx, _top + _dy, _width, _height);
					else
						update_scroll_up(data, _left + _dx, _top + _dy, _width, _height);
				}
			}
		}

		void update_scroll_up(void* _data, int _left, int _top, int _width, int _height)
		{
			mDirty = true;

			byte* data = mBuffer + (((_top * mWidthVisible) + _left) * BIT_IN_COLOUR);
			byte* source = (byte*)_data;
			int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;
			int width_bytes_dest = _width * BIT_IN_COLOUR;

			for (int y = 0; y < _height; ++y)
			{
				// �������� �� ���� � ���� ��, ���� memmove
				memmove(data, source, width_bytes_dest);
				// �������� �� � ����, ������ ������ ����� �� ��� � � ���������
				data += width_bytes_visible;
				source += width_bytes_visible;
			}
		}

		void update_scroll_down(void* _data, int _left, int _top, int _width, int _height)
		{
			mDirty = true;

			byte* data = mBuffer + (((_top * mWidthVisible) + _left) * BIT_IN_COLOUR);
			byte* source = (byte*)_data;
			int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;
			int width_bytes_dest = _width * BIT_IN_COLOUR;

			data += (width_bytes_visible * _height);
			source += (width_bytes_visible * _height);

			for (int y = 0; y < _height; ++y)
			{
				// �������� �� � ����, ������ ������ ����� �� ��� � � ���������
				data -= width_bytes_visible;
				source -= width_bytes_visible;
				// �������� �� ���� � ���� ��, ���� memmove
				memmove(data, source, width_bytes_dest);
			}
		}

		void update(void* _data, int _left, int _top, int _width, int _height, int sourceLeft, int sourceTop, int sourceWidth)
		{
			mDirty = true;

			byte* original_data = mBuffer;
			byte* data = original_data + (((_top * mWidthVisible) + _left) * BIT_IN_COLOUR);
			byte* source = (byte*)_data + (((sourceTop * sourceWidth) + sourceLeft) * BIT_IN_COLOUR);
			int width_bytes_visible = mWidthVisible * BIT_IN_COLOUR;
			int width_bytes_dest = sourceWidth * BIT_IN_COLOUR;
			int width_bytes_copy = _width * BIT_IN_COLOUR;

			for (int y = 0; y < _height; ++y)
			{
				memcpy(data, source, width_bytes_copy);
				data += width_bytes_visible;
				source += width_bytes_dest;
			}
		}

		bool isDirty() const
		{
			return mDirty;
		}

	private:
		bool mDirty;
		byte* mBuffer;
		int mSize;
		int mWidthVisible;
		int mHeightVisible;
	};
}

#endif // __BUFFER_H__
