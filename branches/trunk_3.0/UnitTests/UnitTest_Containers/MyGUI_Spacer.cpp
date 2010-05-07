/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#include "MyGUI_Spacer.h"

namespace MyGUI
{
	void Spacer::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);
		// TODO: only for debug
		setNeedMouseFocus(true);
		//initialiseWidgetSkin(_info, _coord.size() );
	}

	Widget* Spacer::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		MYGUI_EXCEPT("Have you ever seen that nothing has anything? I'm not sure that it's wright, so I put there exception that notify you (and me :) ) not to put anything here. Yes, don't put anything to Spacer!" );
	}
	
} // namespace MyGUI

