/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MAIN_PANEL_H__
#define __MAIN_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	class MainPanel : public wraps::BaseLayout
	{
	public:

		typedef enum {
			EventNew,
			EventLoad,
			EventQuit,
			EventCreate
		} TypeEvents;

	public:
		MainPanel();
		virtual void initialise();
		void addObject(const Ogre::UTFString & _name);

	private:
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyComboAccept(MyGUI::WidgetPtr _sender);

	public:
		MyGUI::delegates::CDelegate2<TypeEvents, size_t> eventAction;

		MyGUI::ButtonPtr mButtonNew;
		MyGUI::ButtonPtr mButtonLoad;
		MyGUI::ButtonPtr mButtonQuit;

		MyGUI::ComboBoxPtr mComboCreate;
	};

} // namespace demo

#endif // __MAIN_PANEL_H__
