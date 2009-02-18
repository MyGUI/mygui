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
#ifndef __MYGUI_MESSAGE_FACTORY_H__
#define __MYGUI_MESSAGE_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Message.h"

namespace MyGUI
{
	namespace factory
	{

		typedef std::vector<Ogre::UTFString> VectorUTFString;

		class MYGUI_EXPORT MessageFactory : public BaseWidgetFactory<Message>
		{
		public:
			MessageFactory();
			~MessageFactory();

			static Ogre::UTFString getButtonName(MessageBoxStyle _style);

		private:
			void initialise();
			void shutdown();
			static void changeLanguage(const std::string & _language);

			// методы для парсинга
			void Message_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_Message(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_Modal(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_Button(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_AddButton(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Message_Fade(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		private:
			static VectorUTFString mVectorButtonName;
			static VectorUTFString mVectorButtonTag;
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MESSAGE_FACTORY_H__
