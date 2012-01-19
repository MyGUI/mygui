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
			mIsColour(false),
			mWidth(0.0f),
			mHeight(0.0f),
			mAdvance(0.0f),
			mBearingX(0.0f),
			mBearingY(0.0f)
		{ }

		CharInfo(
			const FloatRect& _rect,
			float _width,
			float _height,
			float _advance,
			float _bearingX,
			float _bearingY) :
			mIsColour(false),
			mUVRect(_rect),
			mWidth(_width),
			mHeight(_height),
			mAdvance(_advance),
			mBearingX(_bearingX),
			mBearingY(_bearingY)
		{ }

		CharInfo(uint32 _colour) : mIsColour(true), mColour(_colour) { }

		bool isColour() const
		{
			return mIsColour;
		}

		float getWidth() const
		{
			return mWidth;
		}

		float getHeight() const
		{
			return mHeight;
		}

		float getAdvance() const
		{
			return mAdvance;
		}

		float getBearingX() const
		{
			return mBearingX;
		}

		float getBearingY() const
		{
			return mBearingY;
		}

		const FloatRect& getUVRect() const
		{
			return mUVRect;
		}

		uint32 getColour() const
		{
			return mColour;
		}

	private:
		bool mIsColour;
		FloatRect mUVRect;
		float mWidth;
		float mHeight;
		float mAdvance;
		float mBearingX;
		float mBearingY;
		uint32 mColour;
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
