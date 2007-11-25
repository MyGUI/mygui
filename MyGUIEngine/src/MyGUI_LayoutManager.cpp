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

	void LayoutManager::load(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) MYGUI_EXCEPT(doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() == "MyGUI_Layout") parseLayoutMyGUI(xml_root);
		else if (xml_root->getName() == "GUILayout") parseLayoutCEGUI(xml_root);
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

		_widget->findAttribute("Type", widgetType);
		_widget->findAttribute("Skin", widgetSkin);
		_widget->findAttribute("Name", widgetName);
		_widget->findAttribute("Layer", widgetLayer);
		if (_widget->findAttribute("Align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("Position", tmp)) coord = util::parseFloatRect(tmp);
		if (_widget->findAttribute("PositionReal", tmp)) coord = Gui::getInstance().convertToReal(util::parseFloatRect(tmp));

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
				if (false == widget->findAttribute("Key", propertyKey)) continue;
				if (false == widget->findAttribute("Value", propertyValue)) continue;
				// и парсим свойство
				WidgetManager::getInstance().parse(wid, propertyKey, propertyValue);
			}

		};
	}

} // namespace MyGUI