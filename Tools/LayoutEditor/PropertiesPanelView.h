/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PROPERTIES_PANEL_VIEW_H__
#define __PROPERTIES_PANEL_VIEW_H__

//#include "PanelView/PanelView.h"
//#include "PanelView/PanelViewWindow.h"
#include "BaseLayout/BaseLayout.h"
#include "PanelView.h"
#include "PanelMainProperties.h"
#include "PanelProperties.h"
#include "PanelItems.h"
#include "PanelUserData.h"

extern int grid_step;//FIXME_HOOK

class PropertiesPanelView : public wraps::BaseLayout
{
public:
	PropertiesPanelView();
	virtual ~PropertiesPanelView();

	void load(MyGUI::xml::ElementEnumerator field);
	void save(MyGUI::xml::ElementPtr root);

	// widget editing
	void notifyRectangleResize(MyGUI::WindowPtr _sender);
	void notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

	void update(MyGUI::WidgetPtr _current_widget);

	void createPropertiesWidgetsPair(MyGUI::WidgetPtr _window, Ogre::String _property, Ogre::String _value, std::string _type, int y);
	MyGUI::WindowPtr getWidgetRectangle(){return current_widget_rectangle;};

	void setPositionText(const std::string & _caption){propertiesElement[1]->setCaption(_caption);}
	void toggleRelativeMode() { mPanelMainProperties->notifyToggleRelativeMode(); }

	typedef MyGUI::delegates::CDelegate0 EventInfo;
	EventInfo eventRecreate;

	MyGUI::WidgetPtr getMainWidget() { return mMainWidget; }

private:
	void notifyWindowChangeCoord(MyGUI::WindowPtr _sender);
	bool checkType(MyGUI::EditPtr _edit, std::string _type);
	void notifyApplyProperties(MyGUI::WidgetPtr _sender, bool _force);
	void notifyTryApplyProperties(MyGUI::EditPtr _sender); // calls notifyApplyProperties
	void notifyForceApplyProperties(MyGUI::EditPtr _widget); // calls notifyApplyProperties
	void notifyForceApplyProperties2(MyGUI::ComboBoxPtr _widget, size_t _index); // calls notifyApplyProperties

private:
	MyGUI::IntSize mOldSize;
	PanelView* mPanelView;

	// properties window
	size_t pairs_counter;
	std::vector<MyGUI::StaticTextPtr> propertiesText;
	MyGUI::VectorWidgetPtr propertiesElement;

	PanelMainProperties * mPanelMainProperties;
	PanelProperties * mPanelTypeProperties;
	PanelProperties * mPanelGeneralProperties;
	PanelProperties * mPanelEvents;
	PanelItems * mPanelItems;
	PanelUserData * mPanelUserData;

	typedef std::vector<wraps::BasePanelViewItem*> VectorPanel;
	VectorPanel mPanels;

	MyGUI::WidgetPtr current_widget;
	MyGUI::WindowPtr current_widget_rectangle;

	// widget was moved by keyboard arrows
	bool arrow_move;
};

#endif // __PROPERTIES_PANEL_VIEW_H__
