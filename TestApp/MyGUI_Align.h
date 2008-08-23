/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_ALIGN_H__
#define __MYGUI_ALIGN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct Align2
	{
		typedef std::map<std::string, Align2> MapAlign2;

		inline Align2() : align(ALIGN_DEFAULT) { }
		inline Align2(int _align) : align(_align) { }
		inline Align2(Align2 const& _other) : align(_other.align) { }
		inline explicit Align2(const std::string & _parse) : align(Align2::parse(_parse).align) { }

		inline Align2 & operator = (Align2 const& _other)
		{
			align = _other.align;
			return *this;
		}

		inline Align2 & operator |= (Align2 const& _other)
		{
			align |= _other.align;
			return *this;
		}

		inline bool operator==( Align2 const & o ) const
		{
			return align == o.align;
		}

		inline bool operator!=( Align2 const & o ) const
		{
			return align != o.align;
		}

        static friend int& operator = ( int& _l, const Align2 &  _value )
        {
            return _value.align;
        }

        inline friend std::ostream& operator << ( std::ostream& _stream, const Align2 &  _value )
        {
			_stream << _value.print();
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, Align2 &  _value )
        {
			std::string parse;
            _stream >> parse;
			if (_stream.fail()) _value = Align2::Default;
			else _value = Align2::parse(parse);
            return _stream;
        }

		inline bool isHCenter() { return IS_ALIGN_HCENTER(align); }
		inline bool isVCenter() { return IS_ALIGN_VCENTER(align); }
		inline bool isCenter() { return IS_ALIGN_CENTER(align); }
		inline bool isCenterParent() { return IS_ALIGN_CENTER_PARENT(align); }
		inline bool isLeft() { return IS_ALIGN_LEFT(align); }
		inline bool isRight() { return IS_ALIGN_RIGHT(align); }
		inline bool isHStretch() { return IS_ALIGN_HSTRETCH(align); }
		inline bool isTop() { return IS_ALIGN_TOP(align); }
		inline bool isBottom() { return IS_ALIGN_BOTTOM(align); }
		inline bool isVStretch() { return IS_ALIGN_VSTRETCH(align); }
		inline bool isStretch() { return IS_ALIGN_STRETCH(align); }
		inline bool isDefaut() { return IS_ALIGN_DEFAULT(align); }
		inline bool isLeftTop() { return IS_ALIGN_LEFT_TOP(align); }
		inline bool isRightTop() { return IS_ALIGN_RIGHT_TOP(align); }
		inline bool isRightBottom() { return IS_ALIGN_RIGHT_BOTTOM(align); }
		inline bool isLeftBottom() { return IS_ALIGN_LEFT_BOTTOM(align); }

		static Align2 parse(const std::string & _value);
		std::string print() const;

		enum {
			HCenter = ALIGN_HCENTER,
			VCenter = ALIGN_VCENTER,
			Center = ALIGN_CENTER,
			CenterParent = ALIGN_CENTER_PARENT,

			Left = ALIGN_LEFT,
			Right = ALIGN_RIGHT,
			HStretch = ALIGN_HSTRETCH,
			Top = ALIGN_TOP,
			Bottom = ALIGN_BOTTOM,
			VStretch = ALIGN_VSTRETCH,
			Stretch = ALIGN_STRETCH,
			Default = ALIGN_DEFAULT,

			LeftTop = ALIGN_LEFT_TOP,
			RightTop = ALIGN_RIGHT_TOP,
			RightBottom = ALIGN_RIGHT_BOTTOM,
			LeftBottom = ALIGN_LEFT_BOTTOM
		};

		int align;

	private:
		static void initialise();
		static MapAlign2 mMapAlign;
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__