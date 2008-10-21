/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	typedef MyGUI::delegates::CDelegate2<const Ogre::UTFString &, const Ogre::UTFString &> CommandDelegate;
	typedef MyGUI::delegates::IDelegate2<const Ogre::UTFString &, const Ogre::UTFString &> * DelegatePtr;

	class Console : public wraps::BaseLayout
	{
	public:
		Console();

		virtual void initialise();

		void addToConsole(const Ogre::UTFString & _line);
		void addToConsole(const Ogre::UTFString & _reason, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		{
			addToConsole(MyGUI::utility::toString(_reason, "'", _key, " ", _value, "'"));
		}

		void clearConsole();

		/** Method : add command.\n
			example :
				registerConsoleDelegate("your_command_1", MyGUI::newDelegate(your_func));
				registerConsoleDelegate("your_command_2", MyGUI::newDelegate(your_static_method));
				registerConsoleDelegate("your_command_3", MyGUI::newDelegate(your_class_ptr, &your_class_name::your_method_name));

			signature your method : void method(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		void registerConsoleDelegate(const Ogre::UTFString & _command, DelegatePtr _delegate);

		/** Event : Unknow command.\n
			signature : void method(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		CommandDelegate eventConsoleUnknowCommand;

		const Ogre::UTFString & getConsoleStringCurrent() { return mStringCurrent; }
		const Ogre::UTFString & getConsoleStringError() { return mStringError; }
		const Ogre::UTFString & getConsoleStringSuccess() { return mStringSuccess; }
		const Ogre::UTFString & getConsoleStringUnknow() { return mStringUnknow; }
		const Ogre::UTFString & getConsoleStringFormat() { return mStringFormat; }

		bool isShow() { mainWidget()->isShow(); }
		void show() { mainWidget()->show(); }
		void hide() { mainWidget()->hide(); }

	private:
		void notifyWindowButtonPressed(MyGUI::WidgetPtr _sender, const std::string & _button);

		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyComboAccept(MyGUI::WidgetPtr _sender);
		void notifyButtonPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		void internalCommand(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value);

	private:
		MyGUI::ListPtr mListHistory;
		MyGUI::ComboBoxPtr mComboCommand;
		MyGUI::ButtonPtr mButtonSubmit;

		typedef std::map<Ogre::UTFString, CommandDelegate> MapDelegate;
		MapDelegate mDelegates;

		Ogre::UTFString mStringCurrent;
		Ogre::UTFString mStringError;
		Ogre::UTFString mStringSuccess;
		Ogre::UTFString mStringUnknow;
		Ogre::UTFString mStringFormat;

		// если текущий текст автодополнен
		bool mAutocomleted;
	};

} // namespace demo

#endif // __CONSOLE_H__
