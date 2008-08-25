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

	enum ALIGN_INFO
	{
		ALIGN_HCENTER					= MYGUI_FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= MYGUI_FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center
		ALIGN_CENTER_PARENT		    = MYGUI_FLAG(0),						// do not use it in your application это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения

		ALIGN_LEFT						= MYGUI_FLAG(1),						// align from the left (and center the window vertically)
		ALIGN_RIGHT					= MYGUI_FLAG(2),						// align from the left (and center the window vertically)
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch horizontally proportionate to parent window
		ALIGN_TOP						= MYGUI_FLAG(3),						// align from the top (and center the window horizontally)
		ALIGN_BOTTOM					= MYGUI_FLAG(4),						// align from the bottom (and center the window horizontally)
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch vertically proportionate to parent window
		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch proportionate to parent window
		ALIGN_DEFAULT					= ALIGN_LEFT | ALIGN_TOP,

		ALIGN_LEFT_TOP             = ALIGN_LEFT | ALIGN_TOP,
		ALIGN_RIGHT_TOP             = ALIGN_RIGHT | ALIGN_TOP,
		ALIGN_RIGHT_BOTTOM             = ALIGN_RIGHT | ALIGN_BOTTOM,
		ALIGN_LEFT_BOTTOM             = ALIGN_LEFT | ALIGN_BOTTOM,
	};

	#define IS_ALIGN_HCENTER(val) (ALIGN_HCENTER == ((val).align & (ALIGN_LEFT | ALIGN_RIGHT)))
	#define IS_ALIGN_VCENTER(val) (ALIGN_VCENTER == ((val).align & (ALIGN_TOP | ALIGN_BOTTOM)))
	#define IS_ALIGN_CENTER(val) (ALIGN_CENTER == ((val).align & (ALIGN_LEFT | ALIGN_RIGHT | ALIGN_TOP | ALIGN_BOTTOM)))
	#define IS_ALIGN_CENTER_PARENT(val) (ALIGN_CENTER_PARENT == ((val).align & ALIGN_CENTER_PARENT))

	#define IS_ALIGN_LEFT(val) (ALIGN_LEFT == ((val).align & ALIGN_LEFT))
	#define IS_ALIGN_RIGHT(val) (ALIGN_RIGHT == ((val).align & ALIGN_RIGHT))
	#define IS_ALIGN_HSTRETCH(val) (ALIGN_HSTRETCH == ((val).align & ALIGN_HSTRETCH))

	#define IS_ALIGN_TOP(val) (ALIGN_TOP == ((val).align & ALIGN_TOP))
	#define IS_ALIGN_BOTTOM(val) (ALIGN_BOTTOM == ((val).align & ALIGN_BOTTOM))
	#define IS_ALIGN_VSTRETCH(val) (ALIGN_VSTRETCH == ((val).align & ALIGN_VSTRETCH))

	#define IS_ALIGN_STRETCH(val) (ALIGN_STRETCH == ((val).align & ALIGN_STRETCH))
	#define IS_ALIGN_DEFAULT(val) (ALIGN_DEFAULT == ((val).align & ALIGN_DEFAULT))

	#define IS_ALIGN_LEFT_TOP(val) (ALIGN_LEFT_TOP == ((val).align & ALIGN_LEFT_TOP))
	#define IS_ALIGN_RIGHT_TOP(val) (ALIGN_RIGHT_TOP == ((val).align & ALIGN_RIGHT_TOP))
	#define IS_ALIGN_RIGHT_BOTTOM(val) (ALIGN_RIGHT_BOTTOM == ((val).align & ALIGN_RIGHT_BOTTOM))
	#define IS_ALIGN_LEFT_BOTTOM(val) (ALIGN_LEFT_BOTTOM == ((val).align & ALIGN_LEFT_BOTTOM))


	struct _MyGUIExport Align
	{
		typedef std::map<std::string, Align> MapAlign;

		inline Align() : align(ALIGN_DEFAULT) { }
		inline Align(int _align) : align(_align) { }
		inline Align(Align const& _other) : align(_other.align) { }
		inline explicit Align(const std::string & _parse) : align(Align::parse(_parse).align) { }

		inline Align & operator = (Align const& _other)
		{
			align = _other.align;
			return *this;
		}

		inline Align & operator |= (Align const& _other)
		{
			align |= _other.align;
			return *this;
		}

		inline bool operator==( Align const & o ) const
		{
			return align == o.align;
		}

		inline bool operator!=( Align const & o ) const
		{
			return align != o.align;
		}

        inline friend std::ostream& operator << ( std::ostream& _stream, const Align &  _value )
        {
			_stream << _value.print();
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, Align &  _value )
        {
			std::string parse;
            _stream >> parse;
			if (_stream.fail()) _value = Align::Default;
			else _value = Align::parse(parse);
            return _stream;
        }

		inline bool isHCenter() { return IS_ALIGN_HCENTER(*this); }
		inline bool isVCenter() { return IS_ALIGN_VCENTER(*this); }
		inline bool isCenter() { return IS_ALIGN_CENTER(*this); }
		inline bool isCenterParent() { return IS_ALIGN_CENTER_PARENT(*this); }
		inline bool isLeft() { return IS_ALIGN_LEFT(*this); }
		inline bool isRight() { return IS_ALIGN_RIGHT(*this); }
		inline bool isHStretch() { return IS_ALIGN_HSTRETCH(*this); }
		inline bool isTop() { return IS_ALIGN_TOP(*this); }
		inline bool isBottom() { return IS_ALIGN_BOTTOM(*this); }
		inline bool isVStretch() { return IS_ALIGN_VSTRETCH(*this); }
		inline bool isStretch() { return IS_ALIGN_STRETCH(*this); }
		inline bool isDefault() { return IS_ALIGN_DEFAULT(*this); }
		inline bool isLeftTop() { return IS_ALIGN_LEFT_TOP(*this); }
		inline bool isRightTop() { return IS_ALIGN_RIGHT_TOP(*this); }
		inline bool isRightBottom() { return IS_ALIGN_RIGHT_BOTTOM(*this); }
		inline bool isLeftBottom() { return IS_ALIGN_LEFT_BOTTOM(*this); }

		static Align parse(const std::string & _value);
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
		static MapAlign mMapAlign;
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
