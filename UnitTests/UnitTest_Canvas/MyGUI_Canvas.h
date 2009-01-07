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
			RM_NO_RESIZING,
			RM_EXACT_RESIZING,
		};

	public:
		void createTexture( const IntSize & _size, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT );

		void createTexture( size_t _width, size_t _height, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT );

		void destroyTexture();

		// For debug
		//void loadFromFile( const std::string & fileName );

		/// Call user delegate update and removes old texture if it isn't original.
		void updateTexture();

		void restoreFromCache();

		void restoreFromCache( const Ogre::Image::Box & _copyTo );

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
		void setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value );

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( const IntPoint & _pixel );

		/// Gets the _pixel. @remarks Texture buffer must be locked before!
		Ogre::ColourValue getPixel( size_t _x, size_t _y );

		/// Returns real width of texture.
		size_t getWidth() const { return mTexPtr->getWidth(); }

		/// Returns real height of texture.
		size_t getHeight() const { return mTexPtr->getHeight(); }

		/// Returns needed width while creating texture.
		size_t getSrcWidth() const { return mTexPtr->getSrcWidth(); }

		/// Returns needed height while creating texture.
		size_t getSrcHeight() const { return mTexPtr->getSrcHeight(); }

		/// Returns needed sizes while creating texture.
		Ogre::PixelFormat getFormat() const { return mTexPtr->getFormat(); }

		/// Returns current texture. @remarks this method can be dangerous! It's leaved
		Ogre::TexturePtr getTexture() { return mTexPtr; }

		/// Returns name of the current texture.
		const std::string & getName() const { return mTexPtr->getName(); }

		/// Returns group of the current texture.
		const std::string & getGroup() const { return mTexPtr->getGroup(); }

		/// Returns real _size of texture.
		IntSize getSize() const { return IntSize( getWidth(), getHeight() ); }

		/// Returns needed sizes while creating texture.
		IntSize getSrcSize() const { return IntSize( getSrcWidth(), getSrcHeight() ); }

		/// Overriden. Resize texture if needed dimensions are bigger than current. Delegate update will be called. \sa update.
		void setSize( const IntSize & _size );

		void setSize( size_t _width, size_t _height );

		ResizeMode getResizeMode() const { return mTexResizeMode; }

		void setResizeMode( ResizeMode _set ){ mTexResizeMode = _set; }

		void setCacheUse( bool _cache );

		bool getCacheUse() const { return mUseCache; }

		const Ogre::Image& getCacheImage() const { return mCache; }

		bool isCacheEmpty() const { return ! mUseCache || ! mCache.getSize(); }


	protected:

		Canvas( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		virtual ~Canvas();

		void baseChangeWidgetSkin( WidgetSkinInfoPtr _info );

		void initialiseWidgetSkin( WidgetSkinInfoPtr _info );
		
		void shutdownWidgetSkin();

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( const IntPoint & _pixel );

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( size_t _x, size_t _y );

		// owerriden
		void loadResource( Ogre::Resource* _resource );

		void updateCache();

	protected:
		/// Current texture
		Ogre::TexturePtr mTexPtr;

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
