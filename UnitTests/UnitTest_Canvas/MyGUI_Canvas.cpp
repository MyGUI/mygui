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
			mTexData( 0 ), mUseCache( false ), mTexResizeMode( RM_EXACT_REQUEST )
	{
		mTexName = utility::toString( this, "_Canvas" );

		initialiseWidgetSkin( _info );
	}

	Canvas::~Canvas()
	{
		destroyTexture();
		shutdownWidgetSkin();
	}

	void Canvas::createTexture( Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		mTexResizeMode = RM_POWER_OF_TWO;

		createTexture( getSize().width, getSize().height );
	}

	void Canvas::createTexture( size_t _width, size_t _height, Ogre::TextureUsage _usage, Ogre::PixelFormat _format )
	{
		bool create = true;

		if( ! mTexPtr.isNull() )
		{
			if( mTexPtr->getWidth() >= _width && mTexPtr->getHeight() >= _height )
				create = false;
		}

		if( mReqTexSize.empty() )
			mReqTexSize = IntSize( _width, _height );

		if( create && mTexResizeMode == RM_POWER_OF_TWO )
		{
			_width = nextPowerOf2( _width );
			_height = nextPowerOf2( _height );
		}

		if( create )
		{
			destroyTexture();

			mTexPtr = Ogre::TextureManager::getSingleton().createManual( 
				mTexName, 
				ResourceManager::getInstance().getResourceGroup(), 
				Ogre::TEX_TYPE_2D,
				_width, _height, 
				0, _format, 
				_usage, this );

			// calls updateTexture
			mTexPtr->load();
		}
	}

	void Canvas::correctUV()
	{
		if( mTexResizeMode == RM_EXACT_REQUEST )
		{
			_setUVSet( FloatRect( 0, 0, 
				(Ogre::Real)mReqTexSize.width / (Ogre::Real)getTextureRealWidth(), 
				(Ogre::Real)mReqTexSize.height / (Ogre::Real)getTextureRealHeight() 
				) );
		}

		if( mTexResizeMode == RM_POWER_OF_TWO )
		{
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );
		}
	}

	void Canvas::updateTexture()
	{
		mTexPtr->unload();
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

	void Canvas::setSize( const IntSize & _size )
	{
		mReqTexSize = _size;

		Widget::setSize( _size );

		createTexture( mReqTexSize.width, mReqTexSize.height );

		correctUV();
	}

	void Canvas::setCoord(const IntCoord & _coord)
	{
		mReqTexSize = _coord.size();

		Widget::setCoord( _coord );

		createTexture( mReqTexSize.width, mReqTexSize.height );

		correctUV();
	}

	void Canvas::loadResource( Ogre::Resource* _resource )
	{
		if( mTexPtr.get() == _resource )
		{
			mTexPtr->createInternalResources();

			_setTextureName( mTexName );
			correctUV();

			requestUpdateTexture( this );

			updateCache();
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

		updateCache();
	}

	void Canvas::updateCache()
	{
		if( mTexPtr.isNull() || ! mUseCache )
			return;

		bool create = false;

		if( mCache.getWidth() != getTextureRealWidth() || mCache.getWidth() != getTextureRealHeight() || ! mCache.getSize() )
			create = true;

		if( create )
		{
			size_t size = getBuffer()->getSizeInBytes();

			Ogre::uint8* rawData = OGRE_ALLOC_T( Ogre::uint8, size, Ogre::MEMCATEGORY_GENERAL );

			memcpy( rawData, mTexPtr->getBuffer()->lock( Ogre::HardwareBuffer::HBL_DISCARD ), size );

			mTexPtr->getBuffer()->unlock();

			// say image auto delete raw data
			mCache.loadDynamicImage( rawData, getTextureRealWidth(), getTextureRealHeight(), 1, getTextureFormat(), true );
		}
		else
			getBuffer()->blitToMemory( mCache.getPixelBox() );
	}

	void Canvas::setCacheUse( bool _cache )
	{
		mUseCache = _cache;
	}

	void Canvas::restoreFromCache()
	{
		restoreFromCache( Ogre::Image::Box( 0, 0, mCache.getWidth(), mCache.getHeight() ) );
	}

	void Canvas::restoreFromCache( const Ogre::Image::Box & _copyTo )
	{
		if( ! mUseCache )
			return;

		Ogre::PixelBox image( 
			mCache.getWidth(), 
			mCache.getHeight(),
			mCache.getDepth(),
			mCache.getFormat(), 
			mCache.getData() );

		getBuffer()->blitFromMemory( image, _copyTo );
	}

	void Canvas::restoreFromCacheResampled( Ogre::Image::Filter _filter )
	{
		if( ! mUseCache )
			return;

		Ogre::Image resizedImage = mCache;
		resizedImage.resize( getTextureRealWidth(), getTextureRealHeight(), _filter );

		Ogre::PixelBox resizedImageBox( 
			resizedImage.getWidth(), 
			resizedImage.getHeight(),
			resizedImage.getDepth(),
			resizedImage.getFormat(), 
			resizedImage.getData() );

		getBuffer()->blitFromMemory( resizedImageBox, Ogre::Image::Box( 0, 0, resizedImage.getWidth(), resizedImage.getHeight() ) );
	}

	void Canvas::restoreFromCacheResampled( const Ogre::Image::Box & _copyTo, Ogre::Image::Filter _filter )
	{
		if( ! mUseCache )
			return;

		Ogre::Image resizedImage = mCache;
		resizedImage.resize( getTextureRealWidth(), getTextureRealHeight(), _filter );

		Ogre::PixelBox resizedImageBox( 
			resizedImage.getWidth(), 
			resizedImage.getHeight(),
			resizedImage.getDepth(),
			resizedImage.getFormat(), 
			resizedImage.getData() );

		getBuffer()->blitFromMemory( resizedImageBox, _copyTo );
	}

	void* Canvas::pointPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before point pixel!" );

		size_t _pixelDataSize = Ogre::PixelUtil::getNumElemBytes( mTexPtr->getFormat() );

		MYGUI_ASSERT( _pixelDataSize, "Unknown texture format!" );

		return mTexData + ( _y * getTextureRealWidth() + _x ) * _pixelDataSize;
	}

	void Canvas::setPixel( size_t _x, size_t _y, const Ogre::ColourValue & value )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::PixelUtil::packColour( value, getTextureFormat(), pointPixel( _x, _y ) );
	}

	Ogre::ColourValue Canvas::getPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::ColourValue result;

		Ogre::PixelUtil::unpackColour( & result, getTextureFormat(), pointPixel( _x, _y ) );

		return result;
	}

	void Canvas::baseChangeWidgetSkin( WidgetSkinInfoPtr _info )
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin( _info );
		initialiseWidgetSkin( _info );
	}

	void Canvas::initialiseWidgetSkin( WidgetSkinInfoPtr _info )
	{
	}

	void Canvas::shutdownWidgetSkin()
	{
	}

	void Canvas::loadFromFile( const std::string & fileName )
	{
		Ogre::Image image;
		image.load( fileName, ResourceManager::getInstance().getResourceGroup() );

		Ogre::PixelBox imageBox( 
			image.getWidth(), 
			image.getHeight(),
			image.getDepth(),
			image.getFormat(), 
			image.getData() );

		getBuffer()->blitFromMemory( imageBox, Ogre::Image::Box( 0, 0, std::min( image.getWidth(), getTextureRealWidth() ), std::min( image.getHeight(), getTextureRealWidth() ) ) );
	
	}

	bool Canvas::isTextureSrcSize() const
	{
		return getTextureSrcSize() == getTextureRealSize();
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

} // namespace MyGUI
