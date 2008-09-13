/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_CELL_H__
#define __PANEL_CELL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class PanelCell : public BaseLayout
{
public:
	typedef MyGUI::delegates::CDelegate1<PanelCell*> DelegateUpdate;

	PanelCell();

	virtual void initialise(MyGUI::WidgetPtr _parent);

	inline void setCaption(const Ogre::UTFString & _caption) { mTextCaption->setCaption(_caption); }
	inline const Ogre::UTFString & getCaption() { return mTextCaption->getCaption(); }

	DelegateUpdate eventUpdatePanel;

	inline MyGUI::WidgetPtr getClient() { return mWidgetClient; }

	void setClientHeight(int _height, bool _smooth = true);

private:
	void notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
	void notifyUpdateAction(MyGUI::WidgetPtr _widget);

private:

	MyGUI::StaticTextPtr mTextCaption;
	MyGUI::ButtonPtr mButtonMinimize;
	MyGUI::WidgetPtr mWidgetClient;

	int m_minHeight;
	int m_maxHeight;
};

#endif // __PANEL_CELL_H__
