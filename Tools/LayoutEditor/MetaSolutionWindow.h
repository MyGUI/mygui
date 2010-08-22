/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
*/
#ifndef __META_SOLUTION_WINDOW_H__
#define __META_SOLUTION_WINDOW_H__

/*#include "BaseLayout/BaseLayout.h"

struct MetaWidget
{
	~MetaWidget()
	{
		while (mChilds.size())
		{
			delete *mChilds.rbegin();
			mChilds.pop_back();
		}
	}
	MetaWidget * mParent;
	std::vector<MetaWidget *> mChilds;
	std::string mName;
	std::string mType;
	std::string mTarget;
};

struct MetaForm
{
	MetaForm() : mCollapsed(true) { }
	~MetaForm()
	{
		while (mChilds.size())
		{
			delete *mChilds.rbegin();
			mChilds.pop_back();
		}
	}
	std::vector<MetaWidget*> mChilds;
	std::string mDecription;
	std::string mLayoutName;
	std::string mId;

	bool mCollapsed;
};

class MetaSolutionWindow :
	public wraps::BaseLayout
{
public:
	MetaSolutionWindow();
	virtual ~MetaSolutionWindow();

	bool getVisible() { return mMainWidget->getVisible(); }
	void setVisible(bool _visible) { mMainWidget->setVisible(_visible); }
	
	void loadList();
	void updateList();

	typedef MyGUI::delegates::CDelegate1<const MyGUI::UString &> EventInfo_UString;
	EventInfo_UString eventLoadFile;

	MyGUI::Widget* getMainWidget() { return mMainWidget; }

private:
	void notifyChangeSelectedWidget(MyGUI::Widget* _current_widget);
	void notifyCloseWindowButton(MyGUI::Window* _sender, const std::string& _name);

	void notifyListSelectAccept(MyGUI::List* _sender, size_t _index);
	void notifyListChangePosition(MyGUI::List* _sender, size_t _index);

	void parseMetaSolution(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
	void closeMetaSolution();
	MetaWidget * parseMetaWidget(MyGUI::xml::ElementPtr _node, MetaWidget * _parent);

	int addMetaWidgets(std::vector<MetaWidget*> _childs, size_t _index, std::string _level);
	void collapseAll();
	void loadTarget(const std::string& _target);
	bool findTarget(const std::string& _target);

	MyGUI::Widget* createWidget(MetaWidget * _widget, MyGUI::Widget* _parent);

	MyGUI::List* mListTree;
	std::string mMetaSolutionName;
	
	MyGUI::Widget* current_widget;

	std::vector<MetaForm*> mMetaForms;

	typedef std::map<std::string, MyGUI::PairString> MapPairString;
	MapPairString mWidgetType;
	MyGUI::Window* mWindow;
};*/

#endif // __META_SOLUTION_WINDOW_H__
