/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
	@module
*/
#include "precompiled.h"
#include "MetaSolutionWindow.h"
#include "BasisManager.h"
#include "EditorWidgets.h"

const std::string LogSection = "LayoutEditor";

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

MetaSolutionWindow::MetaSolutionWindow() :
	BaseLayout("MetaSolutionWindow.layout")
{
	assignWidget(mListTree, "ListTree");

	mListTree->eventListSelectAccept = MyGUI::newDelegate(this, &MetaSolutionWindow::notifyListSelectAccept);
	mListTree->eventListChangePosition = MyGUI::newDelegate(this, &MetaSolutionWindow::notifyListChangePosition);

	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("MetaSolution") = MyGUI::newDelegate(this, &MetaSolutionWindow::parseMetaSolution);
}

void MetaSolutionWindow::load(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
	while (field.next()) {
		/*std::string key, value;

		if (field->getName() == "Property")
		{
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;

			if (key == "key1") ;//do sth
			else if (key == "key2") ;//do sth
		}*/
	}
}

void MetaSolutionWindow::save(MyGUI::xml::ElementPtr root)
{
	/*root = root->createChild("MetaSolutionWindow");
	MyGUI::xml::ElementPtr nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "Grid");
	nodeProp->addAttribute("value", grid_step);

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "ShowName");
	nodeProp->addAttribute("value", getShowName());

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "ShowType");
	nodeProp->addAttribute("value", getShowType());*/
}

void MetaSolutionWindow::notifyListSelectAccept(MyGUI::ListPtr _sender, size_t _index)
{
	Ogre::UTFString line = _sender->getItemNameAt(_index);
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
		if (false == mw->mTarget.empty())
		{
			loadTarget(mw->mTarget);
		}
	}
}

void MetaSolutionWindow::notifyListChangePosition(MyGUI::ListPtr _sender, size_t _index)
{
	Ogre::UTFString line = _sender->getItemNameAt(_index);
	if (line[0] == '-' || line[0] == '+')
	{
		/*MetaForm* mf = *_sender->getItemDataAt<MetaForm*>(_index);
		mf->mCollapsed = !mf->mCollapsed;*/
	}
	else
	{
		MetaWidget* mw = *_sender->getItemDataAt<MetaWidget*>(_index);
		// выбрать виджет по имени
		//mw->mName
		WidgetContainer * container = EditorWidgets::getInstance().find(mw->mName);
		if (container)
		{
			eventSelectWidget(container->widget);
		}
	}
}

void MetaSolutionWindow::parseMetaSolution(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator metaForms = _node->getElementEnumerator();
	while (metaForms.next("MetaForm"))
	{
		MetaForm * metaForm = new MetaForm();
		metaForm->mDecription = metaForms->findAttribute("desc");

		// тип мерджа переменных
		metaForm->mLayoutName = metaForms->findAttribute("layout");
		metaForm->mId = MyGUI::Guid(metaForms->findAttribute("id"));

		// берем детей и крутимся
		MyGUI::xml::ElementEnumerator metaWidgets = metaForms->getElementEnumerator();
		while (metaWidgets.next("MetaWidget"))
		{
			MetaWidget * metaWidget = new MetaWidget();
			metaWidget->mName = metaWidgets->findAttribute("name");
			metaWidget->mType = metaWidgets->findAttribute("type");

			MyGUI::xml::ElementEnumerator links = metaWidgets->getElementEnumerator();
			while (links.next("MetaLink"))
			{
				if (metaWidget->mTarget.empty())
					metaWidget->mTarget = MyGUI::Guid(links->findAttribute("target"));
				else
					MYGUI_LOGGING(LogSection, Warning,
						"MetaWidget with name '" + metaWidget->mName +
						"' in MetaLayout '" + metaForm->mDecription +
						"' have more than one target");
			}
			metaForm->mChilds.push_back(metaWidget);
		}

		mMetaForms.push_back(metaForm);
	}

	updateList();
}

void MetaSolutionWindow::updateList()
{
	mListTree->removeAllItems();
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		std::string line = ((*iterMF)->mCollapsed ? "+ " : "- ") + (*iterMF)->mLayoutName + "  -  #808080" + (*iterMF)->mDecription;
		mListTree->addItem(line, *iterMF);
		if (false == (*iterMF)->mCollapsed)
		{
			for (std::vector<MetaWidget*>::iterator iter = (*iterMF)->mChilds.begin(); iter != (*iterMF)->mChilds.end(); ++iter)
			{
				line = MyGUI::utility::toString("   [ " + (*iter)->mType, " ] ", ((*iter)->mTarget.empty() ? "" : "#800000"), (*iter)->mName);
				mListTree->addItem(line, *iter);
			}
		}
	}
}

void MetaSolutionWindow::collapseAll()
{
	for (std::vector<MetaForm*>::iterator iterMF = mMetaForms.begin(); iterMF != mMetaForms.end(); ++iterMF)
	{
		(*iterMF)->mCollapsed = true;
	}
}

void MetaSolutionWindow::loadTarget(MyGUI::Guid _target)
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
