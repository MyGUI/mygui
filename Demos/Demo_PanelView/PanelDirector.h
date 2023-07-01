/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef PANEL_DIRECTOR_H_
#define PANEL_DIRECTOR_H_

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	enum EventInfo
	{
		EVENT_SHOW_STATIC,
		EVENT_SHOW_DYNAMIC,
		EVENT_COUNT_DYNAMIC
	};

	using EventHandle_EventDirector = MyGUI::delegates::Delegate<int, size_t>;

	class PanelDirector : public wraps::BasePanelViewItem
	{
	public:
		PanelDirector();

		void initialise() override;
		void shutdown() override;

		EventHandle_EventDirector eventChangePanels;

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index) const;

	private:
		MyGUI::Button* mCheckShowStatic{nullptr};
		MyGUI::Button* mCheckShowDynamic{nullptr};
		MyGUI::ComboBox* mComboCount{nullptr};
	};

} // namespace demo

#endif // PANEL_DIRECTOR_H_
