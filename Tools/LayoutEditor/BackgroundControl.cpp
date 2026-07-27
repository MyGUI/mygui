/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "BackgroundControl.h"

namespace tools
{

	void BackgroundControlLE::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, "Background.layout");
	}

}
