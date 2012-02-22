/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
*/
/*
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
#include "MyGUI_ITexture.h"

namespace MyGUI
{

	/**
		Widget wrapper over Texture - shows the texture.
		Implemented: resizing of texture (see TextureResizeMode); recovery after loosing device;
	*/
	class MYGUI_EXPORT Canvas :
		public Widget,
		public ITextureInvalidateListener
	{
		MYGUI_RTTI_DERIVED( Canvas )

	public:
		Canvas();

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

		typedef delegates::CMultiDelegate1<Canvas*> EventHandle_CanvasPtr;
		typedef delegates::CDelegate2<Canvas*, Event> EventHandle_CanvasPtrEvent;

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
			TRM_PT_VIEW_ALL /// Texture resizes and fills all widget space
		};

	public:
		/// Creates texture
		void createTexture(TextureResizeMode _resizeMode, TextureUsage _usage = getDefaultTextureUsage(), PixelFormat _format = getDefaultTextureFormat());

		/// Creates texture
		void createTexture(int _width, int _height, TextureResizeMode _resizeMode, TextureUsage _usage = getDefaultTextureUsage(), PixelFormat _format = getDefaultTextureFormat());

		/// Creates texture
		void createTexture(const IntSize& _size, TextureResizeMode _resizeMode, TextureUsage _usage = getDefaultTextureUsage(), PixelFormat _format = getDefaultTextureFormat());

		/// Destroys texture
		void destroyTexture();

		/// Call user delegate update and removes old texture if it isn't original.
		void updateTexture();

		/// Locks hardware pixel buffer.
		void* lock(TextureUsage _usage = TextureUsage::Write);

		/// Unlocks hardware pixel buffer.
		void unlock();

		/// Checks lockness of hardware _pixel buffer.
		bool isLocked() const;

		/// Returns real width of texture.
		int getTextureRealWidth() const;

		/// Returns real height of texture.
		int getTextureRealHeight() const;

		/// Returns real _size of texture.
		IntSize getTextureRealSize() const;

		/// Returns needed width while creating texture.
		int getTextureSrcWidth() const;

		/// Returns needed height while creating texture.
		int getTextureSrcHeight() const;

		/// Returns needed sizes while creating texture.
		IntSize getTextureSrcSize() const;

		/// Returns needed sizes while creating texture.
		PixelFormat getTextureFormat() const;

		/// Returns name of the current texture.
		const std::string& getTextureName() const;

		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

		/// Returns resize mode
		TextureResizeMode getResizeMode() const;

		/// Sets resize mode of texture \sa TextureResizeMode
		void setResizeMode(TextureResizeMode _value);

		/// Checks if the texture has the source (required by user) size, otherwise real texture size are bigger.
		bool isTextureSrcSize() const;

		/// Returns true if the texture was created (and exists), otherwise false
		bool isTextureCreated() const;

		/// Returns true if we own the texture, otherwise false. \sa mManaged
		bool isTextureManaged() const;

		/// Reurns interface texture.
		ITexture* getTexture() const;

		/// Sets the texture managed @remarks Be careful with assigning managed status to texture, which wasn't created in Canvas! \sa mManaged
		void setTextureManaged(bool _value);

		/// Returns default GUI texture usage
		static TextureUsage getDefaultTextureUsage();

		/// Returns default GUI texture format
		static PixelFormat getDefaultTextureFormat();

		/*events:*/
		/** Event : Notify user texture instance will be changed \sa requestUpdateCanvas.\n
			signature : void method(MyGUI::Canvas* _canvas)\n
			@param _canvas, which will be updated
		*/
		EventHandle_CanvasPtr eventPreTextureChanges;

		/** Event : Texture instance was changed (May be caused by resizing texture or lossing device). User have to update all references to new instance of texture.\n
			signature : void method(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)\n
			@param _canvas, which needs to update
			@param _event
		*/
		EventHandle_CanvasPtrEvent requestUpdateCanvas;

	protected:
		virtual void shutdownOverride();
		virtual void initialiseOverride();

		/// Destroys texture
		void _destroyTexture(bool _sendEvent);

		/// Update entered parameters according to current texture resize mode(size) and restore (if can) parameters of usage and format from texture
		void validate(int& _width, int& _height, TextureUsage& _usage, PixelFormat& _format) const;

		/// Creates the texture itself
		void createExactTexture(int _width, int _height, TextureUsage _usage, PixelFormat _format);

		/// Checks if we need to create a texture with such sizes.
		bool checkCreate(int _width, int _height) const;

		/// Calls when resize widget
		void resize(const IntSize& _size);

		/// Correct texture uv-coordinates
		void correctUV();

		/// For updating once per frame.
		void frameAdvise(bool _advise);

		/// For updating once per frame.
		void frameEntered(float _time);

		virtual void textureInvalidate(ITexture* _texture);

		void _setUVSet(const FloatRect& _rect);

	protected:
		/// Current texture
		ITexture* mTexture;

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

		bool mInvalidateData;
	};

} // namespace MyGUI

#endif // __MYGUI_CANVAS_H__
