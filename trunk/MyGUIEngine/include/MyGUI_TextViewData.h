/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/
#ifndef __MYGUI_TEXT_VIEW_DATA_H__
#define __MYGUI_TEXT_VIEW_DATA_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class CharInfo
	{
	public:
		CharInfo() :
			width(0)
		{
		}

		CharInfo(const FloatRect& _rect, int _width) :
			rect(_rect),
			width(_width)
		{
		}

		CharInfo(uint32 _colour) :
			rect(-1, 0, 0, 0),
			width((int)_colour)
		{
		}

		bool isColour() const
		{
			return rect.left == -1;
		}

		int getWidth() const
		{
			return width;
		}

		const FloatRect& getUVRect() const
		{
			return rect;
		}

		uint32 getColour() const
		{
			return (uint32)width;
		}

	private:
		FloatRect rect;
		int width;
	};

	typedef std::vector<CharInfo> VectorCharInfo;

	struct LineInfo
	{
		LineInfo() :
			width(0),
			offset(0),
			count(0)
		{
		}

		void clear()
		{
			width = 0;
			count = 0;
			simbols.clear();
			offset = 0;
		}

		int width;
		int offset;
		size_t count;
		VectorCharInfo simbols;
	};

	typedef std::vector<LineInfo> VectorLineInfo;

} // namespace MyGUI

#endif // __MYGUI_TEXT_VIEW_DATA_H__
