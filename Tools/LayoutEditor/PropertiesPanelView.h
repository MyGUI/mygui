/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _2a91cb5d_1a78_495b_8cb7_e46b12a29395_
#define _2a91cb5d_1a78_495b_8cb7_e46b12a29395_

#include "BaseLayout/BaseLayout.h"
#include "PanelView.h"
#include "PanelMainProperties.h"
#include "PanelProperties.h"
#include "PanelItems.h"
#include "PanelUserData.h"
#include "PanelControllers.h"
#include "EditorToolTip.h"
#include "PanelTemplateProperties.h"
#include "PanelExtensionProperties.h"

namespace tools
{

	class PropertiesPanelView :
		public wraps::BaseLayout
	{
	public:
		PropertiesPanelView(MyGUI::Widget* _parent = nullptr);
		virtual ~PropertiesPanelView();

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		PanelProperties* getPropertyWindow(WidgetStyle* _style, size_t _depth);
		size_t getIndexByDepth(size_t _depth);
		size_t getIndexPanel(PanelProperties* _panel);

	private:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;

		PanelMainProperties* mPanelMainProperties;

		typedef std::map<WidgetStyle*, PanelProperties*> MapPropertyWindow;
		MapPropertyWindow mMapPropertyWindow;

		PanelItems* mPanelItems;
		PanelUserData* mPanelUserData;
		PanelControllers* mPanelControllers;
		PanelTemplateProperties* mPanelTemplateProperties;
		PanelExtensionProperties* mPanelExtensionProperties;

		MyGUI::Widget* mCurrentWidget;
	};

}

#endif
