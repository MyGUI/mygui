/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2008
	@module
*//*
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
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ISubWidget.h"
#include <OgreTextureManager.h>
#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	RenderBox::RenderBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mRenderTexture(nullptr),
		mCamera(nullptr),
		mViewport(nullptr),
		mBackgroungColour(Ogre::ColourValue::Blue)
	{
		initialiseWidgetSkin(_info);

		Canvas::eventPreTextureChanges = newDelegate( this, &RenderBox::preTextureChanges );
		Canvas::requestUpdateCanvas = newDelegate( this, &RenderBox::requestUpdateCanvas );
	}

	RenderBox::~RenderBox()
	{
		shutdownWidgetSkin();
	}

	void RenderBox::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void RenderBox::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(nullptr != mMainSkin, "need one subskin");

		mMainSkin->_setUVSet(FloatRect(0, 0, 1, 1));
	}

	void RenderBox::shutdownWidgetSkin()
	{
	}

	void RenderBox::setCamera( Ogre::Camera * _camera )
	{
		removeCamera();

		mCamera = _camera;

		setTextureManaged( true );

		createTexture( TRM_PT_VIEW_ALL, Ogre::TU_RENDERTARGET );

		if (mRenderTexture == nullptr)
			Canvas::updateTexture();
	}

	void RenderBox::removeCamera()
	{
		if( mCamera != nullptr )
		{
			if( isTextureCreated() )
			{
				mRenderTexture->removeViewport( 0 );
				Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );
				mRenderTexture = nullptr;
			}

			setTextureManaged( false );

			mCamera = nullptr;
			mViewport = nullptr;
		}
	}

	void RenderBox::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);
	}

	void RenderBox::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		eventUpdateViewport(this);
	}

	void RenderBox::setCoord(const IntCoord & _coord)
	{
		Base::setCoord(_coord);

		eventUpdateViewport(this);
	}

	void RenderBox::preTextureChanges( MyGUI::CanvasPtr _canvas )
	{
		if( mRenderTexture != nullptr )
		{
			// remove old viewport with 0 z-order
			mRenderTexture->removeViewport( 0 );
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );
			mRenderTexture = nullptr;
		}
	}

	void RenderBox::requestUpdateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent )
	{
		if( mCamera == nullptr )
			return;

		if( mRenderTexture != _canvas->getBuffer()->getRenderTarget() )
		{
			mRenderTexture = _canvas->getBuffer()->getRenderTarget();

			mViewport = mRenderTexture->addViewport( mCamera );
			mViewport->setClearEveryFrame( true );
			mViewport->setOverlaysEnabled( false );
			mViewport->setBackgroundColour(mBackgroungColour);
		}
	}

	void RenderBox::setBackgroungColour(const Ogre::ColourValue & _colour)
	{
		mBackgroungColour = _colour;
		if (mRenderTexture != nullptr)
		{
			Ogre::Viewport* view = mRenderTexture->getViewport(0);
			view->setBackgroundColour(mBackgroungColour);
		}
	}


} // namespace MyGUI
