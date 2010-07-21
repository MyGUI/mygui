/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
*/
#include "precompiled.h"
#include "MetaSolutionWindow.h"
//#include "BasisManager.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "WidgetTypes.h"

const std::string LogSection = "LayoutEditor";

MetaSolutionWindow::MetaSolutionWindow() :
	BaseLayout("MetaSolutionWindow.layout")
{
	current_widget = nullptr;
	assignWidget(mListTree, "ListTree");

	mWindow = mMainWidget->castType<MyGUI::Window>();
	mWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &MetaSolutionWindow::notifyCloseWindowButton);

	mListTree->eventListSelectAccept += MyGUI::newDelegate(this, &MetaSolutionWindow::notifyListSelectAccept);
	mListTree->eventListChangePosition += MyGUI::newDelegate(this, &MetaSolutionWindow::notifyListChangePosition);

	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("MetaSolution") = MyGUI::newDelegate(this, &MetaSolutionWindow::parseMetaSolution);
}

MetaSolutionWindow::~MetaSolutionWindow()
{
	closeMetaSolution();
}

void MetaSolutionWindow::load(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
	while (field.next())
	{
		std::string key, value;

		if (field->getName() == "Property")
		{
			if (!field->findAttribute("key", key)) continue;
			if (!field->findAttribute("value", value)) continue;

			if (key == "MetaSolutionName")
			{
				mMetaSolutionName = value;
				MyGUI::ResourceManager::getInstance().load(mMetaSolutionName/*, ""*/);
			}
			//else if (key == "key2") ;//do sth
		}
	}
}

void MetaSolutionWindow::save(MyGUI::xml::ElementPtr root)
{
	root = root->createChild("MetaSolutionWindow");
	MyGUI::xml::ElementPtr nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "MetaSolutionName");
	nodeProp->addAttribute("value", mMetaSolutionName);

	/*nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "ShowName");
	nodeProp->addAttribute("value", getShowName());*/
}

void MetaSolutionWindow::notifyCloseWindowButton(MyGUI::Window* _sender, const std::string& _name)
{
	if (_name == "close")
	{
		mMainWidget->setVisible(false);
		mMetaSolutionName = "";
	}
}

void MetaSolutionWindow::notifyListSelectAccept(MyGUI::List* _sender, size_t _index)
{
	if (_index == MyGUI::ITEM_NONE) return;
	MyGUI::UString line = _sender->getItemNameAt(_index);
	if (line[0] == '-' || line[0] == '+')
	{
		MetaForm* mf = *_sender->getItemDataAt<MetaForm*>(_index);
		eventLoadFile(mf->mLayoutName);
		collapseAll();
		mf->mCollapsed = false;
		updateList();
	}
	else
	{
		MetaWidget* mw = *_sender->getItemDataAt<MetaWidget*>(_index);
		WidgetContainer * container = EditorWidgets::getInstance().find(mw->mName);
		if (container == nullptr)
		{
			createWidget(mw, current_widget);
		}
		else
		{
			// загрузить лейаут если есть
			if (!mw->mTarget.empty())
			{
				loadTarget(mw->mTarget);
			}
		}
	}
}

void MetaSolutionWindow::notifyListChangePosition(MyGUI::List* _sender, size_t _index)
{
	if (_index == MyGUI::ITEM_NONE) return;
	MyGUI::UString line = _sender->getItemNameAt(_index);
	if (line[0] == '-' || line[0] == '+')
	{
	}
	else
	{
		// выделить виджет
		MetaWidget* mw = *_sender->getItemDataAt<MetaWidget*>(_index);

		WidgetContainer * container = EditorWidgets::getInstance().find(mw->mName);
		if (container)
		{
			eventSelectWidget(container->widget);
		}
	}
}

void MetaSolutionWindow::parseMetaSolution(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	closeMetaSolution();
	mMetaSolutionName = _file;

	size_t pos = _file.find_last_of("\\/");
	if (pos != std::string::npos)
		mWindow->setCaption(_file.substr(pos + 1));
	else
		mWindow->setCaption(_file);

	MyGUI::xml::ElementEnumerator meta_node = _node->getElementEnumerator();
	while (meta_node.next())
	{
		if (meta_node->getName() == "MetaForm")
		{
			MetaForm * metaForm = new MetaForm();

			metaForm->mDecription = meta_node->findAttribute("desc");
			metaForm->mLayoutName = meta_node->findAttribute("layout");
			metaForm->mId = meta_node->findAttribute("id");

			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator metaWidgets = meta_node->getElementEnumerator();
			while (metaWidgets.next("MetaWidget"))
			{
				MetaWidget * metaWidget = parseMetaWidget(metaWidgets.current(), nullptr);
				metaForm->mChilds.push_back(metaWidget);
			}

			mMetaForms.push_back(metaForm);
		}
		else if (meta_node->getName() == "WidgetType")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator meta_widget = meta_node->getElementEnumerator();
			while (meta_widget.next("Widget"))
			{
				mWidgetType[meta_widget->findAttribute("name")] =
					MyGUI::PairString(meta_widget->findAttribute("type"), meta_widget->findAttribute("skin"));
			}
		}
		else if (meta_node->getName() == "Widget")
		{
			std::string type_name = meta_node->findAttribute("name");

			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator widget = meta_node->getElementEnumerator();
			while (widget.next("Property"))
			{
				std::string key = widget->findAttribute("key");
				if (key != "Skin") continue;

				std::string value = widget->findAttribute("value");
				std::string group = widget->findAttribute("group");
				std::string button_name = widget->findAttribute("name");
				if (button_name.empty())
					button_name = value;

				WidgetTypes::getInstance().addWidgetSkinType(type_name, value, group, button_name);
			}
		}
	}

	loadList();
}

void MetaSolutionWindow::closeMetaSolution()
{
	while (mMetaForms.size())
	{
		delete (*mMetaForms.rbegin());
		mMetaForms.pop_back();
	}
}

MetaWidget * MetaSolutionWindow::parseMetaWidget(MyGUI::xml::ElementPtr _node, MetaWidget * _parent)
{
	MetaWidget * metaWidget = new MetaWidget();
	metaWidget->mName = _node->findAttribute("name");
	metaWidget->mType = _node->findAttribute("type");
	metaWidget->mParent = _parent;

	MyGUI::xml::ElementEnumerator links = _node->getElementEnumerator();
	while (links.next("MetaLink"))
	{
		if (metaWidget->mTarget.empty())
			metaWidget->mTarget = links->findAttribute("target");
		else
			MYGUI_LOGGING(LogSection, Warning,
				"MetaWidget with name '" + metaWidget->mName +
				"' have more than one target");
	}
	MyGUI::xml::ElementEnumerator metaWidgets = _node->getElementEnumerator();
	while (metaWidgets.next("MetaWidget"))
	{
		metaWidget->mChilds.push_back(parseMetaWidget(metaWidgets.current(), metaWidget));
	}
	return metaWidget;
}

void MetaSolutionWindow::loadList()
{
	mListTree->removeAllItems();
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		std::string line = MyGUI::utility::toString(((*iterMF)->mCollapsed ? "+ " : "- "), (*iterMF)->mLayoutName, "  -  #808080", (*iterMF)->mDecription);
		mListTree->addItem(line, *iterMF);
	}

	setVisible(true);
}

void MetaSolutionWindow::updateList()
{
	size_t i = 0;
	while (i != mListTree->getItemCount())
	{
		MyGUI::UString line = mListTree->getItemNameAt(i);
		if (line[0] != '-' && line[0] != '+')
			mListTree->removeItemAt(i);
		else
			i++;
	}

	i = 0;
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF, ++i)
	{
		if (!(*iterMF)->mCollapsed)
		{
			i += addMetaWidgets((*iterMF)->mChilds, i, "   ");
		}
	}
}

int MetaSolutionWindow::addMetaWidgets(std::vector<MetaWidget*> _childs, size_t _index, std::string _level)
{
	const std::string colour_created = "#008000";
	const std::string colour_destroed = "#800000";

	int i = 0;
	for (std::vector<MetaWidget*>::iterator iter = _childs.begin(); iter != _childs.end(); ++iter)
	{
		i++;
		WidgetContainer * container = EditorWidgets::getInstance().find((*iter)->mName);
		MyGUI::UString line = MyGUI::utility::toString(_level, "[ ", (*iter)->mType, " ] ",
			container ? colour_created : colour_destroed, (*iter)->mName,
			((*iter)->mTarget.empty() ? "" :
			((findTarget((*iter)->mTarget) ? colour_created : colour_destroed)+std::string(" [*]"))));
		mListTree->insertItemAt(_index+i, line, *iter);
		i += addMetaWidgets((*iter)->mChilds, _index+i, _level + "   ");
	}
	return i;
}

void MetaSolutionWindow::collapseAll()
{
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		(*iterMF)->mCollapsed = true;
	}
}

void MetaSolutionWindow::loadTarget(const std::string& _target)
{
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		if ((*iterMF)->mId == _target)
		{
			eventLoadFile((*iterMF)->mLayoutName);
			collapseAll();
			(*iterMF)->mCollapsed = false;
			updateList();
		}
	}
}

bool MetaSolutionWindow::findTarget(const std::string& _target)
{
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		if ((*iterMF)->mId == _target)
		{
			return true;
		}
	}
	return false;
}

MyGUI::Widget* MetaSolutionWindow::createWidget(MetaWidget * _widget, MyGUI::Widget* _parent)
{
	// создать виджет
	if (_widget->mParent)
	{
		// а создан ли родитель?
		WidgetContainer * trueParent = EditorWidgets::getInstance().find(_widget->mParent->mName);
		if (!trueParent)
		{
			_parent = createWidget(_widget->mParent, nullptr);
		}
		else
		{
			if (_parent == nullptr)
			{
				_parent = trueParent->widget;
			}
			else
			{
				// проверить что у current_widget есть в trueParent или это он и есть
				WidgetContainer * current_widgetContainer = EditorWidgets::getInstance().find(current_widget);
				
				while (current_widgetContainer != nullptr && current_widgetContainer != trueParent)
				{
					MyGUI::Widget* parent = current_widgetContainer->widget->getParent();
					current_widgetContainer = EditorWidgets::getInstance().find(parent);
					while (parent != nullptr && nullptr == current_widgetContainer)
					{
						parent = parent->getParent();
						current_widgetContainer = EditorWidgets::getInstance().find(parent);
					}
					if (parent == nullptr) current_widgetContainer = nullptr;
				}
				if (current_widgetContainer == nullptr) _parent = trueParent->widget;
			}
		}
	}

	std::string new_widget_type = _widget->mType;
	std::string new_widget_skin = _widget->mType; // дефолтный скин

	// ищем соответсвие типу и скину
	MapPairString::const_iterator iter = mWidgetType.find(_widget->mType);
	if (iter != mWidgetType.end())
	{
		new_widget_type = iter->second.first;
		new_widget_skin = iter->second.second;
	}

	int width = 64;
	int height = 32;

	std::string tmpname = "LayoutEditorWidget_" + _widget->mName;
	EditorWidgets::getInstance().global_counter++;

	while (_parent && !WidgetTypes::getInstance().find(_parent->getTypeName())->parent)
		_parent = _parent->getParent();

	MyGUI::IntSize parent_size;
	if (_parent && WidgetTypes::getInstance().find(new_widget_type)->child)
	{
		parent_size = _parent->getSize();
		_parent = _parent->createWidgetT(new_widget_type, new_widget_skin, 0, 0, width, height, MyGUI::Align::Default, tmpname);
	}
	else
	{
		parent_size = MyGUI::RenderManager::getInstance().getViewSize();
		_parent = MyGUI::Gui::getInstance().createWidgetT(new_widget_type, new_widget_skin, MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);

	}
	// place in parent center
	const MyGUI::IntCoord size((parent_size.width - width)/2, (parent_size.height - height)/2, width, height);
	_parent->setCoord(size);
	if (_parent->isType<MyGUI::StaticText>())
		_parent->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", new_widget_skin));

	WidgetContainer * widgetContainer = new WidgetContainer(new_widget_type, new_widget_skin, _parent, _widget->mName);
	EditorWidgets::getInstance().add(widgetContainer);
	eventSelectWidget(widgetContainer->widget);

	UndoManager::getInstance().addValue();

	return _parent;
}
