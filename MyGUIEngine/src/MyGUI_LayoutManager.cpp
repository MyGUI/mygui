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
		MYGUI_ASSERT(false == mIsInitialise, "initialise already");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	VectorWidgetPtr LayoutManager::load(const std::string & _file, bool _resource)
	{
		VectorWidgetPtr ret;
		xml::xmlDocument doc;
		std::string file = (_resource ? helper::getResourcePath(_file) : _file).c_str();
		if (file.empty()) {
			MYGUI_LOG(Error, "Layout: " << _file << " not found");
			return ret;
		}
		if (false == doc.open(file)) {
			MYGUI_LOG(Error, doc.getLastError());
			return ret;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if (root == 0) {
			MYGUI_LOG(Error, "Layout: " << _file << " root tag not found");
			return ret;
		}

		if (root->getName() == "MyGUI") {
			std::string type;
			if ((false == root->findAttribute("type", type)) || (type != "Layout")) {
				MYGUI_LOG(Error, "Layout: " << _file << " root type 'Layout' not found");
				return ret;
			}
			parseLayoutMyGUI(ret, root);
		}
		else if (root->getName() == "GUILayout") parseLayoutCEGUI(ret, root);
		else {
			MYGUI_LOG(Error, "Layout: " << _file << " root type 'GUILayout' or 'MyGUI' not found");
			return ret;
		}

		return ret;
	}

	void LayoutManager::parseLayoutCEGUI(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root)
	{
	}

	void LayoutManager::parseLayoutMyGUI(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root)
	{
		// берем детей и крутимся
		xml::xmlNodeIterator widget = _root->getNodeIterator();
		while (widget.nextNode("Widget")) parseWidgetMyGUI(_widgets, widget, 0);
	}

	void LayoutManager::parseWidgetMyGUI(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent)
	{
		// парсим атрибуты виджета
		Ogre::String widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		FloatCoord coord;
		Align align = ALIGN_DEFAULT;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("name", widgetName);
		_widget->findAttribute("layer", widgetLayer);
		if (_widget->findAttribute("align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("position", tmp)) coord = FloatCoord::parse(tmp);
		if (_widget->findAttribute("position_real", tmp)) coord = convertFromReal(FloatCoord::parse(tmp), _parent);

		WidgetPtr wid;
		if (null == _parent) {
			wid = Gui::getInstance().createWidgetT(widgetType, widgetSkin, (int)coord.left, (int)coord.top, (int)coord.width, (int)coord.height, align, widgetLayer, widgetName);
			_widgets.push_back(wid);
		}
		else wid = _parent->createWidgetT(widgetType, widgetSkin, (int)coord.left, (int)coord.top, (int)coord.width, (int)coord.height, align, widgetName);

		// берем детей и крутимся
		xml::xmlNodeIterator widget = _widget->getNodeIterator();
		while (widget.nextNode()) {

			std::string key, value;

			if (widget->getName() == "Widget") parseWidgetMyGUI(_widgets, widget, wid);
			else if (widget->getName() == "Property") {

				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				// и парсим свойство
				WidgetManager::getInstance().parse(wid, key, value);
			}
			else if (widget->getName() == "UserString") {
				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				wid->setUserString(key, value);
			}

		};
	}

	FloatCoord LayoutManager::convertFromReal(const FloatCoord& _coord, WidgetPtr _parent)
	{
		const FloatSize& size = Gui::getInstance().getViewSize();
		if (null == _parent) {
			return FloatCoord(_coord.left * size.width, _coord.top * size.height, _coord.width * size.width, _coord.height * size.height);
		}
		const IntCoord& coord = _parent->getClientRect();
		return FloatCoord(_coord.left * coord.width, _coord.top * coord.height, _coord.width * coord.width, _coord.height * coord.height);
	}

} // namespace MyGUI
