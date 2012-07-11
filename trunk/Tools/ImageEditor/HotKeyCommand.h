/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __HOT_KEY_COMMAND_H__
#define __HOT_KEY_COMMAND_H__

#include <MyGUI.h>

namespace tools
{

	class HotKeyCommand
	{
	public:
		HotKeyCommand() :
			mPressed(true),
			mShift(false),
			mControl(false),
			mKey(MyGUI::KeyCode::None)
		{
		}

		bool getPressed() const
		{
			return mPressed;
		}
		void setPressed(bool _value)
		{
			mPressed = _value;
		}

		bool getShift() const
		{
			return mShift;
		}
		void setShift(bool _value)
		{
			mShift = _value;
		}

		bool getControl() const
		{
			return mControl;
		}
		void setControl(bool _value)
		{
			mControl = _value;
		}

		MyGUI::KeyCode getKey() const
		{
			return mKey;
		}
		void setKey(MyGUI::KeyCode _value)
		{
			mKey = _value;
		}

		const MyGUI::UString& getCommand() const
		{
			return mCommand;
		}
		void setCommand(const MyGUI::UString& _value)
		{
			mCommand = _value;
		}

	private:
		bool mPressed;
		bool mShift;
		bool mControl;
		MyGUI::KeyCode mKey;
		MyGUI::UString mCommand;
	};

} // namespace tools

#endif // __HOT_KEY_COMMAND_H__
