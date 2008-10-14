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

	/*#define IS_ALIGN_HCENTER(val) (ALIGN_HCENTER == ((val).align & ALIGN_HSTRETCH))
	#define IS_ALIGN_VCENTER(val) (ALIGN_VCENTER == ((val).align & ALIGN_VSTRETCH))
	#define IS_ALIGN_CENTER(val) (ALIGN_CENTER == ((val).align & ALIGN_STRETCH))

	#define IS_ALIGN_LEFT(val) (ALIGN_LEFT == ((val).align & ALIGN_HSTRETCH))
	#define IS_ALIGN_RIGHT(val) (ALIGN_RIGHT == ((val).align & ALIGN_HSTRETCH))
	#define IS_ALIGN_HSTRETCH(val) (ALIGN_HSTRETCH == ((val).align & ALIGN_HSTRETCH))

	#define IS_ALIGN_TOP(val) (ALIGN_TOP == ((val).align & ALIGN_VSTRETCH))
	#define IS_ALIGN_BOTTOM(val) (ALIGN_BOTTOM == ((val).align & ALIGN_VSTRETCH))
	#define IS_ALIGN_VSTRETCH(val) (ALIGN_VSTRETCH == ((val).align & ALIGN_VSTRETCH))

	#define IS_ALIGN_STRETCH(val) (ALIGN_STRETCH == ((val).align & ALIGN_STRETCH))
	#define IS_ALIGN_DEFAULT(val) (ALIGN_DEFAULT == ((val).align & ALIGN_STRETCH))

	#define IS_ALIGN_LEFT_TOP(val) (ALIGN_LEFT_TOP == ((val).align & ALIGN_STRETCH))
	#define IS_ALIGN_RIGHT_TOP(val) (ALIGN_RIGHT_TOP == ((val).align & ALIGN_STRETCH))
	#define IS_ALIGN_RIGHT_BOTTOM(val) (ALIGN_RIGHT_BOTTOM == ((val).align & ALIGN_STRETCH))
	#define IS_ALIGN_LEFT_BOTTOM(val) (ALIGN_LEFT_BOTTOM == ((val).align & ALIGN_STRETCH))*/


	struct _MyGUIExport Align
	{
		enum
		{
			HCenter = MYGUI_FLAG_NONE,
			VCenter = MYGUI_FLAG_NONE,
			Center = HCenter | VCenter,

			Left = MYGUI_FLAG(1),
			Right = MYGUI_FLAG(2),
			HStretch = Left | Right,

			Top = MYGUI_FLAG(3),
			Bottom = MYGUI_FLAG(4),
			VStretch = Top | Bottom,

			Stretch = HStretch | VStretch,
			Default = Left | Top,

			LeftTop = Left | Top,
			RightTop = Right | Top,
			RightBottom = Right | Bottom,
			LeftBottom = Left | Bottom
		};

		Align() : align(Default) { }
		Align(int _align) : align(_align) { }
		explicit Align(const std::string & _parse) : align(Align::parse(_parse).align) { }

		Align & operator |= (Align const& _other)
		{
			align |= _other.align;
			return *this;
		}

		friend bool operator == (Align const & a, Align const & b) { return a.align == b.align; }
		friend bool operator != (Align const & a, Align const & b) { return a.align != b.align; }

        friend std::ostream& operator << ( std::ostream& _stream, const Align &  _value )
        {
			_stream << _value.print();
            return _stream;
        }

        friend std::istream& operator >> ( std::istream& _stream, Align &  _value )
        {
			std::string parse;
            _stream >> parse;
			if (_stream.fail()) _value = Align::Default;
			else _value = Align::parse(parse);
            return _stream;
        }

		bool isHCenter() { return HCenter == (align & HStretch); }
		bool isVCenter() { return VCenter == (align & VStretch); }
		bool isCenter() { return Center == (align & Stretch); }

		bool isLeft() { return Left == (align & HStretch); }
		bool isRight() { return Right == (align & HStretch); }
		bool isHStretch() { return HStretch == (align & HStretch); }

		bool isTop() { return Top == (align & VStretch); }
		bool isBottom() { return (Bottom == (align & VStretch)); }
		bool isVStretch() { return (VStretch == (align & VStretch)); }

		bool isStretch() { return (Stretch == (align & Stretch)); }
		bool isDefault() { return (Default == (align & Stretch)); }

		bool isLeftTop() { return (LeftTop == (align & Stretch)); }
		bool isRightTop() { return (RightTop == (align & Stretch)); }
		bool isRightBottom() { return (RightBottom == (align & Stretch)); }
		bool isLeftBottom() { return (LeftBottom == (align & Stretch)); }

		static Align parse(const std::string & _value);
		std::string print() const;

		int align;

	private:
		typedef std::map<std::string, Align> MapAlign;
		static MapAlign mMapAlign;

		static void initialise();
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
