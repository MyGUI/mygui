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
	typedef enum /*MYGUI_OBSOLETE_START("use : Align")*/
	{
		ALIGN_HCENTER = MYGUI_FLAG_NONE,
		ALIGN_VCENTER = MYGUI_FLAG_NONE,
		ALIGN_CENTER = ALIGN_HCENTER | ALIGN_VCENTER,
		ALIGN_LEFT = MYGUI_FLAG(1),
		ALIGN_RIGHT = MYGUI_FLAG(2),
		ALIGN_HSTRETCH = ALIGN_LEFT | ALIGN_RIGHT,
		ALIGN_TOP = MYGUI_FLAG(3),
		ALIGN_BOTTOM = MYGUI_FLAG(4),
		ALIGN_VSTRETCH = ALIGN_TOP | ALIGN_BOTTOM,
		ALIGN_STRETCH = ALIGN_HSTRETCH | ALIGN_VSTRETCH,
		ALIGN_DEFAULT = ALIGN_LEFT | ALIGN_TOP,
		ALIGN_LEFT_TOP = ALIGN_LEFT | ALIGN_TOP,
		ALIGN_RIGHT_TOP = ALIGN_RIGHT | ALIGN_TOP,
		ALIGN_RIGHT_BOTTOM = ALIGN_RIGHT | ALIGN_BOTTOM,
		ALIGN_LEFT_BOTTOM = ALIGN_LEFT | ALIGN_BOTTOM
	} /*MYGUI_OBSOLETE_END*/ ALIGN_TYPE_OBSOLETE;

	inline ALIGN_TYPE_OBSOLETE operator | (ALIGN_TYPE_OBSOLETE const & a, ALIGN_TYPE_OBSOLETE const & b) { return ALIGN_TYPE_OBSOLETE((int)a | (int)b); }

	struct MYGUI_EXPORT Align
	{
		enum Enum
		{
			HCenter = MYGUI_FLAG_NONE, /**< center horizontally */
			VCenter = MYGUI_FLAG_NONE, /**< center vertically */
			Center = HCenter | VCenter, /**< center in the dead center */

			Left = MYGUI_FLAG(1), /**< value from the left (and center vertically) */
			Right = MYGUI_FLAG(2), /**< value from the right (and center vertically) */
			HStretch = Left | Right, /**< stretch horizontally proportionate to parent window (and center vertically) */

			Top = MYGUI_FLAG(3), /**< value from the top (and center horizontally) */
			Bottom = MYGUI_FLAG(4), /**< value from the bottom (and center horizontally) */
			VStretch = Top | Bottom, /**< stretch vertically proportionate to parent window (and center horizontally) */

			Stretch = HStretch | VStretch, /**< stretch proportionate to parent window */
			Default = Left | Top /**< default value (value from left and top) */

			//LeftTop = Left | Top, /**< value from left and top */
			//RightTop = Right | Top, /**< value from right and top */
			//RightBottom = Right | Bottom,	 /**< value from right and bottom */
			//LeftBottom = Left | Bottom /**< value from left and bottom */
		};

		Align(Enum _value = Default) : value(_value) { }
		Align(ALIGN_TYPE_OBSOLETE _value) : value(_value) { }
		explicit Align(int _value) : value(_value) { }

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

		//bool isLeftTop() { return (LeftTop == (value & Stretch)); }
		//bool isRightTop() { return (RightTop == (value & Stretch)); }
		//bool isRightBottom() { return (RightBottom == (value & Stretch)); }
		//bool isLeftBottom() { return (LeftBottom == (value & Stretch)); }

		Align & operator |= (Align const& _other) { value |= _other.value; return *this; }
		friend Align operator | (Enum const & a, Enum const & b) { return Align((int)a | (int)b); }

		friend bool operator == (Align const & a, Align const & b) { return a.value == b.value; }
		friend bool operator != (Align const & a, Align const & b) { return a.value != b.value; }

		typedef std::map<std::string, int> MapAlign;

		static Align parse(const std::string & _value)
		{
			Align result(0);
			const MapAlign & map_names = result.getValueNames();
			const std::vector<std::string> & vec = utility::split(_value);
			for (size_t pos=0; pos<vec.size(); pos++) {
				MapAlign::const_iterator iter = map_names.find(vec[pos]);
				if (iter != map_names.end()) result.value |= iter->second;
				else { MYGUI_LOG(Warning, "Cannot parse type '" << vec[pos] << "'"); }
			}
			return result;
		}

		std::string print() const
		{
			std::string result;

			if (value & Left) {
				if (value & Right) result = "HStretch";
				else result = "Left";
			}
			else if (value & Right) result = "Right";
			else result = "HCenter";

			if (value & Top) {
				if (value & Bottom) result += " VStretch";
				else result += " Top";
			}
			else if (value & Bottom) result += " Bottom";
			else result += " VCenter";

			return result;
		}

		friend std::ostream& operator << ( std::ostream& _stream, const Align &  _value ) {
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, Align &  _value ) {
			std::string value;
			_stream >> value;
			_value = Align::parse(value);
			return _stream;
		}

		int toValue() { return value; }

	private:
		const MapAlign & getValueNames()
		{
			static MapAlign map_names;

			if (map_names.empty()) {
				// OBSOLETE
				map_names["ALIGN_HCENTER"] = HCenter;
				map_names["ALIGN_VCENTER"] = VCenter;
				map_names["ALIGN_CENTER"] = Center;
				map_names["ALIGN_LEFT"] = Left;
				map_names["ALIGN_RIGHT"] = Right;
				map_names["ALIGN_HSTRETCH"] = HStretch;
				map_names["ALIGN_TOP"] = Top;
				map_names["ALIGN_BOTTOM"] = Bottom;
				map_names["ALIGN_VSTRETCH"] = VStretch;
				map_names["ALIGN_STRETCH"] = Stretch;
				map_names["ALIGN_DEFAULT"] = Default;

				MYGUI_REGISTER_VALUE(map_names, HCenter);
				MYGUI_REGISTER_VALUE(map_names, VCenter);
				MYGUI_REGISTER_VALUE(map_names, Center);
				MYGUI_REGISTER_VALUE(map_names, Left);
				MYGUI_REGISTER_VALUE(map_names, Right);
				MYGUI_REGISTER_VALUE(map_names, HStretch);
				MYGUI_REGISTER_VALUE(map_names, Top);
				MYGUI_REGISTER_VALUE(map_names, Bottom);
				MYGUI_REGISTER_VALUE(map_names, VStretch);
				MYGUI_REGISTER_VALUE(map_names, Stretch);
				MYGUI_REGISTER_VALUE(map_names, Default);

				//MYGUI_REGISTER_VALUE(map_names, LeftTop);
				//MYGUI_REGISTER_VALUE(map_names, RightTop);
				//MYGUI_REGISTER_VALUE(map_names, RightBottom);
				//MYGUI_REGISTER_VALUE(map_names, LeftBottom);
			}

			return map_names;
		}

	private:
		int value;
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
