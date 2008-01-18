/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Message.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	Message::Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Window(_coord, _align, _info, _parent, _name)
	{

		// парсим свойства
		/*const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ButtonPressed");
			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}*/
	}

} // namespace MyGUI
