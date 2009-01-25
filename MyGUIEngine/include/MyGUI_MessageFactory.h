/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
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

			static Ogre::UTFString getButtonName(MessageStyle _style);
			//static const std::string & getDefaultSkin() { return mDefaultSkin; }

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

			//size_t parseButton(const std::string & _info);

		private:
			static VectorUTFString mVectorButtonName;
			static VectorUTFString mVectorButtonTag;
			//static std::map<std::string, size_t> mMapButtonType;
			//static std::string mDefaultSkin;

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MESSAGE_FACTORY_H__
