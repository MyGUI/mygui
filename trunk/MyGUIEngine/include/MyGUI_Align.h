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

	/** Old aligns */
	enum MYGUI_OBSOLETE_START("use Align")
	{
		ALIGN_HCENTER					= MYGUI_FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= MYGUI_FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center

		ALIGN_LEFT						= MYGUI_FLAG(1),						// value from the left (and center the window vertically)
		ALIGN_RIGHT					= MYGUI_FLAG(2),						// value from the left (and center the window vertically)
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch horizontally proportionate to parent window

		ALIGN_TOP						= MYGUI_FLAG(3),						// value from the top (and center the window horizontally)
		ALIGN_BOTTOM					= MYGUI_FLAG(4),						// value from the bottom (and center the window horizontally)
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch vertically proportionate to parent window

		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch proportionate to parent window
		ALIGN_DEFAULT					= ALIGN_LEFT | ALIGN_TOP,

		ALIGN_LEFT_TOP             = ALIGN_LEFT | ALIGN_TOP,
		ALIGN_RIGHT_TOP             = ALIGN_RIGHT | ALIGN_TOP,
		ALIGN_RIGHT_BOTTOM             = ALIGN_RIGHT | ALIGN_BOTTOM,
		ALIGN_LEFT_BOTTOM             = ALIGN_LEFT | ALIGN_BOTTOM
	} MYGUI_OBSOLETE_END ;

	class _MyGUIExport Align
	{
	public:
		enum
		{
			HCenter = MYGUI_FLAG_NONE,		/**< centre horizontally */
			VCenter = MYGUI_FLAG_NONE,		/**< centre vertically */
			Center = HCenter | VCenter,		/**< center in the dead center */

			Left = MYGUI_FLAG(1),			/**< value from the left (and center vertically) */
			Right = MYGUI_FLAG(2),			/**< value from the right (and center vertically) */
			HStretch = Left | Right,		/**< stretch horizontally proportionate to parent window (and center vertically) */

			Top = MYGUI_FLAG(3),			/**< value from the top (and center horizontally) */
			Bottom = MYGUI_FLAG(4),			/**< value from the bottom (and center horizontally) */
			VStretch = Top | Bottom,		/**< stretch vertically proportionate to parent window (and center horizontally) */

			Stretch = HStretch | VStretch,	/**< stretch proportionate to parent window */
			Default = Left | Top,			/**< default value (value from left and top) */

			LeftTop = Left | Top,			/**< value from left and top */
			RightTop = Right | Top,			/**< value from right and top */
			RightBottom = Right | Bottom,	/**< value from right and bottom */
			LeftBottom = Left | Bottom		/**< value from left and bottom */
		};

		Align() : value(Default) { }
		Align(int _value) : value(_value) { }
		explicit Align(const std::string & _parse) : value(Align::parse(_parse).value) { }

		Align & operator |= (Align const& _other)
		{
			value |= _other.value;
			return *this;
		}

		friend bool operator == (Align const & a, Align const & b) { return a.value == b.value; }
		friend bool operator != (Align const & a, Align const & b) { return a.value != b.value; }

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

		bool isHCenter() { return HCenter == (value & HStretch); }
		bool isVCenter() { return VCenter == (value & VStretch); }
		bool isCenter() { return Center == (value & Stretch); }

		bool isLeft() { return Left == (value & HStretch); }
		bool isRight() { return Right == (value & HStretch); }
		bool isHStretch() { return HStretch == (value & HStretch); }

		bool isTop() { return Top == (value & VStretch); }
		bool isBottom() { return (Bottom == (value & VStretch)); }
		bool isVStretch() { return (VStretch == (value & VStretch)); }

		bool isStretch() { return (Stretch == (value & Stretch)); }
		bool isDefault() { return (Default == (value & Stretch)); }

		bool isLeftTop() { return (LeftTop == (value & Stretch)); }
		bool isRightTop() { return (RightTop == (value & Stretch)); }
		bool isRightBottom() { return (RightBottom == (value & Stretch)); }
		bool isLeftBottom() { return (LeftBottom == (value & Stretch)); }

		static Align parse(const std::string & _value);
		std::string print() const;

		int value;

	private:
		typedef std::map<std::string, Align> MapAlign;
		static MapAlign mMapAlign;
		static void initialise();
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
