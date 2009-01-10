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
		enum TextureResizeMode
		{
			// PT - Power of Two, texture 
			TRM_PT_CONST_SIZE,
			TRM_PT_VIEW_REQUESTED,
			//TRM_POWER_OF_TWO,
		};

	public:

		void createTexture( TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 );

		void createTexture( size_t _width, size_t _height, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 );

		void createTexture( const IntSize & _size, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = Ogre::TU_DEFAULT, Ogre::PixelFormat _format = Ogre::PF_A8R8G8B8 );

		void destroyTexture();

		/// Call user delegate update and removes old texture if it isn't original.
		void updateTexture();

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

		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		TextureResizeMode getResizeMode() const { return mTexResizeMode; }

		void setResizeMode( TextureResizeMode _set ) { mTexResizeMode = _set; }


		/// Checks if the texture has the source (required by user) size, otherwise real texture size are bigger.
		bool isTextureSrcSize() const;


	protected:

		Canvas( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		virtual ~Canvas();

		void validateSize( size_t & _width, size_t & _height ) const;
		void validate( size_t & _width, size_t & _height, Ogre::TextureUsage & _usage, Ogre::PixelFormat & _format ) const;

		void createExactTexture( size_t _width, size_t _height, Ogre::TextureUsage _usage, Ogre::PixelFormat _format );

		bool checkCreate( size_t _width, size_t _height ) const;

		void resize( const IntSize & _size );

		//void _createTexture( size_t _width, size_t _height, Ogre::TextureUsage _usage, Ogre::PixelFormat _format );

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

		size_t nextPowerOf2( size_t num ) const;

	protected:
		/// Current texture
		Ogre::TexturePtr mTexPtr;

		IntSize mReqTexSize;

		std::string mTexName;

		TextureResizeMode mTexResizeMode;

		/// Saved pointer from last calling lock. \sa lock
		uint8* mTexData;

	}; // class Texture

} // namespace MyGUI

#endif // __MYGUI_CANVAS_H__
