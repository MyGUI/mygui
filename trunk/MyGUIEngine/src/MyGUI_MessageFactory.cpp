/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_MessageFactory.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LanguageManager.h"

namespace MyGUI
{
	namespace factory
	{

		VectorUTFString MessageFactory::mVectorButtonName;
		VectorUTFString MessageFactory::mVectorButtonTag;

		MessageFactory::MessageFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			initialise();

			// регестрируем все парсеры
			manager.registerDelegate("Message_Caption") = newDelegate(this, &MessageFactory::Message_Caption);
			manager.registerDelegate("Message_Message") = newDelegate(this, &MessageFactory::Message_Message);
			manager.registerDelegate("Message_Modal") = newDelegate(this, &MessageFactory::Message_Modal);
			manager.registerDelegate("Message_Button") = newDelegate(this, &MessageFactory::Message_Button);
			manager.registerDelegate("Message_AddButton") = newDelegate(this, &MessageFactory::Message_AddButton);
			manager.registerDelegate("Message_SmoothShow") = newDelegate(this, &MessageFactory::Message_SmoothShow);
			manager.registerDelegate("Message_Fade") = newDelegate(this, &MessageFactory::Message_Fade);
		}

		MessageFactory::~MessageFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			shutdown();

			// удаляем все парсеры
			manager.unregisterDelegate("Message_Caption");
			manager.unregisterDelegate("Message_Message");
			manager.unregisterDelegate("Message_Modal");
			manager.unregisterDelegate("Message_Button");
			manager.unregisterDelegate("Message_AddButton");
			manager.unregisterDelegate("Message_SmoothShow");
			manager.unregisterDelegate("Message_Fade");
		}

		void MessageFactory::Message_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setCaption(_value);
		}

		void MessageFactory::Message_Message(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setMessageText(_value);
		}

		void MessageFactory::Message_Modal(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			if (utility::parseBool(_value)) InputManager::getInstance().addWidgetModal(_widget);
			else InputManager::getInstance().removeWidgetModal(_widget);
		}

		void MessageFactory::Message_Button(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setMessageButton(MessageBoxStyle::parse(_value));
		}

		void MessageFactory::Message_AddButton(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->addButtonName(_value);
		}

		void MessageFactory::Message_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setSmoothShow(utility::parseBool(_value));
		}

		void MessageFactory::Message_Fade(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setWindowFade(utility::parseBool(_value));
		}

		void MessageFactory::changeLanguage(const std::string & _language)
		{
			MYGUI_ASSERT(mVectorButtonName.size() == mVectorButtonTag.size(), "error mapping buttons names");
			LanguageManager & manager = LanguageManager::getInstance();
			VectorUTFString::iterator iter_name = mVectorButtonName.begin();
			for (VectorUTFString::iterator iter=mVectorButtonTag.begin(); iter!=mVectorButtonTag.end(); ++iter, ++iter_name) {
				const Ogre::UTFString & name = manager.getTag(*iter);
				if ( ! name.empty()) *iter_name = name;
			}
		}

		void MessageFactory::initialise()
		{
			mVectorButtonName.push_back("Ok");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Ok");
			mVectorButtonName.push_back("Yes");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Yes");
			mVectorButtonName.push_back("No");
			mVectorButtonTag.push_back("MyGUI_MessageBox_No");
			mVectorButtonName.push_back("Abort");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Abort");
			mVectorButtonName.push_back("Retry");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Retry");
			mVectorButtonName.push_back("Ignore");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Ignore");
			mVectorButtonName.push_back("Cancel");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Cancel");
			mVectorButtonName.push_back("Try");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Try");
			mVectorButtonName.push_back("Continue");
			mVectorButtonTag.push_back("MyGUI_MessageBox_Continue");

			LanguageManager::getInstance().eventChangeLanguage += newDelegate(MessageFactory::changeLanguage);
		}

		void MessageFactory::shutdown()
		{
			LanguageManager::getInstance().eventChangeLanguage -= newDelegate(MessageFactory::changeLanguage);

			mVectorButtonName.clear();
			mVectorButtonTag.clear();
		}

		Ogre::UTFString MessageFactory::getButtonName(MessageBoxStyle _style)
		{
			size_t index = _style.getButtonIndex();
			if (mVectorButtonName.size() <= index) return "";
			return mVectorButtonName[index];
		}

	} // namespace factory
} // namespace MyGUI
