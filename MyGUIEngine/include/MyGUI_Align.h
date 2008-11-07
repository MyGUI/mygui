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

	enum MYGUI_OBSOLETE("use Align")
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

	struct _MyGUIExport Align
	{
		enum
		{
			HCenter = MYGUI_FLAG_NONE,		/**< centre horizontally */
			VCenter = MYGUI_FLAG_NONE,		/**< centre vertically */
			Center = HCenter | VCenter,		/**< center in the dead center */

			Left = MYGUI_FLAG(1),			/**< align from the left (and center vertically) */
			Right = MYGUI_FLAG(2),			/**< align from the right (and center vertically) */
			HStretch = Left | Right,		/**< stretch horizontally proportionate to parent window (and center vertically) */

			Top = MYGUI_FLAG(3),			/**< align from the top (and center horizontally) */
			Bottom = MYGUI_FLAG(4),			/**< align from the bottom (and center horizontally) */
			VStretch = Top | Bottom,		/**< stretch vertically proportionate to parent window (and center horizontally) */

			Stretch = HStretch | VStretch,	/**< stretch proportionate to parent window */
			Default = Left | Top,			/**< default align (align from left and top) */

			LeftTop = Left | Top,			/**< align from left and top */
			RightTop = Right | Top,			/**< align from right and top */
			RightBottom = Right | Bottom,	/**< align from right and bottom */
			LeftBottom = Left | Bottom		/**< align from left and bottom */
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
