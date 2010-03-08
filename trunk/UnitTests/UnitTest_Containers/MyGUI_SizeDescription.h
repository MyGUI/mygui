/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
*/
#ifndef __MYGUI_SIZE_DESCRIPTION_H__
#define __MYGUI_SIZE_DESCRIPTION_H__

#include <MyGUI_Macros.h>
#include <MyGUI_Types.h>
#include <math.h>

namespace MyGUI
{
	enum SizeBehaviour
	{
		SB_MIN = MYGUI_FLAG(1),
		SB_MAX = MYGUI_FLAG(2),
		SB_CONSTRICT = MYGUI_FLAG(3),
		SB_WIDE = MYGUI_FLAG(4)
	};

	enum FloatMode
	{
		FM_PARENT,
		FM_FREE_SPACE
	};

	//extern IntSize INT_SIZE_UNBOUND;
	static const IntSize INT_SIZE_UNBOUND(10000, 10000);

	class SizeParam
	{
		int mPx;

		float mFl;

		FloatMode mFlMode;

	public:
		SizeParam() : mPx(0), mFl(0), mFlMode(FM_PARENT) {}
		SizeParam(int _px) : mPx(_px), mFl(0), mFlMode(FM_PARENT) { }
		SizeParam(float _fl, FloatMode _flMode = FM_PARENT) : mPx(0), mFl(_fl), mFlMode(_flMode) { }
		void px(int _px){ mPx = _px; mFl = 0; }
		void fl(float _fl, FloatMode _flMode){ mFl = _fl; mPx = 0; mFlMode = _flMode; }
		float fl() const { return mFl; }
		int px() const { return mPx; }
		bool isFl() const { return mPx == 0; }
		bool isPx() const { return fabs(mFl) < 0.0001; }

		bool isParentFl() const { return isFl() && mFlMode == FM_PARENT; }
		bool isFreeSpaceFl() const { return isFl() && mFlMode == FM_FREE_SPACE; }
		bool isFloatMode(FloatMode _flMode) const { return isFl() && mFlMode == _flMode; }

		bool isNull() const
		{
			return fabs(mFl) < 0.0001 && mPx == 0;
		}

		/**
			Text-Serialization:
			Examples:
				p300 // (p)ixel mode - 300
				fp0.50 // (f)ree mode (p)arent
				ff0.50 // (f)ree mode (f)ree space
		*/

		bool fromString(const std::string& str);

		void toString(std::string& output) const;
	};

	struct Dimension
	{
		SizeParam w;
		SizeParam h;

		Dimension(){}

		Dimension(int _w, int _h)
		{
			w.px(_w);
			h.px(_h);
		}

		Dimension(float _w, float _h, FloatMode _flMode)
		{
			w.fl(_w, _flMode);
			h.fl(_h, _flMode);
		}

		void fl(float _w, float _h, FloatMode _flMode)
		{
			w.fl(_w, _flMode);
			h.fl(_h, _flMode);
		}

		void px(int _w, int _h)
		{
			w.px(_w);
			h.px(_h);
		}

		void fl(const FloatSize& _fl, FloatMode _flMode)
		{
			w.fl(_fl.width, _flMode);
			h.fl(_fl.height, _flMode);
		}

		void px(const IntSize& _px)
		{
			w.px(_px.width);
			h.px(_px.height);
		}

		bool isPx() const
		{
			return w.isPx() && h.isPx();
		}

		bool isFl() const
		{
			return w.isFl() && h.isFl();
		}

		IntSize px() const
		{
			return IntSize(w.px(), h.px());
		}

		FloatSize fl() const
		{
			return FloatSize(w.fl(), h.fl());
		}

		bool isNull() const
		{
			return w.isNull() && h.isNull();
		}

		/**
			Text-Serialisation:
				Two reads of SizeParam splitted by ';'
				p300;p200
		*/

		bool fromString(const std::string& str);

		void toString(std::string& output) const;
	};

	class SizeDescription
	{
	public:
		Dimension size;

		Dimension minSize;
		Dimension maxSize;
	};

} // namespace MyGUI

#endif // __MYGUI_SIZE_DESCRIPTION_H__
