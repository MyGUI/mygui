/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ALIGN_H_
#define MYGUI_ALIGN_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Diagnostic.h"
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
			return HCenter == (mValue & HStretch);
		}

		bool isVCenter() const
		{
			return VCenter == (mValue & VStretch);
		}

		bool isCenter() const
		{
			return Center == (mValue & Stretch);
		}

		bool isLeft() const
		{
			return Left == (mValue & HStretch);
		}

		bool isRight() const
		{
			return Right == (mValue & HStretch);
		}

		bool isHStretch() const
		{
			return HStretch == (mValue & HStretch);
		}

		bool isTop() const
		{
			return Top == (mValue & VStretch);
		}

		bool isBottom() const
		{
			return Bottom == (mValue & VStretch);
		}

		bool isVStretch() const
		{
			return VStretch == (mValue & VStretch);
		}

		bool isStretch() const
		{
			return Stretch == (mValue & Stretch);
		}

		bool isDefault() const
		{
			return Default == (mValue & Stretch);
		}

		Align& operator|=(Align const& _other)
		{
			mValue = (mValue | _other.mValue).mValue;
			return *this;
		}

		friend Align operator|(Enum const& a, Enum const& b)
		{
			return {Enum((unsigned int)a | (unsigned int)b)};
		}

		friend Align operator|(Align const& a, Align const& b)
		{
			return a.mValue | b.mValue;
		}

		friend bool operator==(Align const& a, Align const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator!=(Align const& a, Align const& b)
		{
			return a.mValue != b.mValue;
		}

		using MapAlign = std::map<std::string, Align>;

		static Align parse(std::string_view _value);

		std::string print() const;

		friend std::ostream& operator<<(std::ostream& _stream, const Align& _value)
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, Align& _value)
		{
			_value.mValue = Enum(0);
			std::string value;
			_stream >> value;

			const MapAlign& map_names = Align::getValueNames();
			auto iter = map_names.find(value);
			if (iter != map_names.end())
				_value |= iter->second;

			if (!_stream.eof())
			{
				std::string value2;
				_stream >> value2;
				iter = map_names.find(value2);
				if (iter != map_names.end())
					_value |= iter->second;
			}

			return _stream;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		static const MapAlign& getValueNames();

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_ALIGN_H_
