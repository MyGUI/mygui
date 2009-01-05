/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#include "precompiled.h"
#include "MyGUI_Texture.h"

namespace MyGUI
{
	Texture::Texture( const Ogre::String & _imageName, const Ogre::String & _groupName )
		:	mData( 0 )
	{
		mCachedImage.load( _imageName, _groupName );
		mOriginalName = _imageName;

		mTexture = Ogre::TextureManager::getSingleton().createManual( _imageName, _groupName, Ogre::TEX_TYPE_2D,
			mCachedImage.getWidth(), mCachedImage.getHeight(), 0, mCachedImage.getFormat(), Ogre::TU_DEFAULT, this );

		load( _imageName, _groupName );
	}

	Texture::Texture( const IntSize & _size, Ogre::PixelFormat _pixelFormat, const Ogre::String & _textureName, const Ogre::String & _groupName )
		:	mData( 0 )
	{
		mTexture = Ogre::TextureManager::getSingleton().createManual( _textureName, _groupName, Ogre::TEX_TYPE_2D,
			_size.width, _size.height, 0, _pixelFormat, Ogre::TU_DEFAULT, this );

		mOriginalName = _textureName;
	}

	void Texture::load( const Ogre::String & _imageName, const Ogre::String & _groupName )
	{
		if( mOriginalName != _imageName )
		{
			mCachedImage.load( _imageName, _groupName );
			mOriginalName = _imageName;
		}

		Ogre::Image::Box copy( 0, 0, mCachedImage.getWidth(), mCachedImage.getHeight() );

		load( _imageName, _groupName, copy );
	}

	void Texture::load( const Ogre::String & _imageName, const Ogre::String & _groupName, const Ogre::Image::Box & _dst )
	{
		if( mOriginalName != _imageName )
		{
			mCachedImage.load( _imageName, _groupName );
			mOriginalName = _imageName;
		}

		Ogre::PixelBox image( 
			mCachedImage.getWidth(), 
			mCachedImage.getHeight(),
			mCachedImage.getDepth(),
			mCachedImage.getFormat(), 
			mCachedImage.getData() );

		mTexture->getBuffer()->blitFromMemory( image, _dst );
	}

	void Texture::loadOriginal()
	{
		load( mOriginalName, getGroup() );
	}

	void Texture::updateData()
	{
		// initialize texture
		mTexture->createInternalResources();

		updateTexture( this );

		// All user pointers were changed, we can free the old texture
		if( !mOldTexture.isNull() && mOldTexture->getName() != mOriginalName )
		{
			mOldTexture->unload();

			// delete from resource manager
			Ogre::ResourceManager* resMgr = mOldTexture->getCreator();
			resMgr->remove( mOldTexture->getHandle() );

			if( ! mOldTexture.unique() )
				MYGUI_ERROR( true, "Old texture can't be free, because you leaved the pointers to it!" );	
		}
	}

	void Texture::setSize( int _width, int _height )
	{
		// Compare needed sizes with current real sizes.
		if( _width > getWidth() || _height > getHeight() )
		{
			Ogre::String name;

			if( mOriginalName.empty() )
				mOriginalName = mTexture->getName();

			findFreeTextureName( mOriginalName, name );

			mOldTexture = mTexture;

			mTexture = Ogre::TextureManager::getSingleton().createManual( name, mTexture->getGroup(), Ogre::TEX_TYPE_2D,
				_width, _height, 0, mTexture->getFormat(), Ogre::TU_DEFAULT, this );

			// calls updateTexture
			mTexture->load();

			mOldTexture.setNull();
		}
	}

	void Texture::findFreeTextureName( const Ogre::String & _baseName, Ogre::String & _result )
	{
		bool nameExists = true;

		int iter = 0;

		do
		{
			result = baseName + Ogre::StringConverter::toString( iter );

			nameExists = ! Ogre::TextureManager::getSingleton().getByName( result ).isNull();

			iter++;
		} 
		while( nameExists );
	}

	void Texture::loadResource( Ogre::Resource* resource )
	{
		if( mTexture.get() == resource )
		{
			updateData();
		}
	}

	bool Texture::isLocked() const
	{
		return mTexture->getBuffer()->isLocked();
	}

	void* Texture::lock()
	{
		void * data = mTexture->getBuffer()->lock( Ogre::HardwareBuffer::HBL_DISCARD );

		mData = reinterpret_cast< uint8* >( data );

		return data;
	}

	void Texture::unlock()
	{
		mTexture->getBuffer()->unlock();
	}

	void* Texture::pointPixel( int _x, int _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Texture before point _pixel!" );

		size_t _pixelSize = Ogre::PixelUtil::getNumElemBytes( mTexture->getFormat() );

		if( ! _pixelSize )
			MYGUI_ERROR( true, "Unknown texture format!" );

		return mData + ( ( size_t ) y * mTexture->getWidth() + x ) * _pixelSize;
	}

	void* Texture::pointPixel( const IntPoint & _pixel )
	{
		return pointPixel( _pixel.left, _pixel.top );
	}

	void Texture::setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value )
	{
		setPixel( _pixel.left, _pixel.top, value );
	}

	void Texture::setPixel( int _x, int _y, const Ogre::ColourValue & value )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Texture before set _pixel!" );

		Ogre::PixelUtil::packColour( value, getFormat(), pointPixel( _x, _y ) );
	}

	Ogre::ColourValue Texture::getPixel( const IntPoint & _pixel )
	{
		return getPixel( _pixel.left, _pixel.top );
	}

	Ogre::ColourValue Texture::getPixel( int _x, int _y )
	{
		MYGUI_ASSERT( isLocked(), "Must lock MyGUI::Texture before set _pixel!" );

		Ogre::ColourValue result;

		Ogre::PixelUtil::unpackColour( & result, getFormat(), pointPixel( _x, _y ) );

		return result;
	}

	void Texture::setSize( const IntSize & _size )
	{
		setSize( _size.width, _size.height );
	}

} // namespace MyGUI
