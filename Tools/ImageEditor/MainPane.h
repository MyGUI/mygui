/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_PANE_H__
#define __MAIN_PANE_H__

#include "Control.h"

namespace tools
{
	class MainPane :
		public Control
	{
	public:
		MainPane();
		virtual ~MainPane();

		void setVisible(bool _value);

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);

	private:
		MyGUI::TabControl* mTab;
	};

} // namespace tools

#endif // __MAIN_PANE_H__
