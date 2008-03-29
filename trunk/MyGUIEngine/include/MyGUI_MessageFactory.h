/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_MESSAGE_FACTORY_H__
#define __MYGUI_MESSAGE_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport MessageFactory : public WidgetFactoryInterface
		{
		public:
			MessageFactory();
			~MessageFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			static Ogre::UTFString _getButtonName(size_t _index);
			inline static const std::string & _getDefaultSkin() {return mDefaultSkin;}

		private:
			void initialise();
			void shutdown();

			// методы для парсинга
			void Message_Caption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_Message(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_Modal(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_Button(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_AddButton(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_SmoothShow(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Message_Fade(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

			size_t parseButton(const std::string & _info);

		private:
			static std::vector<Ogre::UTFString> mVectorButtonName;
			static std::map<std::string, size_t> mMapButtonType;
			static std::string mDefaultSkin;

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MESSAGE_FACTORY_H__
