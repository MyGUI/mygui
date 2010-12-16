/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PROPERTIES_PANEL_VIEW_H__
#define __PROPERTIES_PANEL_VIEW_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView.h"
#include "PanelMainProperties.h"
#include "PanelProperties.h"
#include "PanelItems.h"
#include "PanelUserData.h"
#include "PanelControllers.h"
#include "EditorToolTip.h"

namespace tools
{
	class Entry
	{
	public:
		Entry();

		void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, MyGUI::TextBox*& _field, int _height, MyGUI::Widget* _currentWidget, EditorToolTip* _toolTip);
		void destroy();

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);
		void notifyForceApplyProperties2(MyGUI::ComboBox* _widget, size_t _index);

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		SkinInfo getCellData(MyGUI::Widget* _sender, size_t _index);

		bool checkType(MyGUI::EditBox* _edit, const std::string& _type);
		bool isSkinExist(const std::string& _skinName);
		std::string splitString(std::string& str, char separator);
		bool checkTemplate(const std::string& _skinName);

	private:
		MyGUI::TextBox* mText;
		MyGUI::Widget* mField;
		MyGUI::Widget* mCurrentWidget;
		EditorToolTip* mToolTip;
	};

	class PropertiesPanelView :
		public wraps::BaseLayout
	{
	public:
		PropertiesPanelView(MyGUI::Widget* _parent = nullptr);
		virtual ~PropertiesPanelView();

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void hideWidgetsPairs(MyGUI::Widget* _window);
		void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, MyGUI::TextBox*& _field);

		PanelProperties* getPropertyWindow(WidgetStyle* _style);

	private:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;

		typedef std::vector<Entry> VectorEntry;

		typedef std::map<MyGUI::Widget*, VectorEntry> MapInfo;
		MapInfo mPropertyInfo;

		PanelMainProperties* mPanelMainProperties;

		typedef std::map<WidgetStyle*, PanelProperties*> MapPropertyWindow;
		MapPropertyWindow mMapPropertyWindow;

		PanelItems* mPanelItems;
		PanelUserData* mPanelUserData;
		PanelControllers* mPanelControllers;

		typedef std::vector<wraps::BasePanelViewItem*> VectorPanel;
		VectorPanel mPanels;

		MyGUI::Widget* mCurrentWidget;
		int mPropertyItemHeight;

		EditorToolTip* mToolTip;
	};

} // namespace tools

#endif // __PROPERTIES_PANEL_VIEW_H__
