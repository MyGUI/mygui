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
	class PropertiesPanelView :
		public wraps::BaseLayout,
		public MyGUI::Singleton<PropertiesPanelView> //FIXME
	{
	public:
		PropertiesPanelView(MyGUI::Widget* _parent = nullptr);
		virtual ~PropertiesPanelView();

		typedef MyGUI::delegates::CMultiDelegate1<const MyGUI::IntCoord&> EventHandle_EventChangeCoord;
		EventHandle_EventChangeCoord eventChangeCoord;

		void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		bool checkType(MyGUI::Edit* _edit, const std::string& _type);
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::Edit* _sender); // calls notifyApplyProperties
		void notifyForceApplyProperties(MyGUI::Edit* _widget); // calls notifyApplyProperties
		void notifyForceApplyProperties2(MyGUI::ComboBox* _widget, size_t _index); // calls notifyApplyProperties

		std::string splitString(std::string& str, char separator);

		void commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result);

		void setPositionText(const std::string& _caption);
		void toggleRelativeMode();

		void hideWidgetsPairs(MyGUI::Widget* _window);
		void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y);

		bool isSkinExist(const std::string& _skinName);
		bool checkTemplate(const std::string& _skinName);

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		SkinInfo getCellData(MyGUI::Widget* _sender, size_t _index);

	private:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;

		// properties window
		size_t mPairsCounter;

		typedef std::vector<MyGUI::TextBox*> VectorTextBox;
		typedef std::map<MyGUI::Widget*, VectorTextBox> MapVectorTextBox;
		MapVectorTextBox mPropertiesText;

		typedef std::map<MyGUI::Widget*, MyGUI::VectorWidgetPtr> MapVectorWidget;
		MapVectorWidget mPropertiesElement;

		PanelMainProperties* mPanelMainProperties;
		static const int MAX_BASE_TYPES_COUNT = 5;
		PanelProperties* mPanelsTypeProperties[MAX_BASE_TYPES_COUNT];
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
