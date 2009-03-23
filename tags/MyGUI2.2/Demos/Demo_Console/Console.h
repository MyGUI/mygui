/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	typedef MyGUI::delegates::CDelegate2<const Ogre::UTFString &, const Ogre::UTFString &> CommandDelegate;

	namespace formates
	{
		template<typename T> inline std::string format() { return MyGUI::utility::toString("[ ", std::numeric_limits<T>::min(), " | ", std::numeric_limits<T>::max(), " ]"); }
		template<> inline std::string format<bool>() { return "[ true | false ]"; }
		template<> inline std::string format<float>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<float>::max(), " | ", std::numeric_limits<float>::max(), " ]"); }
		template<> inline std::string format<double>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<double>::max(), " | ", std::numeric_limits<double>::max(), " ]"); }
	}

	class Console : public wraps::BaseLayout
	{
	public:
		static Console * getInstancePtr();
		static Console & getInstance();

		Console();
		virtual ~Console();

		void addToConsole(const Ogre::UTFString & _line);
		void addToConsole(const Ogre::UTFString & _reason, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		{
			addToConsole(MyGUI::utility::toString(_reason, "'", _key, " ", _value, "'"));
		}

		void clearConsole();

		/** Method : add command.\n
			@example Add_console_command
			@code
				registerConsoleDelegate("your_command_1", MyGUI::newDelegate(your_func));
				registerConsoleDelegate("your_command_2", MyGUI::newDelegate(your_static_method));
				registerConsoleDelegate("your_command_3", MyGUI::newDelegate(your_class_ptr, &your_class_name::your_method_name));
			@endcode

			signature your method : void method(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		void registerConsoleDelegate(const Ogre::UTFString & _command, CommandDelegate::IDelegate * _delegate);

		/** Event : Unknown command.\n
			signature : void method(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
		*/
		CommandDelegate eventConsoleUnknowCommand;

		const Ogre::UTFString & getConsoleStringCurrent() { return mStringCurrent; }
		const Ogre::UTFString & getConsoleStringError() { return mStringError; }
		const Ogre::UTFString & getConsoleStringSuccess() { return mStringSuccess; }
		const Ogre::UTFString & getConsoleStringUnknow() { return mStringUnknow; }
		const Ogre::UTFString & getConsoleStringFormat() { return mStringFormat; }

		bool isVisible() { return mMainWidget->isVisible(); }
		void setVisible(bool _visible) { mMainWidget->setVisible(_visible); }

		template <typename T> bool isAction(T & _result, const Ogre::UTFString & _key, const Ogre::UTFString & _value, const Ogre::UTFString & _format = "")
		{
			if (_value.empty()) {
				addToConsole(getConsoleStringCurrent(), _key, MyGUI::utility::toString(_result));
			}
			else {
				if ( ! MyGUI::utility::parseComplex(_value, _result)) {
					addToConsole(getConsoleStringError(), _key, _value);
					addToConsole(getConsoleStringFormat(), _key, _format.empty() ? formates::format<T>() : _format);
				}
				else {
					addToConsole(getConsoleStringSuccess(), _key, _value);
					return true;
				}
			}
			return false;
		}

	private:
		void notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string & _button);

		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index);
		void notifyButtonPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		void internalCommand(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value);

	private:
		MyGUI::EditPtr mListHistory;
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

		static Console * m_instance;
	};

} // namespace demo

#endif // __CONSOLE_H__
