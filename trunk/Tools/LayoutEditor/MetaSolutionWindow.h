/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
	@module
*/
#ifndef __META_SOLUTION_WINDOW_H__
#define __META_SOLUTION_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

struct MetaWidget
{
	std::string mName;
	std::string mType;
	MyGUI::Guid mTarget;
};

struct MetaForm
{
	MetaForm() : mCollapsed(true) {};
	std::vector<MetaWidget*> mChilds;
	std::string mDecription;
	std::string mLayoutName;
	MyGUI::Guid mId;

	bool mCollapsed;
};

class MetaSolutionWindow : public wraps::BaseLayout
{
public:
	MetaSolutionWindow();

	void load(MyGUI::xml::ElementEnumerator _field);
	void save(MyGUI::xml::ElementPtr root);

	bool isVisible() { return mMainWidget->isVisible(); }
	void setVisible(bool _visible) { mMainWidget->setVisible(_visible); };

	typedef MyGUI::delegates::CDelegate1<const std::string &> EventInfo_OgreString;
	EventInfo_OgreString eventLoadFile;
	MyGUI::EventHandle_WidgetVoid eventSelectWidget;
private:
	void notifyListSelectAccept(MyGUI::ListPtr _sender, size_t _index);
	void notifyListChangePosition(MyGUI::ListPtr _sender, size_t _index);

	void parseMetaSolution(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version);

	void updateList();
	void collapseAll();
	void loadTarget(MyGUI::Guid _target);
	bool findTarget(MyGUI::Guid _target);

	MyGUI::ListPtr mListTree;

	std::vector<MetaForm*> mMetaForms;
};

#endif // __META_SOLUTION_WINDOW_H__
