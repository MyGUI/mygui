/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_TEST_RENDER_BOX_FACTORY_H__
#define __MYGUI_TEST_RENDER_BOX_FACTORY_H__

#include "MyGUI_TestRenderBox.h"
#include "MyGUI_IWidgetFactory.h"

namespace MyGUI
{
	namespace factory
	{
		class /*MYGUI_EXPORT*/ TestRenderBoxFactory : public IWidgetFactory
		{
		public:
			TestRenderBoxFactory()
			{
				// регистрируем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.registerFactory(this);
			}

			~TestRenderBoxFactory()
			{
				// удаляем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.unregisterFactory(this);
			}

			const std::string & getTypeName()
			{
				return TestRenderBox::getClassTypeName();
			}

			WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
			{
				return new TestRenderBox(_style, _coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
			}

			bool isFalseType(WidgetPtr _ptr, const std::string &_key)
			{
				if (!_ptr->isType<TestRenderBox>()) {
					MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
					return true;
				}
				return false;
			}
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_TEST_RENDER_BOX_FACTORY_H__
