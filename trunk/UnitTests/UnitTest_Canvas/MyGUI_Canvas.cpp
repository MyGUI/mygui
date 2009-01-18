/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#include "precompiled.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{	
	Canvas::Canvas( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name )
		:	Widget( _style, _coord, _align, _info, _parent, _croppedParent, _creator, _name ),
			mTexData( 0 ), mTexResizeMode( TRM_PT_CONST_SIZE ), mTexManaged( true )
	{
		mGenTexName = utility::toString( this, "_Canvas" );
	}

	Canvas::~Canvas()
	{
		destroyTexture();

		shutdownWidgetSkin();
	}

	void Canvas::createTexture( TextureResizeMode _resizeMode, Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		createTexture( getSize(), _resizeMode, _usage, _format );
	}

	void Canvas::createTexture( const IntSize & _size, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		if( _size.width <= 0 || _size.height <= 0 )
		{
			MYGUI_ASSERT( 0, "At least one of dimensions isn't positive!" );
			return;
		}

		createTexture( _size.width, _size.height, _resizeMode, _usage, _format );
	}

	void Canvas::createExactTexture( size_t _width, size_t _height, Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		destroyTexture();

		mTexPtr = Ogre::TextureManager::getSingleton().createManual( 
			mGenTexName, 
			ResourceManager::getInstance().getResourceGroup(), 
			Ogre::TEX_TYPE_2D,
			_width, _height, 
			0, _format, 
			_usage, this );

		MYGUI_OUT( "Texture created ", _width, "x", _height,
			". Widget name: ", getName() );

		mTexPtr->load();

		mTexManaged = true;
	}

	void Canvas::resize( const IntSize & _size )
	{
		if( _size.width <= 0 || _size.height <= 0 || ! mTexManaged )
			return;

		mReqTexSize = _size;

		FrameAdvise(true);
	}

	void Canvas::createTexture( size_t _width, size_t _height, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		if( mReqTexSize.empty() )
			mReqTexSize = IntSize( _width, _height );

		mTexResizeMode = _resizeMode;

		bool create = checkCreate( _width, _height );

		validateSize( _width, _height );

		if( create )
			createExactTexture( _width, _height, _usage, _format );
	}

	void Canvas::setSize( const IntSize & _size )
	{
		resize( _size );

		Widget::setSize( _size );
	}

	void Canvas::setCoord( const IntCoord & _coord )
	{
		resize( _coord.size() );

		Widget::setCoord( _coord );
	}

	void Canvas::loadResource( Ogre::Resource* _resource )
	{
		if( mTexPtr.get() == _resource )
		{
			mTexPtr->createInternalResources();

			_setTextureName( mGenTexName );
			correctUV();

			requestUpdateCanvas( this, CE_TEXTURE_RESIZED );
		}
	}

	void Canvas::updateTexture()
	{
		mTexPtr->reload();
	}

	bool Canvas::checkCreate( size_t _width, size_t _height ) const
	{
		if( mTexPtr.isNull() )
			return true;

		if( mTexPtr->getWidth() >= _width && mTexPtr->getHeight() >= _height )
			return false;
	
		return true;
	}

	void Canvas::validateSize( size_t & _width, size_t & _height ) const
	{
		if( mTexResizeMode == TRM_PT_CONST_SIZE || mTexResizeMode == TRM_PT_VIEW_REQUESTED )
		{
			_width = nextPowerOf2( _width );
			_height = nextPowerOf2( _height );
		}
	}

	void Canvas::validate( size_t & _width, size_t & _height, Ogre::TextureUsage & _usage, Ogre::PixelFormat & _format ) const
	{
		validateSize( _width, _height );

		// restore usage and format
		if( ! mTexPtr.isNull() )
		{
			if( _usage == getDefaultTextureUsage() )
				_usage = (Ogre::TextureUsage) mTexPtr->getUsage();

			if( _format == getDefaultTextureFormat() )
				_format = mTexPtr->getFormat();
		}
	}

	void Canvas::destroyTexture()
	{
		if( ! mTexPtr.isNull() )
		{
			mTexPtr->freeInternalResources();
			mTexPtr->unload();

			// delete from resource manager
			Ogre::ResourceManager* parentResourceManager = mTexPtr->getCreator();
			parentResourceManager->remove( mTexPtr->getHandle() );

			mTexPtr.setNull();
		}
	}

	void Canvas::correctUV()
	{
		if( mTexResizeMode == TRM_PT_VIEW_REQUESTED )
		{
			_setUVSet( FloatRect( 0, 0, 
				(Ogre::Real) mReqTexSize.width  / (Ogre::Real) getTextureRealWidth(), 
				(Ogre::Real) mReqTexSize.height / (Ogre::Real) getTextureRealHeight() 
				) );
		}

		if( mTexResizeMode == TRM_PT_CONST_SIZE )
		{
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );
		}
	}

	void* Canvas::lock()
	{
		void* data = getBuffer()->lock( Ogre::HardwareBuffer::HBL_DISCARD );

		mTexData = reinterpret_cast< uint8* >( data );

		return data;
	}

	void Canvas::unlock()
	{
		mTexPtr->getBuffer()->unlock();
	}

	void* Canvas::pointPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( _x <= getTextureRealWidth() && _y <= getTextureRealHeight(), 
			"Access to non-exists pixel! Check real dimensions of texture!" );

		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before point pixel!" );

		size_t _pixelDataSize = Ogre::PixelUtil::getNumElemBytes( mTexPtr->getFormat() );

		MYGUI_ASSERT( _pixelDataSize, "Unknown texture format!" );

		return mTexData + ( _y * getTextureRealWidth() + _x ) * _pixelDataSize;
	}

	void Canvas::setPixel( size_t _x, size_t _y, const Ogre::ColourValue & value )
	{
		MYGUI_ASSERT( _x <= getTextureRealWidth() && _y <= getTextureRealHeight(), 
			"Access to non-exists pixel! Check real dimensions of texture!" );

		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::PixelUtil::packColour( value, getTextureFormat(), pointPixel( _x, _y ) );
	}

	Ogre::ColourValue Canvas::getPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( _x <= getTextureRealWidth() && _y <= getTextureRealHeight(), 
			"Access to non-exists pixel! Check real dimensions of texture!" );

		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::ColourValue result;

		Ogre::PixelUtil::unpackColour( & result, getTextureFormat(), pointPixel( _x, _y ) );

		return result;
	}

	void Canvas::baseChangeWidgetSkin( WidgetSkinInfoPtr _info )
	{
		Widget::baseChangeWidgetSkin( _info );
	}

	void Canvas::initialiseWidgetSkin( WidgetSkinInfoPtr _info )
	{
	}

	void Canvas::shutdownWidgetSkin()
	{
	}

	bool Canvas::isTextureSrcSize() const
	{
		return getTextureSrcSize() == getTextureRealSize();
	}

	void Canvas::loadTexture( Ogre::TexturePtr texture, bool manageMode )
	{
		destroyTexture();

		mTexPtr = texture;

		mTexManaged = manageMode;
	}

	// I'm too lazy to write binary search :)
	size_t Canvas::nextPowerOf2( size_t num )
	{
		size_t cur = 1;
		for( int iter = 1; iter < 32; ++iter, cur *= 2 )
		{
			if( num <= cur )
				break;
		}
		return cur;
	}

	void Canvas::FrameAdvise(bool _advise)
	{
		if (_advise) {
			if (!mFrameAdvise) {
				mFrameAdvise = true;
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &Canvas::frameEntered);
			}
		}
		else {
			if (mFrameAdvise) {
				mFrameAdvise = false;
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &Canvas::frameEntered);
			}
		}
	}

	void Canvas::frameEntered(float _time)
	{
		size_t width = (size_t) mReqTexSize.width;
		size_t height = (size_t) mReqTexSize.height;
		Ogre::TextureUsage usage = getDefaultTextureUsage();
		Ogre::PixelFormat format = getDefaultTextureFormat();

		validate( width, height, usage, format );

		bool create = checkCreate( width, height );
		
		if( mTexResizeMode == TRM_PT_CONST_SIZE )
			create = false;

		if( create )
		{
			createExactTexture( width, height, usage, format );
			correctUV();
		}
		else // I think order is important
		{
			correctUV();
			requestUpdateCanvas( this, CE_WIDGET_RESIZED );
		}

		FrameAdvise(false);
	}

} // namespace MyGUI
