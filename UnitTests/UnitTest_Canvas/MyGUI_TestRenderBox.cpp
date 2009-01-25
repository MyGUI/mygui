/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2008
	@module
*/
#include "precompiled.h"
#include "MyGUI_TestRenderBox.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ISubWidget.h"
#include <OgreTextureManager.h>
#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	TestRenderBox::TestRenderBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Canvas(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mRttCam(nullptr),
		mViewport(nullptr),
		mRenderTexture(nullptr)
	{
		initialiseWidgetSkin(_info);

		Canvas::eventPreTextureChanges = newDelegate( this, &TestRenderBox::preTextureChanges );
		Canvas::requestUpdateCanvas = newDelegate( this, &TestRenderBox::requestUpdateCanvas );
	}

	TestRenderBox::~TestRenderBox()
	{
		shutdownWidgetSkin();
	}

	void TestRenderBox::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void TestRenderBox::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(nullptr != mMainSkin, "need one subskin");

		mMainSkin->_setUVSet(FloatRect(0, 0, 1, 1));
	}

	void TestRenderBox::shutdownWidgetSkin()
	{
	}

	void TestRenderBox::setCamera( Ogre::Camera * _camera )
	{	
		removeCamera();

		mRttCam = _camera;

		setTextureManaged( true );

		createTexture( getSize(), TRM_PT_VIEW_ALL, Ogre::TU_RENDERTARGET );

		if( mRenderTexture == nullptr )
			Canvas::updateTexture();
	}

	void TestRenderBox::removeCamera()
	{
		if( mRttCam != nullptr )
		{
			if( isTextureCreated() )
			{
				mRenderTexture->removeViewport( 0 );
				Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );
				mRenderTexture = nullptr;
			}

			setTextureManaged( false );

			mRttCam = nullptr;
		}
	}

	void TestRenderBox::setPosition(const IntPoint & _point)
	{
		Canvas::setPosition(_point);
	}

	void TestRenderBox::setSize(const IntSize& _size)
	{
		Canvas::setSize(_size);
		updateViewport();
	}

	void TestRenderBox::setCoord(const IntCoord & _coord)
	{
		Canvas::setCoord(_coord);
		updateViewport();
	}

	void TestRenderBox::frameEntered(float _time)
	{
	}

	// на всяк крайняк
	void TestRenderBox::updateViewport()
	{
		// при нуле вылетает
		if ((getWidth() <= 1) || (getHeight() <= 1) ) return;
	}

	bool TestRenderBox::getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen)
	{
		Ogre::Matrix4 mat = (mRttCam->getProjectionMatrix() * mRttCam->getViewMatrix(true));
		Ogre::Vector4 Point = mat * Ogre::Vector4(_world.x, _world.y, _world.z, 1);
		_screen.x = (Point.x / Point.w + 1) * 0.5;
		_screen.y = 1 - (Point.y / Point.w + 1) * 0.5;
		float Depth = Point.z / Point.w;
		return (Depth >= 0.0f && Depth <= 1.0f);
	}

	void TestRenderBox::preTextureChanges( MyGUI::CanvasPtr _canvas )
	{
		if( mRenderTexture != nullptr )
		{
			// remove old viewport with 0 z-order
			mRenderTexture->removeViewport( 0 );
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );
			mRenderTexture = nullptr;
		}
	}

	void TestRenderBox::requestUpdateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent )
	{
		if( mRttCam == nullptr )
			return;

		if( mRenderTexture != _canvas->getBuffer()->getRenderTarget() )
		{
			mRenderTexture = _canvas->getBuffer()->getRenderTarget();

			mViewport = mRenderTexture->addViewport( mRttCam );
			mViewport->setClearEveryFrame( true );
			mViewport->setOverlaysEnabled( false );
		}
	}


} // namespace MyGUI
