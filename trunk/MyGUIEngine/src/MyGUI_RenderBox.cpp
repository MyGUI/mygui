/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2008
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderBox.h"

namespace MyGUI
{

	RenderBox::RenderBox() :
		mViewport(nullptr),
		mBackgroundColour(Colour::Blue),
		mChange(false)
	{
		Canvas::eventPreTextureChanges = newDelegate( this, &RenderBox::preTextureChanges );
		Canvas::requestUpdateCanvas = newDelegate( this, &RenderBox::requestUpdateCanvas );
	}

	void RenderBox::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		initialiseWidgetSkin(_info);
	}

	RenderBox::~RenderBox()
	{
		shutdownWidgetSkin();
	}

	void RenderBox::baseChangeWidgetSkin(ResourceSkin* _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void RenderBox::initialiseWidgetSkin(ResourceSkin* _info)
	{
	}

	void RenderBox::shutdownWidgetSkin()
	{
	}

	void RenderBox::setViewport(IViewport* _viewport)
	{
		removeViewport();

		mViewport = _viewport;

		mChange = true;

		setTextureManaged( true );

		createTexture( TRM_PT_VIEW_ALL, TextureUsage::RenderTarget );

		this->updateTexture();
	}

	void RenderBox::removeViewport()
	{
		if ( mViewport != nullptr )
		{
			ITexture* texture = getTexture();
			texture->removeViewport();

			setTextureManaged( false );

			mViewport = nullptr;
		}
	}

	void RenderBox::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void RenderBox::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		eventUpdateViewport(this);
	}

	void RenderBox::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		eventUpdateViewport(this);
	}

	void RenderBox::preTextureChanges( MyGUI::CanvasPtr _canvas )
	{
		ITexture* texture = getTexture();
		texture->removeViewport();

		mChange = true;
	}

	void RenderBox::requestUpdateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent )
	{
		if (mViewport && mChange)
		{
			ITexture* texture = getTexture();
			texture->setViewport(mViewport);
			mViewport->setBackgroundColour(mBackgroundColour);

			mChange = false;
		}
	}

	void RenderBox::setBackgroundColour(const Colour& _colour)
	{
		mBackgroundColour = _colour;

		if (mViewport)
		{
			mViewport->setBackgroundColour(mBackgroundColour);
		}
	}

	void RenderBox::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "RenderBox_BackgroundColour") setBackgroundColour(Colour::parse(_value));

#ifndef MYGUI_DONT_USE_OBSOLETE
		else if (_key == "RenderBox_BackgroungColour")
		{
			MYGUI_LOG(Warning, "RenderBox_BackgroungColour is obsolete, use RenderBox_BackgroundColour");
			setBackgroundColour(Colour::parse(_value));
		}
#endif // MYGUI_DONT_USE_OBSOLETE

		else Base::setProperty(_key, _value);
	}

} // namespace MyGUI
