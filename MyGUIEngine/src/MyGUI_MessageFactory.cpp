/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
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
		std::map<std::string, size_t> MessageFactory::mMapButtonType;
		std::string MessageFactory::mDefaultSkin;

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

		Ogre::UTFString MessageFactory::_getButtonName(size_t _index)
		{
			if (mVectorButtonName.size() <= _index) return "";
			return mVectorButtonName[_index];
		}

		void MessageFactory::Message_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setCaption(_value);
		}

		void MessageFactory::Message_Message(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MessagePtr>(_widget)->setMessage(_value);
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
			static_cast<MessagePtr>(_widget)->setButton((Message::ViewInfo)parseButton(_value));
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

		size_t MessageFactory::parseButton(const std::string & _info)
		{
			size_t ret = 0;
			std::vector<std::string> vec = utility::split(_info);
			for (std::vector<std::string>::iterator iter=vec.begin(); iter!=vec.end(); ++iter) {
				std::map<std::string, size_t>::iterator value = mMapButtonType.find(*iter);
				if (value != mMapButtonType.end()) ret |= (*value).second;
			}
			return ret;
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
			// потом загружать из файла
			mDefaultSkin = "Message";

			mVectorButtonName.push_back("Ok");
			mVectorButtonTag.push_back("MessageBox_Ok");
			mVectorButtonName.push_back("Yes");
			mVectorButtonTag.push_back("MessageBox_Yes");
			mVectorButtonName.push_back("No");
			mVectorButtonTag.push_back("MessageBox_No");
			mVectorButtonName.push_back("Abort");
			mVectorButtonTag.push_back("MessageBox_Abort");
			mVectorButtonName.push_back("Retry");
			mVectorButtonTag.push_back("MessageBox_Retry");
			mVectorButtonName.push_back("Ignore");
			mVectorButtonTag.push_back("MessageBox_Ignore");
			mVectorButtonName.push_back("Cancel");
			mVectorButtonTag.push_back("MessageBox_Cancel");
			mVectorButtonName.push_back("Try");
			mVectorButtonTag.push_back("MessageBox_Try");
			mVectorButtonName.push_back("Continue");
			mVectorButtonTag.push_back("MessageBox_Continue");

			LanguageManager::getInstance().eventChangeLanguage += newDelegate(MessageFactory::changeLanguage);

			#undef REGISTER_VALUE
			#if MYGUI_DEBUG_MODE == 1
				#define REGISTER_VALUE(map, value) { \
					MYGUI_LOG(Info, "Register value : '" << #value << "' = " << Message::value); \
					map[#value] = Message::value; }
			#else // MYGUI_DEBUG_MODE == 1
				#define REGISTER_VALUE(map, value) map[#value] = Message::value;
			#endif // MYGUI_DEBUG_MODE == 1

			REGISTER_VALUE(mMapButtonType, Ok);
			REGISTER_VALUE(mMapButtonType, Yes);
			REGISTER_VALUE(mMapButtonType, No);
			REGISTER_VALUE(mMapButtonType, Abort);
			REGISTER_VALUE(mMapButtonType, Retry);
			REGISTER_VALUE(mMapButtonType, Ignore);
			REGISTER_VALUE(mMapButtonType, Cancel);
			REGISTER_VALUE(mMapButtonType, Try);
			REGISTER_VALUE(mMapButtonType, Continue);
			REGISTER_VALUE(mMapButtonType, OkCancel);
			REGISTER_VALUE(mMapButtonType, YesNo);
			REGISTER_VALUE(mMapButtonType, YesNoCancel);
			REGISTER_VALUE(mMapButtonType, RetryCancel);
			REGISTER_VALUE(mMapButtonType, AbortRetryIgnore);
			REGISTER_VALUE(mMapButtonType, CancelTryContinue);

			REGISTER_VALUE(mMapButtonType, IconDefault);

			REGISTER_VALUE(mMapButtonType, IconInfo);
			REGISTER_VALUE(mMapButtonType, IconQuest);
			REGISTER_VALUE(mMapButtonType, IconError);
			REGISTER_VALUE(mMapButtonType, IconWarning);

			REGISTER_VALUE(mMapButtonType, Icon1);
			REGISTER_VALUE(mMapButtonType, Icon2);
			REGISTER_VALUE(mMapButtonType, Icon3);
			REGISTER_VALUE(mMapButtonType, Icon4);
			REGISTER_VALUE(mMapButtonType, Icon5);
			REGISTER_VALUE(mMapButtonType, Icon6);
			REGISTER_VALUE(mMapButtonType, Icon7);
			REGISTER_VALUE(mMapButtonType, Icon8);

			#undef REGISTER_VALUE

		}

		void MessageFactory::shutdown()
		{
			LanguageManager::getInstance().eventChangeLanguage -= newDelegate(MessageFactory::changeLanguage);

			mVectorButtonName.clear();
			mVectorButtonTag.clear();
			mMapButtonType.clear();
		}

	} // namespace factory
} // namespace MyGUI
