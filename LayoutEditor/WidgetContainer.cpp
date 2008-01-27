#include "WidgetContainer.h"
#include "MyGUI.h"

INSTANCE_IMPLEMENT(EditorWidgets);

void EditorWidgets::initialise()
{

}

void EditorWidgets::shutdown()
{
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter) delete *iter;
	widgets.clear();
}

bool EditorWidgets::load(std::string _fileName)
{
	// copy from Gui::_loadImplement
	/*xml::xmlDocument doc;
	std::string file(_group.empty() ? _file : helper::getResourcePath(_file, _group));
	if (file.empty()) {
		MYGUI_LOG(Error, _instance << " : '" << _file << "' not found");
		return false;
	}
	if (false == doc.open(file)) {
		MYGUI_LOG(Error, _instance << " : " << doc.getLastError());
		return false;
	}

	xml::xmlNodePtr root = doc.getRoot();
	if ( (null == root) || (root->getName() != "MyGUI") ) {
		MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
		if (iter != mMapLoadXmlDelegate.end()) {
			if ((false == _match) || (type == _type)) (*iter).second(root, file);
			else {
				MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not found");
				return false;
			}
		}
		else {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
			return false;
		}
	}
	// предпологаем что будут вложенные
	else if (false == _match) {
		xml::xmlNodeIterator node = root->getNodeIterator();
		while (node.nextNode("MyGUI")) {
			if (node->findAttribute("type", type)) {
				MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
				if (iter != mMapLoadXmlDelegate.end()) {
					(*iter).second(node.currentNode(), file);
				}
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
				}
			}
			else {
				MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not found");
			}
		}
	}
*/
	return true;
}

bool EditorWidgets::save(std::string _fileName)
{
	return true;
}

void EditorWidgets::add(std::string _name, MyGUI::WidgetPtr _widget, MyGUI::WidgetPtr _widget_rectangle)
{
	widgets.push_back(new WidgetContainer(_name, _widget, _widget_rectangle));
}

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget, bool _isRectangle)
{
	// найдем соответствующий виджет и переместим/растянем
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if (_isRectangle){
			if ((*iter)->widget_rectangle == _widget)
			{
				return *iter;
			}
		}
		else
		{
			if ((*iter)->widget == _widget)
			{
				return *iter;
			}
		}
	}
	MYGUI_EXCEPT("как это не нашли виджет?");
	return null;
}
WidgetContainer * EditorWidgets::find(std::string _name, bool _isRectangle)
{
	// найдем соответствующий виджет и переместим/растянем
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if (_isRectangle){
			if ((*iter)->name == _name + "_rectangle")
			{
				return *iter;
			}
		}else{
			if ((*iter)->name == _name)
			{
				return *iter;
			}
		}
	}
	MYGUI_EXCEPT("как это не нашли виджет?");
	return null;
}
