/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#ifndef __MYGUI_SIZE_DESCRIPTION_H__
#define __MYGUI_SIZE_DESCRIPTION_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	enum SizeBehaviour
	{
		SB_MIN = MYGUI_FLAG( 1 ),
		SB_MAX = MYGUI_FLAG( 2 ),
		SB_CONSTRICT = MYGUI_FLAG( 3 ),
		SB_WIDE = MYGUI_FLAG( 4 ),
	};

	//extern IntSize INT_SIZE_UNBOUND;
	static const IntSize INT_SIZE_UNBOUND( 10000, 10000 );

	class SizeDescription
	{
	public:


	protected:

		WidgetPtr mWidget;

		IntSize mPxSize;
		FloatSize mFlSize;

		IntSize mMinSize;
		IntSize mMaxSize;

		uint8 mSizeBehaviour;

		bool mChanged;

		bool checkBehaviour( uint8 _beh ) const;



	public:
		SizeDescription( WidgetPtr _widget );

		SizeDescription( WidgetPtr _widget, const IntSize& _pxSsize );

		SizeDescription( WidgetPtr _widget, const FloatSize& _flSize );

		void setSizeBehaviour( uint8 _beh );

		uint8 getSizeBehaviour() const { return mSizeBehaviour; }

		IntSize getSize( const IntSize& _canGive = IntSize( -1, -1 ) ) const;
		IntSize getMinSize() const;

		IntSize getWidgetMinSize() const;

		void setSize( const IntSize& _pxSize );

		void setSize( const FloatSize& _flSize );

		void setMinSize( const IntSize& _pxSize );
		void setMaxSize( const IntSize& pxSize );

		bool isChanged() const { return mChanged; }

		
		IntSize getPxSize() const { return mPxSize; }
		FloatSize getFlSize() const { return mFlSize; }

		bool isPxSize() const;
		bool isFlSize() const;
	};

} // namespace MyGUI

#endif // __MYGUI_SIZE_DESCRIPTION_H__
