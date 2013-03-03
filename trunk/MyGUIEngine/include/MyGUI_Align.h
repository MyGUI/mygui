/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_ALIGN_H__
#define __MYGUI_ALIGN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_StringUtility.h"
#include <map>

namespace MyGUI
{

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
		};

		Align(Enum _value = Default) :
			mValue(_value)
		{
		}

		bool isHCenter() const
		{
			return HCenter == (mValue & ((int)HStretch));
		}

		bool isVCenter() const
		{
			return VCenter == (mValue & ((int)VStretch));
		}

		bool isCenter() const
		{
			return Center == (mValue & ((int)Stretch));
		}

		bool isLeft() const
		{
			return Left == (mValue & ((int)HStretch));
		}

		bool isRight() const
		{
			return Right == (mValue & ((int)HStretch));
		}

		bool isHStretch() const
		{
			return HStretch == (mValue & ((int)HStretch));
		}

		bool isTop() const
		{
			return Top == (mValue & ((int)VStretch));
		}

		bool isBottom() const
		{
			return (Bottom == (mValue & ((int)VStretch)));
		}

		bool isVStretch() const
		{
			return (VStretch == (mValue & ((int)VStretch)));
		}

		bool isStretch() const
		{
			return (Stretch == (mValue & ((int)Stretch)));
		}

		bool isDefault() const
		{
			return (Default == (mValue & ((int)Stretch)));
		}

		Align& operator |= (Align const& _other)
		{
			mValue = Enum(int(mValue) | int(_other.mValue));
			return *this;
		}

		friend Align operator | (Enum const& a, Enum const& b)
		{
			return Align(Enum(int(a) | int(b)));
		}

		friend Align operator | (Align const& a, Align const& b)
		{
			return Align(Enum(int(a.mValue) | int(b.mValue)));
		}

		friend bool operator == (Align const& a, Align const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (Align const& a, Align const& b)
		{
			return a.mValue != b.mValue;
		}

		typedef std::map<std::string, int> MapAlign;

		static Align parse(const std::string& _value)
		{
			Align result(Enum(0));
			const MapAlign& map_names = result.getValueNames();
			const std::vector<std::string>& vec = utility::split(_value);
			for (size_t pos = 0; pos < vec.size(); pos++)
			{
				MapAlign::const_iterator iter = map_names.find(vec[pos]);
				if (iter != map_names.end())
				{
					result.mValue = Enum(int(result.mValue) | int(iter->second));
				}
			}
			return result;
		}

		std::string print() const
		{
			std::string result;

			if (mValue & Left)
			{
				if (mValue & Right)
					result = "HStretch";
				else
					result = "Left";
			}
			else if (mValue & Right)
				result = "Right";
			else
				result = "HCenter";

			if (mValue & Top)
			{
				if (mValue & Bottom)
					result += " VStretch";
				else
					result += " Top";
			}
			else if (mValue & Bottom)
				result += " Bottom";
			else
				result += " VCenter";

			return result;
		}

		friend std::ostream& operator << ( std::ostream& _stream, const Align&  _value )
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, Align&  _value )
		{
			_value.mValue = Enum(0);
			std::string value;
			_stream >> value;

			const MapAlign& map_names = _value.getValueNames();
			MapAlign::const_iterator iter = map_names.find(value);
			if (iter != map_names.end())
				_value.mValue = Enum(int(_value.mValue) | int(iter->second));

			if (!_stream.eof())
			{
				std::string value2;
				_stream >> value2;
				iter = map_names.find(value2);
				if (iter != map_names.end())
					_value.mValue = Enum(int(_value.mValue) | int(iter->second));
			}

			return _stream;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		const MapAlign& getValueNames() const
		{
			static MapAlign map_names;

			if (map_names.empty())
			{
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
			}

			return map_names;
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
