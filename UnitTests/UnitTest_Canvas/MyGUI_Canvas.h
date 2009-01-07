/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_CANVAS_H__
#define __MYGUI_CANVAS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	namespace factory
	{
		class CanvasFactory;
	}
	class Canvas;

	typedef Canvas* CanvasPtr;
	
	typedef delegates::CDelegate1<CanvasPtr> EventInfo_Canvas;

	/**
	 * Wrapper over Ogre::Texture. 
	 * Implemented: resizing by call; recovery after lossing device; set/get pixel methods for fun :) .
	 */
	class /*MYGUI_EXPORT*/ Canvas : public Widget, public Ogre::ManualResourceLoader
	{
		// для вызова закрытого конструктора
		friend class factory::CanvasFactory;

		MYGUI_RTTI_CHILD_HEADER( Canvas, Widget );

	public:
		enum ResizeMode
		{
			RM_EXACT_REQUEST,
			RM_POWER_OF_TWO,
		};

	public:

		/// Creates texture with non-resizing mode woth sizes
		void createTexture( Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 );
		
		void createTexture( size_t _width, size_t _height, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 );

		void createTexture( const IntSize & _size, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 ) { createTexture( _size.width, _size.height, _usage, _format ); }

		void destroyTexture();

		// это под нож... для отладки только было
		void loadFromFile( const std::string & fileName );

		/// Call user delegate update and removes old texture if it isn't original.
		void updateTexture();

		void restoreFromCache();

		void restoreFromCache( const Ogre::Image::Box & _copyTo );

		// пока ввёл - потом снесу, наверное
		void restoreFromCacheResampled( Ogre::Image::Filter _filter );
		void restoreFromCacheResampled( const Ogre::Image::Box & _copyTo, Ogre::Image::Filter _filter );

		/** Event : Texture instance was changed (May be caused by resizing texture or lossing device). User have to update all references to new instance of texture.\n
			signature : void method(MyGUI::Texture _texture)\n
			@param _texture, which needs to update
		 */
		EventInfo_Canvas requestUpdateTexture;

		/// Locks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->lock()
		void* lock();

		/// Unlocks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->unlock()
		void unlock();

		/// Checks lockness of hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->isLocked()
		bool isLocked() const { return mTexPtr->getBuffer()->isLocked(); }

		Ogre::HardwarePixelBufferSharedPtr getBuffer() { return mTexPtr->getBuffer(); }

		/// Sets the _pixel. @remarks Texture buffer must be locked before!
		void setPixel( size_t _x, size_t _y, const Ogre::ColourValue & value );

		/// Sets the _pixel. @remarks Texture buffer must be locked before!
		void setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value ){ setPixel( _pixel.left, _pixel.top, value ); }

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( const IntPoint & _pixel ) { return getPixel( _pixel.left, _pixel.top ); }

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( size_t _x, size_t _y );

		/// Returns real width of texture.
		size_t getTextureRealWidth() const { return mTexPtr->getWidth(); }

		/// Returns real height of texture.
		size_t getTextureRealHeight() const { return mTexPtr->getHeight(); }

		/// Returns real _size of texture.
		IntSize getTextureRealSize() const { return IntSize( getTextureRealWidth(), getTextureRealHeight() ); }

		/// Returns needed width while creating texture.
		size_t getTextureSrcWidth() const { return mReqTexSize.width; }

		/// Returns needed height while creating texture.
		size_t getTextureSrcHeight() const { return mReqTexSize.height; }

		/// Returns needed sizes while creating texture.
		IntSize getTextureSrcSize() const { return mReqTexSize; }

		/// Returns needed sizes while creating texture.
		Ogre::PixelFormat getTextureFormat() const { return mTexPtr->getFormat(); }

		/// Returns name of the current texture.
		const std::string & getTextureName() const { return mTexPtr->getName(); }

		/// Returns group of the current texture.
		const std::string & getTextureGroup() const { return mTexPtr->getGroup(); }

		
		/// Overriden. Resize texture if needed dimensions are bigger than current. Delegate update will be called. \sa update.
		void setSize( const IntSize & _size );

		void setSize( size_t _width, size_t _height );

		ResizeMode getResizeMode() const { return mTexResizeMode; }

		void setResizeMode( ResizeMode _set ) { mTexResizeMode = _set; }

		void setCacheUse( bool _cache );

		bool getCacheUse() const { return mUseCache; }

		const Ogre::Image& getCacheImage() const { return mCache; }

		bool isCacheEmpty() const { return ! mUseCache || ! mCache.getSize(); }

		/// Checks if the texture has the source (required by user) size, otherwise real texture size are bigger.
		bool isTextureSrcSize() const;


	protected:

		Canvas( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		virtual ~Canvas();

		void correctUV();

		void baseChangeWidgetSkin( WidgetSkinInfoPtr _info );

		void initialiseWidgetSkin( WidgetSkinInfoPtr _info );
		
		void shutdownWidgetSkin();

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( const IntPoint & _pixel ) { return pointPixel( _pixel.left, _pixel.top ); }

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( size_t _x, size_t _y );

		// owerriden
		void loadResource( Ogre::Resource* _resource );

		void updateCache();

		size_t nextPowerOf2( size_t num );

	protected:
		/// Current texture
		Ogre::TexturePtr mTexPtr;

		IntSize mReqTexSize;

		std::string mTexName;

		/// Set \sa setCacheState
		bool mUseCache;

		Ogre::Image mCache;

		ResizeMode mTexResizeMode;

		/// Saved pointer from last calling lock. \sa lock
		uint8* mTexData;

	}; // class Texture

} // namespace MyGUI

#endif // __MYGUI_CANVAS_H__
