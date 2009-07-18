/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#ifndef __MYGUI_SPACER_H__
#define __MYGUI_SPACER_H__

#include "MyGUI_Common.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	class Spacer;
	typedef Spacer* SpacerPtr;
		
	/** Spacer
	*/
	class /*MYGUI_EXPORT*/ Spacer : public Widget
	{
		MYGUI_RTTI_DERIVED(Spacer);
	protected:

		WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

	public:	

	};
} // namespace MyGUI

#endif // __MYGUI_SPACER_H__
