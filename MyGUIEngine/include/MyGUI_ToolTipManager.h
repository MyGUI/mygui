/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TOOL_TIP_MANAGER_H_
#define MYGUI_TOOL_TIP_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_Singleton.h"

namespace MyGUI
{

	class MYGUI_EXPORT ToolTipManager : public IUnlinkWidget
	{
		MYGUI_SINGLETON_DECLARATION(ToolTipManager);

	public:
		ToolTipManager();

		void initialise();
		void shutdown();

		void setDelayVisible(float _value);
		float getDelayVisible() const;

		/*internal:*/
		void _unlinkWidget(Widget* _widget) override;

	private:
		void notifyEventFrameStart(float _time);

		void hideToolTip(Widget* _widget);
		void showToolTip(Widget* _widget, size_t _index, const IntPoint& _point);
		void moveToolTip(Widget* _widget, size_t _index, const IntPoint& _point);

		bool isNeedToolTip(Widget* _widget);
		size_t getToolTipIndex(Widget* _widget) const;

	private:
		float mDelayVisible{0.5f};
		Widget* mOldFocusWidget{nullptr};
		IntPoint mOldMousePoint;
		bool mToolTipVisible{false};
		float mCurrentTime{0};
		size_t mOldIndex{ITEM_NONE};
		bool mNeedToolTip{false};

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_TOOL_TIP_MANAGER_H_
