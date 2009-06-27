/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextureManager.h"

namespace MyGUI
{

	Canvas::Canvas() :
		mTexture( nullptr ),
		mTexResizeMode( TRM_PT_CONST_SIZE ),
		mTexData( 0 ),
		mTexManaged( true ),
		mFrameAdvise( false )
	{
		mGenTexName = utility::toString( this, "_Canvas" );
	}

	void Canvas::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, const SkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);
	}

	Canvas::~Canvas()
	{
		_destroyTexture( false );
	}

	void Canvas::createTexture( TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		createTexture( getSize(), _resizeMode, _usage, _format );
	}

	void Canvas::createTexture( const IntSize & _size, TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		if( _size.width <= 0 || _size.height <= 0 )
		{
			MYGUI_ASSERT( 0, "At least one of dimensions isn't positive!" );
			return;
		}

		createTexture( _size.width, _size.height, _resizeMode, _usage, _format );
	}

	void Canvas::createExactTexture( int _width, int _height, TextureUsage _usage, PixelFormat _format )
	{
		MYGUI_ASSERT( _width >= 0 && _height >= 0, "negative size" );

		destroyTexture();

		mTexture = TextureManager::getInstance().createTexture(mGenTexName);
		mTexture->setManualResourceLoader( this );
		mTexture->createManual( _width, _height, _usage, _format );

		mTexManaged = true;
	}

	void Canvas::resize( const IntSize & _size )
	{
		if( _size.width <= 0 || _size.height <= 0 || ! mTexManaged )
			return;

		mReqTexSize = _size;

		frameAdvise( true );
	}

	void Canvas::createTexture( int _width, int _height, TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		MYGUI_ASSERT( _width >= 0 && _height >= 0, "negative size" );

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

		Base::setSize( _size );
	}

	void Canvas::setCoord( const IntCoord & _coord )
	{
		resize( _coord.size() );

		Base::setCoord( _coord );
	}

	void Canvas::updateTexture()
	{
		requestUpdateCanvas( this, Event( false, false, true ) );
	}

	bool Canvas::checkCreate( int _width, int _height ) const
	{
		if ( mTexture == nullptr )
			return true;

		if ( mTexture->getWidth() >= _width && mTexture->getHeight() >= _height )
			return false;

		return true;
	}

	void Canvas::validateSize( int & _width, int & _height ) const
	{
		if( mTexResizeMode == TRM_PT_CONST_SIZE
		 || mTexResizeMode == TRM_PT_VIEW_REQUESTED
		 || mTexResizeMode == TRM_PT_VIEW_ALL )
		{
			_width = (int)nextPowerOf2( _width );
			_height = (int)nextPowerOf2( _height );
		}
	}

	void Canvas::validate( int & _width, int & _height, TextureUsage& _usage, PixelFormat& _format ) const
	{
		validateSize( _width, _height );

		// restore usage and format
		if ( mTexture != nullptr )
		{
			if ( _usage == getDefaultTextureUsage() )
				_usage = mTexture->getUsage();

			if ( _format == getDefaultTextureFormat() )
				_format = mTexture->getFormat();
		}
	}

	void Canvas::destroyTexture()
	{
		_destroyTexture( true );
	}

	void Canvas::_destroyTexture( bool _sendEvent )
	{
		if ( mTexture != nullptr )
		{
			if ( _sendEvent )
			{
				eventPreTextureChanges( this );
			}

			TextureManager::getInstance().destroyTexture( mTexture );
			mTexture = nullptr;
		}

	}

	void Canvas::correctUV()
	{
		if( mTexResizeMode == TRM_PT_VIEW_REQUESTED )
		{
			_setUVSet( FloatRect( 0, 0,
				(float) mReqTexSize.width  / (float) getTextureRealWidth(),
				(float) mReqTexSize.height / (float) getTextureRealHeight()
				) );
		}

		if( mTexResizeMode == TRM_PT_CONST_SIZE || mTexResizeMode == TRM_PT_VIEW_ALL )
		{
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );
		}
	}

	void* Canvas::lock()
	{
		void* data = mTexture->lock();

		mTexData = reinterpret_cast< uint8* >( data );

		return data;
	}

	void Canvas::unlock()
	{
		mTexture->unlock();
	}

	void Canvas::baseChangeWidgetSkin( SkinInfoPtr _info )
	{
		Base::baseChangeWidgetSkin( _info );
	}

	void Canvas::initialiseWidgetSkin( SkinInfoPtr _info )
	{
	}

	void Canvas::shutdownWidgetSkin()
	{
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

	void Canvas::frameAdvise( bool _advise )
	{
		if( _advise )
		{
			if( ! mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate( this, &Canvas::frameEntered );
				mFrameAdvise = true;
			}
		}
		else
		{
			if( mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate( this, &Canvas::frameEntered );
				mFrameAdvise = false;
			}
		}
	}

	void Canvas::frameEntered( float _time )
	{
		int width = mReqTexSize.width;
		int height = mReqTexSize.height;
		TextureUsage usage = getDefaultTextureUsage();
		PixelFormat format = getDefaultTextureFormat();

		validate( width, height, usage, format );

		bool create = checkCreate( width, height );

		if( mTexResizeMode == TRM_PT_CONST_SIZE )
			create = false;

		if( create )
		{
			createExactTexture( width, height, usage, format );
			correctUV();
		}
		else // I thought order is important
		{
			correctUV();
			requestUpdateCanvas( this, Event( false, true, false ) );
		}

		frameAdvise( false );
	}

	void Canvas::loadResource( IRenderResource* _resource )
	{
		ITexture* texture = static_cast< ITexture* >( _resource );

		if( mTexture == texture )
		{
			_setTextureName( mGenTexName );

			correctUV();

			requestUpdateCanvas( this, Event( true, true, false ) );
		}
	}

} // namespace MyGUI
