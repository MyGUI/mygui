/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_I_WIDGET_FACTORY_H__
#define __MYGUI_I_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Align.h"
#include "MyGUI_WidgetStyle.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ICroppedRectangle.h"

#include "MyGUI_WidgetManager.h"
#include "MyGUI_SkinManager.h"

namespace MyGUI
{
	class MYGUI_EXPORT IWidgetFactory
	{
	public:
		virtual ~IWidgetFactory() { }

		virtual const std::string & getTypeName() = 0;
		virtual WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name) = 0;
	};

	namespace factory
	{

		template <typename T>
		class MYGUI_EXPORT BaseWidgetFactory : public IWidgetFactory
		{
		public:
			BaseWidgetFactory()
			{
				// регестрируем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.registerFactory(this);
			}

			~BaseWidgetFactory()
			{
				// удаляем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.unregisterFactory(this);
			}

			const std::string & getTypeName()
			{
				return T::getClassTypeName();
			}

			WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
			{
				return new T(_style, _coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
			}

			bool isFalseType(WidgetPtr _ptr, const std::string &_key)
			{
				if (!_ptr->isType<T>()) {
					MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
					return true;
				}
				return false;
			}
		};

	} // namespace factory
} // namespace MyGUI

//#include "MyGUI_IWidgetFactory.cpp"

#endif // __MYGUI_I_WIDGET_FACTORY_H__
