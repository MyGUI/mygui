/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_Container.h"

namespace MyGUI
{
	void Container::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise( _style, _coord, _align, _info, _parent, _croppedParent, _creator, _name );
	}

	Widget* Container::baseCreateWidget( WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name )
	{
		Widget* widget = Widget::baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name );
		add( widget );
		return widget;
	}

	void Container::setSize( const IntSize& _size )
	{
		Widget::setSize( _size );
		update();
	}

	void Container::setCoord( const IntCoord& _coord )
	{
		Widget::setCoord( _coord );
		update();
	}
	
} // namespace MyGUI

