/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a1e61370_99b4_4e15_abd5_f33332ac52f0_
#define _a1e61370_99b4_4e15_abd5_f33332ac52f0_

#include "Control.h"
#include "sigslot.h"

namespace tools
{

	class StatisticInfoControl : public Control, public sigslot::has_slots<>
	{
	public:
		~StatisticInfoControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result);
		void notifyFrameStart(float _time);

	private:
		MyGUI::TextBox* mText{nullptr};
	};

}

#endif
