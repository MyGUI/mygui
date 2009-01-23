/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_CANVAS_FACTORY_H__
#define __MYGUI_CANVAS_FACTORY_H__

#include "MyGUI_Canvas.h"
#include "MyGUI_IWidgetFactory.h"

namespace MyGUI
{
	namespace factory
	{
		class /*MYGUI_EXPORT*/ CanvasFactory : public IWidgetFactory
		{
		public:
			CanvasFactory()
			{
				// регистрируем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.registerFactory(this);
			}

			~CanvasFactory()
			{
				// удаляем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.unregisterFactory(this);
			}

			const std::string & getTypeName()
			{
				return Canvas::getClassTypeName();
			}

			WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
			{
				return new Canvas(_style, _coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
			}

			bool isFalseType(WidgetPtr _ptr, const std::string &_key)
			{
				if (!_ptr->isType<Canvas>()) {
					MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
					return true;
				}
				return false;
			}
		};

		/*
		class MYGUI_EXPORT CanvasFactory : public BaseWidgetFactory< Canvas >
		{
		public:
			CanvasFactory();

			~CanvasFactory();
		};*/

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_CANVAS_FACTORY_H__
