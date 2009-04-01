/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
	@module
*/
#ifndef __META_SOLUTION_WINDOW_H__
#define __META_SOLUTION_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

class MetaSolutionWindow : public wraps::BaseLayout
{
public:
	MetaSolutionWindow();

	void initialise();

	void load(MyGUI::xml::ElementEnumerator _field);
	void save(MyGUI::xml::ElementPtr root);

	bool isVisible() { return mMainWidget->isVisible(); }
	void setVisible(bool _visible) { mMainWidget->setVisible(_visible); };
private:
	void notifyListSelectAccept(MyGUI::ListPtr _sender, size_t _index);

	MyGUI::ListPtr mListTree;
};

#endif // __META_SOLUTION_WINDOW_H__
