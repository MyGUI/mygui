/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(LayoutManager);

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	bool LayoutManager::load(const std::string & _file, bool _resource)
	{
		xml::xmlDocument doc;
		if (false == doc.open((_resource ? helper::getResourcePath(_file) : _file).c_str())) {
			MYGUI_ERROR(doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if (root == 0) {
			MYGUI_ERROR("not find root tag");
			return false;
		}

		if (root->getName() == "MyGUI") {
			std::string type;
			if ((false == root->findAttribute("type", type)) || (type != "Layout")) {
				MYGUI_ERROR("not find root type 'Layout'");
				return false;
			}
			parseLayoutMyGUI(root);
		}
		else if (root->getName() == "GUILayout") parseLayoutCEGUI(root);
		else {
			MYGUI_ERROR("not find root tag 'GUILayout' or 'MyGUI'");
			return false;
		}

		return true;
	}

	void LayoutManager::parseLayoutCEGUI(xml::xmlNodePtr _root)
	{
	}

	void LayoutManager::parseLayoutMyGUI(xml::xmlNodePtr _root)
	{
		// берем детей и крутимся
		xml::xmlNodeIterator widget = _root->getNodeIterator();
		while (widget.nextNode("Widget")) parseWidgetMyGUI(widget, 0);
	}

	void LayoutManager::parseWidgetMyGUI(xml::xmlNodeIterator & _widget, WidgetPtr _parent)
	{
		// парсим атрибуты виджета
		//const xml::VectorAttributes & attrib = _widgetInfo->getAttributes();
		Ogre::String widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		FloatRect coord;
		Align align;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("name", widgetName);
		_widget->findAttribute("layer", widgetLayer);
		if (_widget->findAttribute("align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("position", tmp)) coord = util::parseFloatRect(tmp);
		if (_widget->findAttribute("position_real", tmp)) coord = convertToReal(util::parseFloatRect(tmp), _parent);

		WidgetPtr wid;
		if (!_parent) wid = Gui::getInstance().createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetLayer, widgetName);
		else wid = _parent->createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetName);

		// берем детей и крутимся
		xml::xmlNodeIterator widget = _widget->getNodeIterator();
		while (widget.nextNode()) {
			if (widget->getName() == "Widget") parseWidgetMyGUI(widget, wid);
			else if (widget->getName() == "Property") {

				// парсим атрибуты
				std::string propertyKey, propertyValue;
				if (false == widget->findAttribute("key", propertyKey)) continue;
				if (false == widget->findAttribute("value", propertyValue)) continue;
				// и парсим свойство
				WidgetManager::getInstance().parse(wid, propertyKey, propertyValue);
			}

		};
	}

	FloatRect LayoutManager::convertToReal(const FloatRect & _rect, WidgetPtr _parent)
	{
		if (null == _parent)
		{
			return FloatRect(_rect.left*Gui::getInstance().getWidth(), _rect.top*Gui::getInstance().getHeight(), _rect.right*Gui::getInstance().getWidth(), _rect.bottom*Gui::getInstance().getHeight());
		}
		else
    {
      IntRect ret = _parent->getClientRect();
			return FloatRect(_rect.left*ret.getWidth(), _rect.top*ret.getHeight(), _rect.right*ret.getWidth(), _rect.bottom*ret.getHeight());
    }
	}

} // namespace MyGUI