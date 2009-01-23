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
		mUserViewport(true),
		mRttCam(nullptr),
		mViewport(nullptr),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mScale(1.0f),
		mCurrentScale(1.0f),
		mUseScale(false)
	{
		initialiseWidgetSkin(_info);

		Canvas::requestUpdateCanvas = newDelegate( this, &TestRenderBox::updateTexture );
	}

	TestRenderBox::~TestRenderBox()
	{
		//Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
		clear();

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
		// первоначальна€ инициализаци€
		MYGUI_DEBUG_ASSERT(nullptr != mMainSkin, "need one subskin");

		// сохран€ем оригинальный курсор
		mPointerKeeper = mPointer;
		mPointer.clear();

		mMainSkin->_setUVSet(FloatRect(0, 0, 1, 1));
		//createRenderTexture();
	}

	void TestRenderBox::shutdownWidgetSkin()
	{

	}

	// очищает сцену
	void TestRenderBox::clear()
	{
	}

	void TestRenderBox::setBackgroungColour(const Ogre::ColourValue & _colour)
	{
		if (false == mUserViewport){
			mBackgroungColour = _colour;
			Ogre::Viewport *v = mRenderTexture->getViewport(0);
			v->setBackgroundColour(mBackgroungColour);
		}
	}

	void TestRenderBox::setViewScale(bool _scale)
	{
		if (mUseScale == _scale) return;
		if (needFrameUpdate())
		{
			mUseScale = _scale;
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &TestRenderBox::frameEntered);
		}
		else
		{
			mUseScale = _scale;
			if (needFrameUpdate()) Gui::getInstance().eventFrameStart += newDelegate(this, &TestRenderBox::frameEntered);
			//if (needFrameUpdate()) Gui::getInstance().addFrameListener(newDelegate(this, &TestRenderBox::frameEntered), this);
		}
	}

	void TestRenderBox::setRenderTarget(Ogre::Camera * _camera)
	{
		// полна€ очистка
		clear();
		mPointer = "";

		Ogre::Root * root = Ogre::Root::getSingletonPtr();

		// создаем новый материал
		mUserViewport = true;

		mRttCam = _camera;

		//mRttCam->getFrustumExtents( mRttCamSrcRect.left, mRttCamSrcRect.right, mRttCamSrcRect.top, mRttCamSrcRect.bottom );


		if( isTextureCreated() )
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );

		createTexture( getSize(), TRM_PT_VIEW_REQUESTED, Ogre::TU_RENDERTARGET );

		mRenderTexture = getBuffer()->getRenderTarget();
		mRenderTexture->addListener( this );

		// remove old viewport with 0 z-order
		mRenderTexture->removeViewport( 0 );

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setClearEveryFrame( true );
		mViewport->setOverlaysEnabled(false);
	}

	void TestRenderBox::setPosition(const IntPoint & _point)
	{
		Canvas::setPosition(_point);
	}

	void TestRenderBox::setSize(const IntSize& _size)
	{
		Canvas::setSize(_size);
		MYGUI_OUT( getSize() );
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

	void TestRenderBox::onMouseDrag(int _left, int _top)
	{
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::onMouseDrag(_left, _top);
	}

	void TestRenderBox::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void TestRenderBox::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::onMouseButtonReleased(_left, _top, _id);
	}

	void TestRenderBox::preRenderTargetUpdate( const Ogre::RenderTargetEvent & evt )
	{
		// save
		mSaveCamAspect = mRttCam->getAspectRatio();

		//mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

		mRttCam->getViewport()->setDimensions( 0, 0, 1, 1 );
		
		//Ogre::Root::getRenderSystem()->
	}

	void TestRenderBox::postRenderTargetUpdate( const Ogre::RenderTargetEvent & evt )
	{
		//restore
		mRttCam->setAspectRatio( mSaveCamAspect );
		mRttCam->getViewport()->setDimensions( 0, 0, 1, 1 );
	}

	void TestRenderBox::updateViewport()
	{
		// при нуле вылетает
		if ((getWidth() <= 1) || (getHeight() <= 1) ) return;

		if ( nullptr != mRttCam ) {
			// не €сно, нужно ли раст€гивать камеру, установленную юзером
			//mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());
		}

		Canvas::_setUVSet( FloatRect( 0, 0, 1,1 ) );

		if( mViewport != nullptr )
		{
			mViewport->setDimensions( 0, 0, 1, 1 );
			return;

			Ogre::Real dwc = (Ogre::Real) getTextureRealWidth() - (Ogre::Real) getWidth();
			Ogre::Real dhc = (Ogre::Real) getTextureRealHeight() - (Ogre::Real) getHeight();

			dwc = -dwc / 2 / mViewport->getActualWidth();
			dhc = -dhc / 2 / mViewport->getActualHeight();

			Ogre::RealRect set;

			set.left = mViewportRect.left + dwc;
			set.top = mViewportRect.top + dhc;
			set.right = set.left + mViewportRect.width() * (Ogre::Real)getWidth() / (Ogre::Real)getTextureRealWidth();
			set.bottom = set.top + mViewportRect.height() * (Ogre::Real)getHeight() / (Ogre::Real)getTextureRealHeight();

#define CHECK_RANGE(v) MYGUI_ASSERT( 0 <= v && v <= 1, "Out of range" );

			CHECK_RANGE( set.left );
			CHECK_RANGE( set.top );
			CHECK_RANGE( set.right );
			CHECK_RANGE( set.bottom );

#undef CHECK_RANGE
			MYGUI_OUT( "N:", set.left, " ", set.top, " ", set.width(), " ", set.height()  );

			mViewport->setDimensions( 
				set.left, set.top,
				set.width(), set.height() );

			mViewport->setClearEveryFrame( true );

			mViewport->_updateDimensions();
		}
	}

	void TestRenderBox::onMouseWheel(int _rel)
	{
		if ( ! mUseScale) return;

		const float near_min = 0.5f;
		const float coef = 0.0005;

		mScale += (-_rel) * coef;

		if (mScale > 1) mScale = 1;
		else if (mScale < near_min) mScale = near_min;

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

	void TestRenderBox::updateTexture( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent )
	{
		mRenderTexture = _canvas->getBuffer()->getRenderTarget();
		mRenderTexture->addListener( this );

		// remove old viewport with 0 z-order
		mRenderTexture->removeViewport( 0 );

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setClearEveryFrame( true );
		mViewport->setOverlaysEnabled( false );

		
		//mViewportRect.left = mViewport->getLeft();
		//mViewportRect.top = mViewport->getTop();
		//mViewportRect.right = mViewport->getLeft() + mViewport->getWidth();
		//mViewportRect.bottom = mViewport->getTop() + mViewport->getHeight();
		//MYGUI_OUT( "S:", mViewportRect.left, " ", mViewportRect.top, " ", mViewportRect.width(), " ", mViewportRect.height() );

	}


} // namespace MyGUI
