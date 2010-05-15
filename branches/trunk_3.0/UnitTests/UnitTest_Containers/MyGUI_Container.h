/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#ifndef __MYGUI_CONTAINER_H__
#define __MYGUI_CONTAINER_H__

#include "MyGUI_SizeDescription.h"
#include "MyGUI_Spacer.h"

namespace MyGUI
{
	/** Abstract container class
	*/
	class /*MYGUI_EXPORT*/ Container : public Widget
	{
		MYGUI_RTTI_DERIVED( Container )
	protected:
		Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

		/// Attach widget to container
		virtual void add(Widget* widget) = 0;

		/// Detach widget from container
		virtual void remove(Widget* widget) = 0;

		/// Update all widget's positions. You've to do it manyally, because sometimes it's not so fast.
		virtual void update() = 0;

		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);

		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord);

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, const std::string& _name);

	public:	
		
		struct BaseWidgetInfo
		{
			Widget* widget;

			BaseWidgetInfo(Widget* _widget) : widget(_widget){}
		};
	};
} // namespace MyGUI

#endif // __MYGUI_CONTAINER_H__
