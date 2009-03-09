/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_CANVAS_H__
#define __MYGUI_CANVAS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	/**
	 * Widget wrapper over Ogre::Texture - shows the texture.
	 * Implemented: resizing of texture (see TextureResizeMode); recovery after lossing device; set/get pixel methods for fun :) .
	 */
	class MYGUI_EXPORT Canvas : public Widget, public Ogre::ManualResourceLoader
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Canvas>;

		MYGUI_RTTI_CHILD_HEADER( Canvas, Widget );

	public:

		struct Event
		{
			Event( bool _textureChanged, bool _widgetResized, bool _requested ) :
				textureChanged( _textureChanged ),
				widgetResized( _widgetResized ),
				requested( _requested )
			{
			}

			bool textureChanged;
			bool widgetResized;

			/// This update was caused by calling updateTexture();
			bool requested;
		};

		typedef delegates::CDelegate1<CanvasPtr> EventInfo_Canvas;
		typedef delegates::CDelegate2<CanvasPtr, Event> EventInfo_CanvasEvent;

		//FIXME
		/** 
			Available resize and view modes of texture
			@remarks PT - Power of Two (size)
		*/
		enum TextureResizeMode
		{
			// 
			TRM_PT_CONST_SIZE, /// Texture doesn't resizes and fills all widget space
			TRM_PT_VIEW_REQUESTED, /// You can view all pixels of texture, texture cropped by sizes of widget
			TRM_PT_VIEW_ALL, /// Texture resizes and fills all widget space
		};

	public:

		/// Creates texture
		void createTexture( TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = getDefaultTextureUsage(), Ogre::PixelFormat _format = getDefaultTextureFormat() );

		/// Creates texture
		void createTexture( int _width, int _height, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = getDefaultTextureUsage(), Ogre::PixelFormat _format = getDefaultTextureFormat() );

		/// Creates texture
		void createTexture( const IntSize & _size, TextureResizeMode _resizeMode, Ogre::TextureUsage _usage = getDefaultTextureUsage(), Ogre::PixelFormat _format = getDefaultTextureFormat() );

		/// Destroys texture
		void destroyTexture();

		/**
		 Loads texture
		 @param manageMode switch canvas to manage mode \sa mManaged, setTextureManaged.
		 @remarks Be careful with assigning managed status to texture (lost memory, invalid memory and so on :) ).
		 */
		void loadTexture( Ogre::TexturePtr texture, bool manageMode = false );

		/// Call user delegate update and removes old texture if it isn't original.
		void updateTexture();

		/// Locks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->lock()
		void* lock();

		/// Unlocks hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->unlock()
		void unlock();

		/// Checks lockness of hardware _pixel buffer. \sa Ogre::HardwarePixelBufferSharedPtr->isLocked()
		bool isLocked() const { return mTexPtr->getBuffer()->isLocked(); }

		/// Returns hardware pixel buffer
		Ogre::HardwarePixelBufferSharedPtr getBuffer() { return mTexPtr->getBuffer(); }

		/// Sets the pixel. @remarks Texture buffer must be locked before call!
		void setPixel( int _x, int _y, const Ogre::ColourValue & value );

		/// Sets the pixel. @remarks Texture buffer must be locked before call!
		void setPixel( const IntPoint & _pixel, const Ogre::ColourValue & value ) { setPixel( _pixel.left, _pixel.top, value ); }

		/// Retrieves a colour value of pixel. @remarks Texture buffer must be locked before call!
		Ogre::ColourValue getPixel( const IntPoint & _pixel ) { return getPixel( _pixel.left, _pixel.top ); }

		/// Retrieves a colour value of pixel. @remarks Texture buffer must be locked before call!
		Ogre::ColourValue getPixel( int _x, int _y );

		/// Returns real width of texture.
		int getTextureRealWidth() const { return int(mTexPtr->getWidth()); }

		/// Returns real height of texture.
		int getTextureRealHeight() const { return int(mTexPtr->getHeight()); }

		/// Returns real _size of texture.
		IntSize getTextureRealSize() const { return IntSize( getTextureRealWidth(), getTextureRealHeight() ); }

		/// Returns needed width while creating texture.
		int getTextureSrcWidth() const { return mReqTexSize.width; }

		/// Returns needed height while creating texture.
		int getTextureSrcHeight() const { return mReqTexSize.height; }

		/// Returns needed sizes while creating texture.
		IntSize getTextureSrcSize() const { return mReqTexSize; }

		/// Returns needed sizes while creating texture.
		Ogre::PixelFormat getTextureFormat() const { return mTexPtr->getFormat(); }

		/// Returns name of the current texture.
		const std::string & getTextureName() const { return mTexPtr->getName(); }

		/// Returns group of the current texture.
		const std::string & getTextureGroup() const { return mTexPtr->getGroup(); }

		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/// Returns resize mode
		TextureResizeMode getResizeMode() const { return mTexResizeMode; }

		/// Sets resize mode of texture \sa TextureResizeMode
		void setResizeMode( TextureResizeMode _set ) { mTexResizeMode = _set; }

		/// Checks if the texture has the source (required by user) size, otherwise real texture size are bigger.
		bool isTextureSrcSize() const;

		/// Returns true if the texture was created (and exists), otherwise false
		bool isTextureCreated() const { return ! mTexPtr.isNull(); }

		/// Returns true if we own the texture, otherwise false. \sa mManaged
		bool isTextureManaged() const { return mTexManaged; }

		/// Sets the texture managed @remarks Be careful with assigning managed status to texture, which wasn't created in Canvas! \sa mManaged
		void setTextureManaged( bool managed ) { mTexManaged = managed; }

		/// Returns default texture usage
		static Ogre::TextureUsage getDefaultTextureUsage() { return Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE; }

		/// Returns default texture format
		static Ogre::PixelFormat getDefaultTextureFormat() { return Ogre::PF_BYTE_BGRA; }

	/*event:*/
		/** Event : Notify user texture instance will be changed \sa requestUpdateCanvas\n
			signature : void method(MyGUI::CanvasPtr _canvas)\n
			@param _canvas, which will be updated
		 */
		EventInfo_Canvas eventPreTextureChanges;

		/** Event : Texture instance was changed (May be caused by resizing texture or lossing device). User have to update all references to new instance of texture.\n
			signature : void method(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event )\n
			@param _canvas, which needs to update
			@param _event
		 */
		EventInfo_CanvasEvent requestUpdateCanvas;

	protected:

		/// Protected constructor. Use factory!
		Canvas( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		virtual ~Canvas();

		/// Destroys texture
		void _destroyTexture( bool _sendEvent );

		/// Update entered parameters according to current texture resize mode
		void validateSize( int & _width, int & _height ) const;

		/// Update entered parameters according to current texture resize mode(size) and restore (if can) parameters of usage and format from texture
		void validate( int & _width, int & _height, Ogre::TextureUsage & _usage, Ogre::PixelFormat & _format ) const;

		/// Create Ogre texture
		void createExactTexture( int _width, int _height, Ogre::TextureUsage _usage, Ogre::PixelFormat _format );

		/// Checks if we need to create a texture with such sizes.
		bool checkCreate( int _width, int _height ) const;

		/// Calls when resize widget
		void resize( const IntSize & _size );

		/// Correct texture uv-coordinates
		void correctUV();

		//! @copydoc Widget::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
		void baseChangeWidgetSkin( WidgetSkinInfoPtr _info );

		//! @copydoc Widget::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
		void initialiseWidgetSkin( WidgetSkinInfoPtr _info );

		//! @copydoc Widget::shutdownWidgetSkin()
		void shutdownWidgetSkin();

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( const IntPoint & _pixel ) { return pointPixel( _pixel.left, _pixel.top ); }

		/// Detect position of _pixel in pixel buffer
		void* pointPixel( int _x, int _y );

		void loadResource( Ogre::Resource* _resource );

		/// Returns number power of two not less than entered.
		static size_t nextPowerOf2( size_t num );

		/// For updating once per frame.
		void frameAdvise( bool _advise );

		/// For updating once per frame.
		void frameEntered( float _time );

	protected:
		/// Current texture
		Ogre::TexturePtr mTexPtr;

		/// Requested bu user sizes
		IntSize mReqTexSize;

		/// Generated texture name
		std::string mGenTexName;

		/// Texture resize mode \sa TextureResizeMode
		TextureResizeMode mTexResizeMode;

		/// Saved pointer from last calling lock. \sa lock
		uint8* mTexData;

		/// true if we own the texture (can delete it or replace by another instance), otherwise false
		bool mTexManaged;

		/// For updating once per frame. True state means updating before next frame starts.
		bool mFrameAdvise;

	};

} // namespace MyGUI

#endif // __MYGUI_CANVAS_H__
