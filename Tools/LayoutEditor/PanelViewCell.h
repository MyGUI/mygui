/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/

#ifndef _f4088af8_f82a_48ab_b7ec_f8bdd04e4a85_
#define _f4088af8_f82a_48ab_b7ec_f8bdd04e4a85_

#include "PanelView/BasePanelViewItem.h"

namespace tools
{

	class PanelViewCell :
		public wraps::BasePanelViewCell
	{
	public:
		PanelViewCell(MyGUI::Widget* _parent) :
			BasePanelViewCell("PanelCell.layout", _parent),
			mButtonMinimize(nullptr)
		{
			assignWidget(mTextCaption, "text_Caption");
			assignWidget(mButtonMinimize, "button_Minimize");
			assignWidget(mWidgetClient, "widget_Client");

			mTextCaption->eventMouseButtonDoubleClick += MyGUI::newDelegate(this, &PanelViewCell::notifyMouseButtonDoubleClick);
			mButtonMinimize->eventMouseButtonPressed += MyGUI::newDelegate(this, &PanelViewCell::notfyMouseButtonPressed);
		}

		virtual void setMinimized(bool _minimized)
		{
			wraps::BasePanelViewCell::setMinimized(_minimized);
			mButtonMinimize->setStateSelected(isMinimized());
		}

	private:
		void notfyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
			{
				setMinimized( ! isMinimized() );
			}
		}

		void notifyMouseButtonDoubleClick(MyGUI::Widget* _sender)
		{
			setMinimized( ! isMinimized() );
		}

	private:
		MyGUI::Button* mButtonMinimize;

	};

}

#endif
