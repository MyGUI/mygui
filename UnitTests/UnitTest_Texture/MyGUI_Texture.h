/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_TEXTURE_H__
#define __MYGUI_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	class Texture;

	typedef Texture* TexturePtr;

	typedef delegates::CDelegate1<TexturePtr> EventInfo_Texture;

	/** @example "Class Texture usage"
	@code
	// Initializing
	MyGUI::TexturePtr mTexturePtr;
	MyGUI::StaticImagePtr mWallpaperImagePtr;

	void DemoClass::init()
    {
		// Creating
		mTexture = new MyGUI::Texture( "wallpaper0.jpg", "General" );
		mTexture->requestUpdateTexture = MyGUI::newDelegate( this, &DemoClass::requestUpdateTexture );

		mWallpaperImagePtr = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		mWallpaperImagePtr->setImageTexture("wallpaper0.jpg");
	}

	// After lossing device or resizing

	// Update delegate
	void DemoClass::requestUpdateTexture( MyGUI::TexturePtr texture )
    {
		// load the same texture that was..
		texture->loadOriginal();

		mWallpaperImagePtr->setImageTexture( texture->getTexture()->getName() );
	}
	@endcode
	*/

	/**
	 * Wrapper over Ogre::Texture. 
	 * Implemented: resizing by call; recovery after lossing device; set/get _pixel methods for fun :) .
	 */
	class /*MYGUI_EXPORT*/ Texture : public Ogre::ManualResourceLoader
	{
	protected:

		/// Detect position of _pixel in _pixel buffer
		void* pointPixel( const IntPoint & _pixel );

		/// Detect position of _pixel in _pixel buffer
		void* pointPixel( size_t _x, size_t _y );

		// owerriden
		void loadResource( Ogre::Resource* resource );

	public:
		/// Loads texture from image file.
		Texture( const Ogre::String & _imageName, const Ogre::String & _groupName );

		/// Constructs an emty texture.
		Texture( const IntSize & _size, Ogre::PixelFormat _pixelFormat, const Ogre::String & _textureName, const Ogre::String & _groupName );

		/// Call user delegate update and removes old texture if it isn't original.
		void updateData();

		/** Event : Texture instance was changed (May be caused by resizing texture or lossing device). User have to update all references to new instance of texture.\n
			signature : void method(MyGUI::Texture _texture)\n
			@param _texture, which needs to update
		 */
		EventInfo_Texture requestUpdateTexture;

		/// Loads Image with _imageName and place it at top left corner. This is original texture.
		void load( const Ogre::String & _imageName, const Ogre::String & _groupName );

		/// Loads Image with _imageName and place at dst box. This is original texture.
		void load( const Ogre::String & _imageName, const Ogre::String & _groupName, const Ogre::Image::Box & _dst );

		/// load texture with original name.
		void loadOriginal();

		/// Locks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->lock()
		void* lock();

		/// Unlocks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->unlock()
		void unlock();

		/// Checks lockness of hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->isLocked()
		bool isLocked() const;

		/// Sets the _pixel. @remarks Texture buffer must be locked before!
		void setPixel( size_t _x, size_t _y, const Ogre::ColourValue & value );

		/// Sets the _pixel. @remarks Texture buffer must be locked before!
		void setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value );

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( const IntPoint & _pixel );

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( size_t _x, size_t _y );

		/// Returns real width of texture.
		size_t getWidth() const { return mTexture->getWidth(); }

		/// Returns real height of texture.
		size_t getHeight() const { return mTexture->getHeight(); }

		/// Returns needed width while creating texture.
		size_t getSrcWidth() const { return mTexture->getSrcWidth(); }

		/// Returns needed height while creating texture.
		size_t getSrcHeight() const { return mTexture->getSrcHeight(); }

		/// Returns needed sizes while creating texture.
		Ogre::PixelFormat getFormat() const { return mTexture->getFormat(); }

		/// Returns current texture. @remarks this method can be dangerous! It's leaved
		Ogre::TexturePtr getTexture() { return mTexture; }

		/// Returns pointer to old texture. @remarks Valid only while calling user delegate update. \sa mOldTexture
		Ogre::TexturePtr getOldTexture() { return mOldTexture; }

		/// Returns name of the current texture.
		const Ogre::String & getName() const { return mTexture->getName(); }

		/// Returns group of the current texture.
		const Ogre::String & getGroup() const { return mTexture->getGroup(); }

		/// Returns real _size of texture.
		IntSize getSize() const { return IntSize( getWidth(), getHeight() ); }

		/// Returns needed sizes while creating texture.
		IntSize getSrcSize() const { return IntSize( getSrcWidth(), getSrcHeight() ); }

		/// Resize texture if needed dimensions are bigger than current. Delegate update will be called. \sa update.
		void setSize( size_t _width, size_t _height );

		/// Resize texture if needed dimensions are bigger than current. Delegate update will be called. \sa update.
		void setSize( const IntSize & _size );

		/// Retrieves the original name.\sa mOriginalName
		const Ogre::String & getOriginalName() const { return mOriginalName; }

		/// Sets the original name.\sa mOriginalName
		void setOriginalName( const Ogre::String & _name ) { mOriginalName = _name; }

	protected:
		/// Current texture
		Ogre::TexturePtr mTexture;

		/// Pointer to old texture. Valid only while calling user delegate update, other time it's null!
		Ogre::TexturePtr mOldTexture;

		/// Original name means image filename, which from texture was created. \sa setOriginalName,getOriginalName
		Ogre::String mOriginalName;

		/// Caches orignal image.
		Ogre::Image mCachedImage;

		/// Saved pointer from last calling lock. \sa lock
		uint8* mData;

		/// Finds first free texture name. Works so... checks baseName0, baseName1, baseName2... ans so on.
		static void findFreeTextureName( const Ogre::String & _baseName, Ogre::String & _result );

	}; // class Texture

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_H__
