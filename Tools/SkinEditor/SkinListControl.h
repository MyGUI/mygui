/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _0fb2901a_cd4e_4472_82d5_f18f9843bced_
#define _0fb2901a_cd4e_4472_82d5_f18f9843bced_

#include "DataListBaseControl.h"

namespace tools
{

	class SkinListControl : public DataListBaseControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void commandCreateImageData(const MyGUI::UString& _commandName, bool& _result);
	};

}

#endif
