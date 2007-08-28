#pragma once

#include <vector>
#include <Ogre.h>
#include "xmlDocument.h"
#include "SkinManager.h"
#include "ParserManager.h"


namespace widget
{

	class LayoutManager
	{
	private:
		LayoutManager()
		{
		}

		~LayoutManager()
		{
		}

	public:
		static LayoutManager & getInstance()
		{
			static LayoutManager instance;
			return instance;
		}

		void loadLayout(const std::string & _file)
		{
			xml::xmlDocument doc;
			if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

			xml::xmlNodePtr xml_root = doc.getRoot();
			if (xml_root == 0) return;
			if (xml_root->getName() == "MyGUI_Layout") parseLayoutMyGUI(xml_root);
			else if (xml_root->getName() == "GUILayout") parseLayoutCEGUI(xml_root);
		}

	private:
		void parseLayoutMyGUI(xml::xmlNodePtr _root)
		{
			// берем детей и крутимся
			xml::VectorNode & widgets = _root->getChilds();
			for (size_t index=0; index<widgets.size(); index++) {
				xml::xmlNodePtr widgetInfo = widgets[index];
				if (widgetInfo->getName() == "Widget") parseWidgetMyGUI(widgetInfo, 0);
			}
		}

		void parseWidgetMyGUI(xml::xmlNodePtr _widgetInfo, WidgetPtr _parent)
		{
			// парсим атрибуты виджета
			const xml::VectorAttributes & attrib = _widgetInfo->getAttributes();
			Ogre::String widgetType, widgetSkin, widgetName, widgetLayer;
			floatRect coord;
			char align;
			for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
				// достаем пару атрибут - значение
				const xml::PairAttributes & pairAttributes = attrib[i_attrib];
				if (pairAttributes.first == "Type") widgetType = pairAttributes.second;
				else if (pairAttributes.first == "Skin") widgetSkin = pairAttributes.second;
				else if (pairAttributes.first == "Name") widgetName = pairAttributes.second;
				else if (pairAttributes.first == "Layer") widgetLayer = pairAttributes.second;
				else if (pairAttributes.first == "Align") align = SkinManager::getInstance().parseAlign(pairAttributes.second);
				else if (pairAttributes.first == "Position") coord = floatRect::parse(pairAttributes.second);
				else if (pairAttributes.first == "PositionReal") coord = Gui::getInstance().convertToReal(floatRect::parse(pairAttributes.second));
			}
			WidgetPtr wid;
			if (!_parent) wid = Gui::getInstance().createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetLayer, widgetName);
			else wid = _parent->createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetName);

			// берем детей и крутимся
			xml::VectorNode & widgets = _widgetInfo->getChilds();
			for (size_t index=0; index<widgets.size(); index++) {
				xml::xmlNodePtr widgetInfo = widgets[index];
				if (widgetInfo->getName() == "Widget") parseWidgetMyGUI(widgetInfo, wid);
				else if (widgetInfo->getName() == "Property") {

					// парсим атрибуты скина
					const xml::VectorAttributes & attrib = widgetInfo->getAttributes();
					std::string propertyKey, propertyValue;
					for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
						// достаем пару атрибут - значение
						const xml::PairAttributes & pairAttributes = attrib[i_attrib];
						if (pairAttributes.first == "Key") propertyKey = pairAttributes.second;
						else if (pairAttributes.first == "Value") propertyValue = pairAttributes.second;
					}
					// и парсим свойство
					ParserManager::getInstance().parce(wid, propertyKey, propertyValue);
				}
			}
		}

		void parseLayoutCEGUI(xml::xmlNodePtr _root)
		{
		}
		
	}; // class LayoutManager

} // namespace widget