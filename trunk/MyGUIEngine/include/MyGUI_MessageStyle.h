/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_MESSAGE_STYLE_H__
#define __MYGUI_MESSAGE_STYLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{


	struct MYGUI_EXPORT MessageStyle
	{

		enum Enum
		{
			None = MYGUI_FLAG_NONE,
			Ok = MYGUI_FLAG(0),
			Yes = MYGUI_FLAG(1),
			No = MYGUI_FLAG(2),
			Abort = MYGUI_FLAG(3),
			Retry = MYGUI_FLAG(4),
			Ignore = MYGUI_FLAG(5),
			Cancel = MYGUI_FLAG(6),
			Try = MYGUI_FLAG(7),
			Continue = MYGUI_FLAG(8),

			_IndexUserButton1 = 9, // ������ ������ ������ �����

			Button1 = MYGUI_FLAG(_IndexUserButton1),
			Button2 = MYGUI_FLAG(_IndexUserButton1 + 1),
			Button3 = MYGUI_FLAG(_IndexUserButton1 + 2),
			Button4 = MYGUI_FLAG(_IndexUserButton1 + 3),

			_CountUserButtons = 4, // ����������� ������ �����
			_IndexIcon1 = _IndexUserButton1 + _CountUserButtons, // ������ ������ ������

			IconDefault = MYGUI_FLAG(_IndexIcon1),

			IconInfo = MYGUI_FLAG(_IndexIcon1),
			IconQuest = MYGUI_FLAG(_IndexIcon1 + 1),
			IconError = MYGUI_FLAG(_IndexIcon1 + 2),
			IconWarning = MYGUI_FLAG(_IndexIcon1 + 3),

			Icon1 = MYGUI_FLAG(_IndexIcon1),
			Icon2 = MYGUI_FLAG(_IndexIcon1 + 1),
			Icon3 = MYGUI_FLAG(_IndexIcon1 + 2),
			Icon4 = MYGUI_FLAG(_IndexIcon1 + 3),
			Icon5 = MYGUI_FLAG(_IndexIcon1 + 4),
			Icon6 = MYGUI_FLAG(_IndexIcon1 + 5),
			Icon7 = MYGUI_FLAG(_IndexIcon1 + 6),
			Icon8 = MYGUI_FLAG(_IndexIcon1 + 7)
		};

		MessageStyle(Enum _value = None) : value(_value) { }
		explicit MessageStyle(int _value) : value(_value) { }

		MessageStyle & operator |= (MessageStyle const& _other) { value |= _other.value; return *this; }
		friend Enum operator | (Enum const & a, Enum const & b) { return Enum((int)a | (int)b); }
		friend MessageStyle operator | (MessageStyle const & a, MessageStyle const & b) { return MessageStyle((int)a.value | (int)b.value); }

		friend bool operator == (MessageStyle const & a, MessageStyle const & b) { return a.value == b.value; }
		friend bool operator != (MessageStyle const & a, MessageStyle const & b) { return a.value != b.value; }

		// ���������� ������ ������
		size_t getIconIndex()
		{
			size_t index = 0;
			int num = value >> _IndexIcon1;

			while (num != 0)
			{
				if ((num & 1) == 1) return index;

				++index;
				num >>= 1;
			}

			return ITEM_NONE;
		}

		// ���������� ������ ������
		std::vector<MessageStyle> getButtons()
		{
			std::vector<MessageStyle> buttons;

			size_t index = 0;
			int num = value;
			while (index < _IndexIcon1)
			{
				if ((num & 1) == 1)
				{
					buttons.push_back( MessageStyle( MYGUI_FLAG(index) ) );
				}

				++index;
				num >>= 1;
			}

			return buttons;
		}

		typedef std::map<std::string, int> MapAlign;

		static MessageStyle parse(const std::string & _value)
		{
			MessageStyle result(0);
			const MapAlign & map_names = result.getValueNames();
			const std::vector<std::string> & vec = utility::split(_value);
			for (size_t pos=0; pos<vec.size(); pos++) {
				MapAlign::const_iterator iter = map_names.find(vec[pos]);
				if (iter != map_names.end()) result.value |= iter->second;
				else { MYGUI_LOG(Warning, "Cannot parse type '" << vec[pos] << "'"); }
			}
			return result;
		}

		int toValue() { return value; }

	private:
		const MapAlign & getValueNames()
		{
			static MapAlign map_names;

			if (map_names.empty()) {
				MYGUI_REGISTER_VALUE(map_names, None);
				MYGUI_REGISTER_VALUE(map_names, Ok);
				MYGUI_REGISTER_VALUE(map_names, Yes);
				MYGUI_REGISTER_VALUE(map_names, No);
				MYGUI_REGISTER_VALUE(map_names, Abort);
				MYGUI_REGISTER_VALUE(map_names, Retry);
				MYGUI_REGISTER_VALUE(map_names, Ignore);
				MYGUI_REGISTER_VALUE(map_names, Cancel);
				MYGUI_REGISTER_VALUE(map_names, Try);
				MYGUI_REGISTER_VALUE(map_names, Continue);

				MYGUI_REGISTER_VALUE(map_names, Button1);
				MYGUI_REGISTER_VALUE(map_names, Button2);
				MYGUI_REGISTER_VALUE(map_names, Button3);
				MYGUI_REGISTER_VALUE(map_names, Button4);

				MYGUI_REGISTER_VALUE(map_names, IconDefault);

				MYGUI_REGISTER_VALUE(map_names, IconInfo);
				MYGUI_REGISTER_VALUE(map_names, IconQuest);
				MYGUI_REGISTER_VALUE(map_names, IconError);
				MYGUI_REGISTER_VALUE(map_names, IconWarning);

				MYGUI_REGISTER_VALUE(map_names, Icon1);
				MYGUI_REGISTER_VALUE(map_names, Icon2);
				MYGUI_REGISTER_VALUE(map_names, Icon3);
				MYGUI_REGISTER_VALUE(map_names, Icon4);
				MYGUI_REGISTER_VALUE(map_names, Icon5);
				MYGUI_REGISTER_VALUE(map_names, Icon6);
				MYGUI_REGISTER_VALUE(map_names, Icon7);
				MYGUI_REGISTER_VALUE(map_names, Icon8);
			}

			return map_names;
		}

	private:
		int value;
	};

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_STYLE_H__
