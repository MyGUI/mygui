/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "StrangeButton.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ISubWidgetText.h"

namespace plugin
{

	StrangeButton::StrangeButton()
	{
	}

	void StrangeButton::onMouseSetFocus(MyGUI::Widget* _old)
	{
		Base::onMouseSetFocus(_old);

		if (getSubWidgetText() == nullptr)
			return;

		std::string s, str = getSubWidgetText()->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		getSubWidgetText()->setCaption(s);
	}

	void StrangeButton::onMouseLostFocus(MyGUI::Widget* _new)
	{
		Base::onMouseLostFocus(_new);

		if (getSubWidgetText() == nullptr)
			return;

		std::string s, str = getSubWidgetText()->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		getSubWidgetText()->setCaption(s);
	}

} // namespace plugin
