/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_SizeDescription.h"
#include "MyGUI_Window.h"

namespace MyGUI
{
	SizeDescription::SizeDescription( WidgetPtr _widget )
	:	mSizeBehaviour( 0 ), 
		mMinSize( 0, 0 ), 
		mMaxSize( INT_SIZE_UNBOUND ),
		mChanged( false ),
		mWidget( _widget )
	{
	}

	SizeDescription::SizeDescription( WidgetPtr _widget, const IntSize& _pxSsize )
	:	mSizeBehaviour( 0 ), 
		mPxSize( _pxSsize ), 
		mMinSize( 0, 0 ), 
		mMaxSize( INT_SIZE_UNBOUND ),
		mChanged( false ),
		mWidget( _widget )
	{
	}

	SizeDescription::SizeDescription( WidgetPtr _widget, const FloatSize& _flSize )
	:	mSizeBehaviour( 0 ), 
		mFlSize( _flSize ), 
		mMinSize( 0, 0 ), 
		mMaxSize( INT_SIZE_UNBOUND ),
		mChanged( false ),
		mWidget( _widget )
	{
	}

	bool SizeDescription::checkBehaviour( uint8 _beh ) const
	{
		if( ( _beh & SB_MIN ) && ( _beh & SB_MAX ) )
		{
			MYGUI_EXCEPT( "Can't be min and max!" ); 
			return false;
		}

		return true;
	}

	void SizeDescription::setSize( const IntSize& _pxSize )
	{
		mPxSize = _pxSize;
		mFlSize = FloatSize();
		mChanged = true;
	}

	void SizeDescription::setSize( const FloatSize& _flSize )
	{
		mFlSize = _flSize;
		mPxSize = IntSize();
		mChanged = true;
	}

	void SizeDescription::setSizeBehaviour( uint8 _beh )
	{
		MYGUI_ASSERT( checkBehaviour( _beh ), "Invalid size benaviour!" );

		mSizeBehaviour = _beh;
		mChanged = true;
	}

	IntSize SizeDescription::getSize( const IntSize& _canGive ) const
	{
		if( _canGive == IntSize( -1, -1 ) )
		{
			return mPxSize;
		}
		else
		{
			if( mSizeBehaviour & SB_MIN )
			{
				if( mMinSize != IntSize( -1, -1 ) )
					return mMinSize;
				else
					return _canGive;
			}

			//return m
		}
	}

	IntSize SizeDescription::getWidgetMinSize() const
	{
		IntSize result;

		const std::string& type =  mWidget->getTypeName();

		if( type == "Window" )
		{
			WindowPtr window = mWidget->castType< Window >( false );
			return window->getMinSize();
		}
		else
			return IntSize( 0, 0 );
	}

	IntSize SizeDescription::getMinSize() const
	{
		return getWidgetMinSize();
		//return mMinSize;
	}

	void SizeDescription::setMinSize( const IntSize& _pxSize )
	{
		mMinSize = _pxSize;
		mChanged = true;
	}

	void SizeDescription::setMaxSize( const IntSize& _pxSize )
	{
		mMaxSize = _pxSize;
		mChanged = true;
	}

	bool SizeDescription::isPxSize() const
	{
		return mPxSize != IntSize( 0, 0 ) && mFlSize == FloatSize( 0, 0 ); 
	}

	bool SizeDescription::isFlSize() const 
	{ 
		return mFlSize != FloatSize( 0, 0 ) && mPxSize == IntSize( 0, 0 ); 
	}
	
} // namespace MyGUI

