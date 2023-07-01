/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef MAIN_PANEL_H_
#define MAIN_PANEL_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class MainPanel : public wraps::BaseLayout
	{
	public:
		enum TypeEvents
		{
			EventNew,
			EventLoad,
			EventQuit,
			EventCreate
		};

	public:
		MainPanel();

		void addObject(const MyGUI::UString& _name) const;

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender) const;
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index) const;

	public:
		MyGUI::delegates::Delegate<TypeEvents, size_t> eventAction;

		MyGUI::Button* mButtonNew;
		MyGUI::Button* mButtonLoad;
		MyGUI::Button* mButtonQuit;

		MyGUI::ComboBox* mComboCreate;
	};

} // namespace demo

#endif // MAIN_PANEL_H_
