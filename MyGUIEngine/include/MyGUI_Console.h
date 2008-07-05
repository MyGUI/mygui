/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __MYGUI_CONSOLE_H__
#define __MYGUI_CONSOLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

	typedef delegates::CDelegate3<WidgetPtr,  const Ogre::UTFString &, const Ogre::UTFString &> CommandDelegate;

	class _MyGUIExport Console : public Window
	{
		// для вызова закрытого конструктора
		friend class factory::ConsoleFactory;

	protected:
		Console(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }


		void showConsole();
		void hideConsole();
		inline void showConsole(bool _show) { _show ? showConsole() : hideConsole(); }

		inline bool isShowConsole() { return isShow(); }

		void addToConsole(const Ogre::UTFString & _line);
		inline void addToConsole(const Ogre::UTFString & _reason, const Ogre::UTFString & _key, const Ogre::UTFString & _value) {
			addToConsole(utility::toString(_reason, "'", _key, " ", _value, "'"));
		}

		void clearConsole();
		void printCommandFormat(const Ogre::UTFString & _command);

		const Ogre::UTFString & getCommandFormat(const Ogre::UTFString & _command);

		/** Method : add command.\n
			example :
				registerConsoleDelegate("your_command_1", newDelegate(your_func));
				registerConsoleDelegate("your_command_2", newDelegate(your_static_method));
				registerConsoleDelegate("your_command_3", newDelegate(your_class_ptr, &your_class_name::your_method_name));

			signature your method : void method(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		void registerConsoleDelegate(const Ogre::UTFString & _command, delegates::IDelegate3<WidgetPtr, const Ogre::UTFString &, const Ogre::UTFString &> * _delegate, const Ogre::UTFString & _format = "");

		/** Event : Unknow command.\n
			signature : void method(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		CommandDelegate eventConsoleUnknowCommand;

		const Ogre::UTFString & getConsoleStringCurrent() { return mStringCurrent; }
		const Ogre::UTFString & getConsoleStringError() { return mStringError; }
		const Ogre::UTFString & getConsoleStringSuccess() { return mStringSuccess; }
		const Ogre::UTFString & getConsoleStringUnknow() { return mStringUnknow; }
		const Ogre::UTFString & getConsoleStringFormat() { return mStringFormat; }

	private:
		void notifyMouseButtonClick(WidgetPtr _sender);
		void notifyComboAccept(WidgetPtr _sender);
		void notifyWindowButtonPressed(WidgetPtr _sender, const std::string & _button);

		void internalCommand(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value);

	private:
		ButtonPtr mSendButton;
		ListPtr mCommandList;
		ComboBoxPtr mCommandInput;

		typedef std::map<Ogre::UTFString, CommandDelegate> MapDelegate;
		MapDelegate mDelegates;

		typedef std::map<Ogre::UTFString, Ogre::UTFString> MapFormat;
		MapFormat mFormates;

		Ogre::UTFString mStringCurrent;
		Ogre::UTFString mStringError;
		Ogre::UTFString mStringSuccess;
		Ogre::UTFString mStringUnknow;
		Ogre::UTFString mStringFormat;

	}; // class _MyGUIExport Console : public Widget

} // namespace MyGUI

#endif // __MYGUI_CONSOLE_H__
