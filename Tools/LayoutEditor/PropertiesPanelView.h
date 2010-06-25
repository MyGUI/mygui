/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
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
#include "PanelControllers.h"

extern int grid_step;//FIXME_HOOK

class PropertiesPanelView : public wraps::BaseLayout
{
public:
	PropertiesPanelView();
	virtual ~PropertiesPanelView();

	void load(MyGUI::xml::ElementEnumerator field);
	void save(MyGUI::xml::ElementPtr root);
	void update(MyGUI::Widget* _current_widget);

	// widget editing
	void notifyRectangleResize(MyGUI::Window* _sender);
	void notifyRectangleKeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);


	void hideWidgetsPairs(MyGUI::Widget* _window);
	void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y);
	MyGUI::Window* getWidgetRectangle() { return current_widget_rectangle; }

	void setPositionText(const std::string& _caption);
	void toggleRelativeMode() { mPanelMainProperties->notifyToggleRelativeMode(); }

	typedef MyGUI::delegates::CDelegate0 EventInfo;
	EventInfo eventRecreate;

	MyGUI::Widget* getMainWidget() { return mMainWidget; }

private:
	void notifyWindowChangeCoord(MyGUI::Window* _sender);
	bool checkType(MyGUI::Edit* _edit, const std::string& _type);
	void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
	void notifyTryApplyProperties(MyGUI::Edit* _sender); // calls notifyApplyProperties
	void notifyForceApplyProperties(MyGUI::Edit* _widget); // calls notifyApplyProperties
	void notifyForceApplyProperties2(MyGUI::ComboBox* _widget, size_t _index); // calls notifyApplyProperties

	std::string splitString(std::string& str, char separator);
private:
	MyGUI::IntSize mOldSize;
	PanelView* mPanelView;

	// properties window
	size_t mPairsCounter;
	std::map<MyGUI::Widget*, std::vector<MyGUI::StaticText*> > mPropertiesText;
	std::map<MyGUI::Widget*, MyGUI::VectorWidgetPtr> mPropertiesElement;

	PanelMainProperties * mPanelMainProperties;
	static const int MaxBaseTypesCount = 5;
	PanelProperties* mPanelsTypeProperties[MaxBaseTypesCount];
	PanelItems * mPanelItems;
	PanelUserData * mPanelUserData;
	PanelControllers * mPanelControllers;

	typedef std::vector<wraps::BasePanelViewItem*> VectorPanel;
	VectorPanel mPanels;

	MyGUI::Widget* current_widget;
	MyGUI::Window* current_widget_rectangle;

	// widget was moved by keyboard arrows
	bool arrow_move;
};

#endif // __PROPERTIES_PANEL_VIEW_H__
