/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _c51113dc_3e4d_4f5c_8ef4_7521ea4edfb3_
#define _c51113dc_3e4d_4f5c_8ef4_7521ea4edfb3_

#include <MyGUI.h>

namespace tools
{

	class MYGUI_EXPORT_DLL HotKeyCommand
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

}

#endif
