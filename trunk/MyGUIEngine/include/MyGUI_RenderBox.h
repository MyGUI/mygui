/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RENDER_BOX_H__
#define __MYGUI_RENDER_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"

namespace MyGUI
{

	typedef delegates::CDelegate1<RenderBoxPtr> EventInfo_RenderBoxPtr;

	/** @brief Widget that show one entity or anything from viewport.

		Set your own Ogre::Camera and you'll see anything from your viewport.
	*/
	class MYGUI_EXPORT RenderBox :
		public Canvas
	{
		MYGUI_RTTI_DERIVED( RenderBox );

	public:
		RenderBox();

		/** Set any user created viewport */
		void setViewport(IViewport* _value);

		/** Removes viewport. Renders nothing */
		void removeViewport();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }


		/** Set colour behind entity.*/
		void setBackgroundColour(const Colour& _value);
		/** Get colour behind entity.*/
		const Colour& getBackgroundColour() { return mBackgroundColour; }

		/** @copydoc Widget::setProperty(const std::string& _key, const std::string& _value) */
		virtual void setProperty(const std::string& _key, const std::string& _value);


	/*event:*/
		/** Event : Viewport size was changed\n
			signature : void method(MyGUI::RenderBoxPtr _sender)\n
			@param _sender
		 */
		EventInfo_RenderBoxPtr eventUpdateViewport;

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

	protected:
		virtual ~RenderBox();

		void baseChangeWidgetSkin(ResourceSkin* _info);

		void preTextureChanges( MyGUI::CanvasPtr _canvas );

		void requestUpdateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent );

	private:
		void initialiseWidgetSkin(ResourceSkin* _info);
		void shutdownWidgetSkin();

	private:
		IViewport* mViewport;
		Colour mBackgroundColour;
		bool mChange;

	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_H__
