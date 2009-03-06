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
		//mMaxSize( INT_SIZE_UNBOUND ),
		//mIsInitialized( false ),
		mWidget( _widget )
	{
	}

	SizeDescription::SizeDescription( WidgetPtr _widget, const Dimension& _dim )
	:	mSizeBehaviour( 0 ), 
		size( _dim ), 
		//mMinSize( 0, 0 ), 
		//mMaxSize( INT_SIZE_UNBOUND ),
		//mIsInitialized( false ),
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

	/*void SizeDescription::setSize( const Dimension& _dim )
	{
		size = _dim;
	}*/

	void SizeDescription::setSizeBehaviour( uint8 _beh )
	{
		MYGUI_ASSERT( checkBehaviour( _beh ), "Invalid size benaviour!" );

		mSizeBehaviour = _beh;
		//mIsInitialized = true;
	}


	/*IntSize SizeDescription::getWidgetMinSize() const
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
	}*/

	//IntSize SizeDescription::getMinSize() const
	//{
	//	return getWidgetMinSize();
	//	//return mMinSize;
	//}

	//void SizeDescription::setMinSize( const IntSize& _pxSize )
	//{
	//	mMinSize = _pxSize;
	//	mIsInitialized = true;
	//}

	//void SizeDescription::setMaxSize( const IntSize& _pxSize )
	//{
	//	mMaxSize = _pxSize;
	//	mIsInitialized = true;
	//}

	//bool SizeDescription::isPxSize() const
	//{
	//	return mPxSize != IntSize( 0, 0 ) && mFlSize == FloatSize( 0, 0 ); 
	//}

	//bool SizeDescription::isFlSize() const 
	//{ 
	//	return mFlSize != FloatSize( 0, 0 ) && mPxSize == IntSize( 0, 0 ); 
	//}
	
} // namespace MyGUI

