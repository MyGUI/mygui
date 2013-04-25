/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Bitwise.h"

namespace MyGUI
{

	Canvas::Canvas() :
		mTexture( nullptr ),
		mTexResizeMode( TRM_PT_CONST_SIZE ),
		mTexData( 0 ),
		mTexManaged( true ),
		mFrameAdvise( false ),
		mInvalidateData(false)
	{
		mGenTexName = utility::toString((size_t)this, "_Canvas");
	}

	void Canvas::createTexture( TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		int width = std::max(1, getWidth());
		int height = std::max(1, getHeight());

		createTexture( width, height, _resizeMode, _usage, _format );
	}

	void Canvas::createTexture( const IntSize& _size, TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		int width = std::max(1, _size.width);
		int height = std::max(1, _size.height);

		createTexture( width, height, _resizeMode, _usage, _format );
	}

	void Canvas::createExactTexture( int _width, int _height, TextureUsage _usage, PixelFormat _format )
	{
		int width = std::max(1, _width);
		int height = std::max(1, _height);

		destroyTexture();

		mTexture = RenderManager::getInstance().createTexture(mGenTexName);
		mTexture->setInvalidateListener(this);
		mTexture->createManual( width, height, _usage, _format );

		mTexManaged = true;

		_setTextureName( mGenTexName );
		correctUV();
		requestUpdateCanvas( this, Event( true, true, mInvalidateData ) );
	}

	void Canvas::resize( const IntSize& _size )
	{
		if ( _size.width <= 0 || _size.height <= 0 || ! mTexManaged )
			return;

		mReqTexSize = _size;

		frameAdvise( true );
	}

	void Canvas::createTexture( int _width, int _height, TextureResizeMode _resizeMode, TextureUsage _usage, PixelFormat _format )
	{
		mTexResizeMode = _resizeMode;

		int width = std::max(1, _width);
		int height = std::max(1, _height);

		if (_resizeMode == TRM_PT_CONST_SIZE)
		{
			mReqTexSize = IntSize(width, height);
		}
		else
		{
			mReqTexSize = IntSize(std::max(1, getWidth()), std::max(1, getHeight()));
		}

		bool create = checkCreate( width, height );

		width = Bitwise::firstPO2From(width);
		height = Bitwise::firstPO2From(height);

		if ( create )
			createExactTexture( width, height, _usage, _format );
	}

	void Canvas::setSize( const IntSize& _size )
	{
		resize( _size );

		Base::setSize( _size );
	}

	void Canvas::setCoord( const IntCoord& _coord )
	{
		resize( _coord.size() );

		Base::setCoord( _coord );
	}

	void Canvas::updateTexture()
	{
		mInvalidateData = true;
		frameAdvise( true );
	}

	bool Canvas::checkCreate( int _width, int _height ) const
	{
		if ( mTexture == nullptr )
			return true;

		if ( mTexture->getWidth() >= _width && mTexture->getHeight() >= _height )
			return false;

		return true;
	}

	void Canvas::validate( int& _width, int& _height, TextureUsage& _usage, PixelFormat& _format ) const
	{
		_width = std::max(1, _width);
		_height = std::max(1, _height);

		_width = Bitwise::firstPO2From(_width);
		_height = Bitwise::firstPO2From(_height);

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

	void Canvas::shutdownOverride()
	{
		_destroyTexture(false);
		frameAdvise(false);
	}

	void Canvas::initialiseOverride()
	{
	}

	void Canvas::_destroyTexture( bool _sendEvent )
	{
		if ( mTexture != nullptr )
		{
			if ( _sendEvent )
			{
				eventPreTextureChanges( this );
			}

			RenderManager::getInstance().destroyTexture( mTexture );
			mTexture = nullptr;
		}

	}

	void Canvas::correctUV()
	{
		if ( mTexResizeMode == TRM_PT_VIEW_REQUESTED )
		{
			_setUVSet(
				FloatRect(
					0,
					0,
					(float) mReqTexSize.width  / (float) getTextureRealWidth(),
					(float) mReqTexSize.height / (float) getTextureRealHeight()));
		}

		if ( mTexResizeMode == TRM_PT_CONST_SIZE || mTexResizeMode == TRM_PT_VIEW_ALL )
		{
			_setUVSet( FloatRect( 0, 0, 1, 1 ) );
		}
	}

	void* Canvas::lock(TextureUsage _usage)
	{
		void* data = mTexture->lock(_usage);

		mTexData = reinterpret_cast< uint8* >( data );

		return data;
	}

	void Canvas::unlock()
	{
		mTexture->unlock();
	}

	bool Canvas::isTextureSrcSize() const
	{
		return getTextureSrcSize() == getTextureRealSize();
	}

	void Canvas::frameAdvise( bool _advise )
	{
		if ( _advise )
		{
			if ( ! mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate( this, &Canvas::frameEntered );
				mFrameAdvise = true;
			}
		}
		else
		{
			if ( mFrameAdvise )
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

		if ( mTexResizeMode == TRM_PT_CONST_SIZE )
			create = false;

		if ( create )
		{
			createExactTexture( width, height, usage, format );
			correctUV();
		}
		else // I thought order is important
		{
			correctUV();
			requestUpdateCanvas( this, Event( false, true, mInvalidateData ) );
		}

		mInvalidateData = false;
		frameAdvise( false );
	}

	void Canvas::textureInvalidate(ITexture* _texture)
	{
		updateTexture();
	}

	void Canvas::_setUVSet(const FloatRect& _rect)
	{
		if (nullptr != getSubWidgetMain())
			getSubWidgetMain()->_setUVSet(_rect);
	}

	bool Canvas::isLocked() const
	{
		return mTexture->isLocked();
	}

	int Canvas::getTextureRealWidth() const
	{
		return (int) mTexture->getWidth();
	}

	int Canvas::getTextureRealHeight() const
	{
		return (int) mTexture->getHeight();
	}

	IntSize Canvas::getTextureRealSize() const
	{
		return IntSize( getTextureRealWidth(), getTextureRealHeight() );
	}

	int Canvas::getTextureSrcWidth() const
	{
		return mReqTexSize.width;
	}

	int Canvas::getTextureSrcHeight() const
	{
		return mReqTexSize.height;
	}

	IntSize Canvas::getTextureSrcSize() const
	{
		return mReqTexSize;
	}

	PixelFormat Canvas::getTextureFormat() const
	{
		return mTexture->getFormat();
	}

	const std::string& Canvas::getTextureName() const
	{
		return mTexture->getName();
	}

	void Canvas::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void Canvas::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	Canvas::TextureResizeMode Canvas::getResizeMode() const
	{
		return mTexResizeMode;
	}

	void Canvas::setResizeMode(TextureResizeMode _value)
	{
		mTexResizeMode = _value;
	}

	bool Canvas::isTextureCreated() const
	{
		return mTexture != nullptr;
	}

	bool Canvas::isTextureManaged() const
	{
		return mTexManaged;
	}

	ITexture* Canvas::getTexture() const
	{
		return mTexture;
	}

	void Canvas::setTextureManaged(bool _value)
	{
		mTexManaged = _value;
	}

	TextureUsage Canvas::getDefaultTextureUsage()
	{
		return TextureUsage::Stream | TextureUsage::Write;
	}

	PixelFormat Canvas::getDefaultTextureFormat()
	{
		return PixelFormat::R8G8B8A8;
	}

} // namespace MyGUI
