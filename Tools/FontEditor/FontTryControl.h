/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _80ec6a1e_0ffd_4d34_9427_4a3b0cdd8bc6_
#define _80ec6a1e_0ffd_4d34_9427_4a3b0cdd8bc6_

#include "Control.h"
#include "sigslot.h"
#include "Data.h"

namespace tools
{

	class FontTryControl : public Control, public sigslot::has_slots<>
	{
	public:
		~FontTryControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void commandOnGenerateFont(const MyGUI::UString& _commandName, bool& _result);
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void updateFont();

	private:
		MyGUI::EditBox* mEdit{nullptr};
	};

}

#endif
