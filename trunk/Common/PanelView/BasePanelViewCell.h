/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_PANEL_VIEW_CELL_H__
#define __BASE_PANEL_VIEW_CELL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	class BasePanelViewCell : public BaseLayout2
	{
	public:

		BasePanelViewCell(const std::string & _layout, MyGUI::WidgetPtr _parent);
		virtual ~BasePanelViewCell();

		void setCaption(const Ogre::UTFString & _caption) { if (mTextCaption) mTextCaption->setCaption(_caption); }

		MyGUI::WidgetPtr getClient() { return mWidgetClient ? mWidgetClient : mMainWidget; }
		MyGUI::WidgetPtr getMainWidget() { return mMainWidget; }

		void setClientHeight(int _height, bool _smooth = true);
		bool isMinimized();
		void setMinimized(bool _minimized);

		void show() { mMainWidget->show(); }
		void hide() { mMainWidget->hide(); }
		bool isShow() { return mMainWidget->isShow(); }

		MyGUI::delegates::CDelegate1<BasePanelViewCell*> eventUpdatePanel;

	private:
		void notifyUpdateAction(MyGUI::WidgetPtr _widget);
		void updateMinimized();

	protected:
		MyGUI::StaticTextPtr mTextCaption;
		MyGUI::WidgetPtr mWidgetClient;
		bool m_minimized;

		int m_minHeight;
		int m_maxHeight;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_CELL_H__
