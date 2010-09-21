/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __MAIN_PANEL_H__
#define __MAIN_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class MainPanel :
		public wraps::BaseLayout
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

		void addObject(const MyGUI::UString& _name);

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);

	public:
		MyGUI::delegates::CDelegate2<TypeEvents, size_t> eventAction;

		MyGUI::Button* mButtonNew;
		MyGUI::Button* mButtonLoad;
		MyGUI::Button* mButtonQuit;

		MyGUI::ComboBox* mComboCreate;
	};

} // namespace demo

#endif // __MAIN_PANEL_H__
