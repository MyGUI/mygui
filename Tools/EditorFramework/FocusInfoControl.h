/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _fd8c12d8_17c8_4bb6_a654_55ac29b333e9_
#define _fd8c12d8_17c8_4bb6_a654_55ac29b333e9_

#include "Control.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL FocusInfoControl : public Control, public sigslot::has_slots<>
	{
	public:
		~FocusInfoControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void Command_FocusVisible(const MyGUI::UString& _commandName, bool& _result);

		void notifyFrameStart(float _time);
		void updateFocusWidgetHelpers();

	private:
		MyGUI::Widget* mMouseView{nullptr};
		MyGUI::Widget* mKeyView{nullptr};
		MyGUI::IntCoord mKeyCoord;
		MyGUI::IntCoord mMouseCoord;
	};

}

#endif
