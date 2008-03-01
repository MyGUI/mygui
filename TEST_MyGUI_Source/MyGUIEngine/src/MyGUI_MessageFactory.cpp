/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_MessageFactory.h"
#include "MyGUI_Message.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{
	namespace factory
	{

		std::vector<Ogre::DisplayString> MessageFactory::mVectorButtonName;
		std::map<std::string, size_t> MessageFactory::mMapButtonType;
		std::string MessageFactory::mDefaultSkin;

		MessageFactory::MessageFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

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
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

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

		const Ogre::String& MessageFactory::getType()
		{
			return Message::_getType();
		}

		WidgetPtr MessageFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new Message(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		Ogre::DisplayString MessageFactory::_getButtonName(size_t _index)
		{
			if (mVectorButtonName.size() <= _index) return "";
			return mVectorButtonName[_index];
		}

		void MessageFactory::Message_Caption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			static_cast<MessagePtr>(_widget)->setCaption(_value);
		}

		void MessageFactory::Message_Message(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			static_cast<MessagePtr>(_widget)->setMessage(_value);
		}

		void MessageFactory::Message_Modal(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			if (utility::parseBool(_value)) InputManager::getInstance().addWidgetModal(_widget);
			else InputManager::getInstance().removeWidgetModal(_widget);
		}

		void MessageFactory::Message_Button(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			static_cast<MessagePtr>(_widget)->setButton((Message::ViewInfo)parseButton(_value));
		}

		void MessageFactory::Message_AddButton(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			static_cast<MessagePtr>(_widget)->addButtonName(_value);
		}

		void MessageFactory::Message_SmoothShow(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
			static_cast<MessagePtr>(_widget)->setSmoothShow(utility::parseBool(_value));
		}

		void MessageFactory::Message_Fade(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MessagePtr, _widget, _key);
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

		void MessageFactory::initialise()
		{
			// потом загружать из файла
			mDefaultSkin = "Message";
			mVectorButtonName.push_back("Ok");
			mVectorButtonName.push_back("Yes");
			mVectorButtonName.push_back("No");
			mVectorButtonName.push_back("Abort");
			mVectorButtonName.push_back("Retry");
			mVectorButtonName.push_back("Ignore");
			mVectorButtonName.push_back("Cancel");
			mVectorButtonName.push_back("Try");
			mVectorButtonName.push_back("Continue");

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

			#undef REGISTER_VALUE

		}

		void MessageFactory::shutdown()
		{
			mVectorButtonName.clear();
			mMapButtonType.clear();
		}

	} // namespace factory
} // namespace MyGUI
