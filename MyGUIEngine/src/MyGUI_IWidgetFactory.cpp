///*!
//	@file
//	@author		Georgiy Evmenov
//	@date		10/2008
//	@module
//*/
//#ifndef __MYGUI_I_WIDGET_FACTORY_CPP__
//#define __MYGUI_I_WIDGET_FACTORY_CPP__
//
//#include "MyGUI_IWidgetFactory.h"
//#include "MyGUI_WidgetManager.h"
//#include "MyGUI_SkinManager.h"
//
//namespace MyGUI
//{
//	namespace factory
//	{
//
//		template <typename T>
//		BaseWidgetFactory<T>::BaseWidgetFactory()
//		{
//			// регестрируем себя
//			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
//			manager.registerFactory(this);
//		}
//
//		template <typename T>
//		BaseWidgetFactory<T>::~BaseWidgetFactory()
//		{
//			// удаляем себя
//			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
//			manager.unregisterFactory(this);
//		}
//
//		template <typename T>
//		const std::string & BaseWidgetFactory<T>::getTypeName()
//		{
//			return T::getClassTypeName();
//		}
//
//		template <typename T>
//		WidgetPtr BaseWidgetFactory<T>::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
//		{
//			return new T(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
//		}
//		
//		template <typename T>
//		bool BaseWidgetFactory<T>::isFalseType(WidgetPtr _ptr, const std::string &_key)
//		{
//			if (!_ptr->isType<T>()) {
//				MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
//				return true;
//			}
//			return false;
//		}
//
//	} // namespace factory
//} // namespace MyGUI
//
//#endif // __MYGUI_I_WIDGET_FACTORY_CPP__
