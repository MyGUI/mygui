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
			mTexData( 0 ), mUseCache( false ), mTexResizeMode( RM_EXACT_RESIZING )
	{
		mTexName = utility::toString( this, "_Canvas" );

		initialiseWidgetSkin( _info );

		_setUVSet( FloatRect( 0, 0, 1, 1 ) );
	}

	Canvas::~Canvas()
	{
		destroyTexture();
		shutdownWidgetSkin();
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

	/*void Canvas::loadFromFile( const std::string & fileName )
	{
		Ogre::Image image;
		image.load( fileName, ResourceManager::getInstance().getResourceGroup() );

		Ogre::PixelBox imageBox( 
			image.getWidth(), 
			image.getHeight(),
			image.getDepth(),
			image.getFormat(), 
			image.getData() );

		getBuffer()->blitFromMemory( imageBox, Ogre::Image::Box( 0, 0, std::min( image.getWidth(), getWidth() ), std::min( image.getHeight(), getHeight() ) ) );
	
	}*/

	void Canvas::createTexture( size_t _width, size_t _height, Ogre::PixelFormat _format, Ogre::TextureUsage _usage )
	{
		bool create = true;

		if( ! mTexPtr.isNull() )
		{
			if( mTexPtr->getWidth() >= _width
			&&
				mTexPtr->getHeight() >= _height )
			{
				create = false;
			}
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
		Widget::setSize( _size );

		if( mTexResizeMode == RM_EXACT_RESIZING )
		{
			createTexture( _size.width, _size.height );

			// if we make texture smaller
			if( (size_t) _size.width < getWidth() 
			 || (size_t) _size.height < getHeight() )
			{
				_setUVSet( 
					FloatRect( 0, 0, 
					(Ogre::Real)_size.width / (Ogre::Real)getWidth(), 
					(Ogre::Real)_size.height / (Ogre::Real)getHeight() 
					) );
			}
			else
			{
				_setUVSet( FloatRect( 0, 0, 1, 1 ) );
			}
		}
		else 
			if( mTexResizeMode == RM_NO_RESIZING )
		{
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );
		}
	}

	void Canvas::setSize( size_t _width, size_t _height )
	{
		setSize( IntSize( _width, _height ) );
	}

	void Canvas::loadResource( Ogre::Resource* _resource )
	{
		if( mTexPtr.get() == _resource )
		{
			mTexPtr->createInternalResources();

			_setTextureName( mTexName );
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );

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

		if( mCache.getWidth() != getWidth() || mCache.getWidth() != getHeight() || ! mCache.getSize() )
			create = true;

		if( create )
		{
			size_t size = getBuffer()->getSizeInBytes();

			Ogre::uint8* rawData = OGRE_ALLOC_T( Ogre::uint8, size, Ogre::MEMCATEGORY_GENERAL );

			memcpy( rawData, mTexPtr->getBuffer()->lock( Ogre::HardwareBuffer::HBL_DISCARD ), size );

			mTexPtr->getBuffer()->unlock();

			// say image auto delete raw data
			mCache.loadDynamicImage( rawData, getWidth(), getHeight(), 1, getFormat(), true );
		}
		else
			getBuffer()->blitToMemory( mCache.getPixelBox() );
	}

	void Canvas::setCacheUse( bool _cache )
	{
		mUseCache = _cache;

		//if( ! mUseCache )
		//	mCache.resize( 0, 0, Ogre::Image::FILTER_LINEAR );
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

		getBuffer()->blitFromMemory( image, Ogre::Image::Box( 0, 0, mCache.getWidth(), mCache.getHeight() ) );
	}

	void* Canvas::pointPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before point pixel!" );

		size_t _pixelDataSize = Ogre::PixelUtil::getNumElemBytes( mTexPtr->getFormat() );

		MYGUI_ASSERT( ! _pixelDataSize , "Unknown texture format!" );

		return mTexData + ( _y * getWidth() + _x ) * _pixelDataSize;
	}

	void* Canvas::pointPixel( const IntPoint & _pixel )
	{
		return pointPixel( _pixel.left, _pixel.top );
	}

	void Canvas::setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value )
	{
		setPixel( _pixel.left, _pixel.top, value );
	}

	void Canvas::setPixel( size_t _x, size_t _y, const Ogre::ColourValue & value )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::PixelUtil::packColour( value, getFormat(), pointPixel( _x, _y ) );
	}

	Ogre::ColourValue Canvas::getPixel( const IntPoint & _pixel )
	{
		return getPixel( _pixel.left, _pixel.top );
	}

	Ogre::ColourValue Canvas::getPixel( size_t _x, size_t _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Canvas before set pixel!" );

		Ogre::ColourValue result;

		Ogre::PixelUtil::unpackColour( & result, getFormat(), pointPixel( _x, _y ) );

		return result;
	}

	void Canvas::createTexture( const IntSize & _size, Ogre::PixelFormat _format, Ogre::TextureUsage _usage )
	{
		createTexture( _size.width, _size.height, _format, _usage );
	}

} // namespace MyGUI
