/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_FlowContainer.h"

namespace MyGUI
{
	FlowContainer::FlowContainer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name )
		:	Container( _style, _coord, _align, _info, _parent, _croppedParent, _creator, _name )
	{
	}

	WidgetPtr FlowContainer::baseCreateWidget( WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name )
	{
		//MYGUI_ASSERT( _coord.point() != IntPoint(), "Warning! Non-default value of position - it will be ignored!" );

		return Container::baseCreateWidget( _style, _type, _skin, _coord, _align, _layer, _name );
	}

	FlowContainer::WidgetInfo* FlowContainer::getWidgetInfo( WidgetPtr _widget )
	{
		for( ListWidgetInfo::reverse_iterator widgetIter = mWidgetsInfo.rbegin(); widgetIter != mWidgetsInfo.rend(); ++widgetIter )
		{
			if( widgetIter->widget == _widget )
				return &( *widgetIter );
		}

		return 0;
	}

	SizeDescription* FlowContainer::getSizeDescription( WidgetPtr _widget )
	{
		for( ListWidgetInfo::reverse_iterator widgetIter = mWidgetsInfo.rbegin(); widgetIter != mWidgetsInfo.rend(); ++widgetIter )
		{
			if( widgetIter->widget == _widget )
				return &widgetIter->sizeDesc;
		}

		return 0;
	}

	void FlowContainer::_destroyChildWidget( WidgetPtr _widget )
	{
		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			if( widgetIter->widget == _widget )
			{
				mWidgetsInfo.erase( widgetIter );
				break;
			}
		}

		Widget::_destroyChildWidget( _widget );

		update();
	}

	void FlowContainer::updateWidgetInfo( WidgetPtr _widget )
	{
		updateWidgetInfo( *getWidgetInfo( _widget ) );
	}

	void FlowContainer::updateWidgetInfo( WidgetInfo& _widgetInfo )
	{
		_widgetInfo.sizeDesc.minSize.px( getWidgetMinSize( _widgetInfo ) );

		if( _widgetInfo.sizeDesc.size.isNull() )
			_widgetInfo.sizeDesc.size.px( _widgetInfo.widget->getSize() );
	}
	

//#define IS(type) ( type* w = info.widget->castType< type >( false ) )

	IntSize FlowContainer::getWidgetMinSize( const WidgetInfo& _info ) const
	{
		WidgetPtr widget = _info.widget;

		//if( IS( Button ) )
		return widget->_getTextSize();
		
		//return IntSize();
	}
//#undef IS

	void FlowContainer::add( WidgetPtr _widget )
	{
		WidgetInfo info( _widget );
		updateWidgetInfo( info );
		mWidgetsInfo.push_back( info );
		update();
	}

	void FlowContainer::remove( WidgetPtr _widget )
	{
		_destroyChildWidget( _widget );
	}

	IntSize FlowContainer::getWidgetPxSize( const RowData& _data, const WidgetInfo& _info )
	{
		return IntSize( getWidgetPxWidth( _data, _info ), _info.sizeDesc.size.h.px() );
	}
	
	template< class T >
	int FlowContainer::_getWidgetPxDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info )
	{
		switch( _sizeData.state )
		{
		case PS_ALL:
			if( _size.dim().isPx() )
				return _size.dim().px();

			if( isSpacer( _info.widget ) )
			{
				if( _size.dim().isFl() )
				{
					if( isCoeff( _sizeData.spacersCoeff ) )
						return _size.dim().fl() * _sizeData.spacersCoeff;
				}
			}
			else
			{
				if( _size.dim().isFl() )
				{
					if( isCoeff( _sizeData.mainCoeff ) )
						return _size.dim().fl() * _sizeData.mainCoeff;
				}
			}
			break;

		case PS_SMALLER_SPACES:
			if( isSpacer( _info.widget ) )
			{
				if( _size.dim().isFl() )
				{
					if( isCoeff( _sizeData.spacersCoeff ) )
						return _size.dim().fl() * _sizeData.spacersCoeff;
				}
			}
			else
			{
				if( _size.dim().isPx() )
					return _size.dim().px();

				if( _size.dim().isFl() )
				{
					if( isCoeff( _sizeData.mainCoeff ) )
						return _size.dim().fl() * _sizeData.mainCoeff;
				}
			}
			break;

		//case PS_SMALLER_ALL:
		//	break;

		default:
			MYGUI_EXCEPT( "State of flow container isn't handled!" );
			break;
		}

		return _size.minDim().px();
	}

	int FlowContainer::getWidgetPxWidth( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Width > wr( _info.sizeDesc );
		return _getWidgetPxDimension( _data, wr, _info );
	}

	int FlowContainer::getWidgetPxHeight( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Height > wr( _info.sizeDesc );
		return _getWidgetPxDimension( _data, wr, _info );
	}


	template< class T >
	float FlowContainer::_getWidgetFlDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info )
	{
		switch( _sizeData.state )
		{
		case PS_ALL:
			return _size.dim().fl();
			break;

		case PS_SMALLER_SPACES:
			if( ! isSpacer( _info.widget ) )
			{
				return _size.dim().fl() * _sizeData.spacersCoeff;
			}
			break;

		//case PS_SMALLER_ALL:
		//	break;

		default:
			MYGUI_EXCEPT( "State of flow container isn't handled!" );
			break;
		}

		return _size.minDim().fl();
	}

	float FlowContainer::getWidgetFlWidth( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Width > wr( _info.sizeDesc );
		return _getWidgetFlDimension( _data, wr, _info );
	}

	float FlowContainer::getWidgetFlHeight( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Height > wr( _info.sizeDesc );
		return _getWidgetFlDimension( _data, wr, _info );
	}

	float FlowContainer::calcFlWidthSum( const RowData& _data )
	{
		ListWidgetInfoIter goIter = _data.first;

		float flHSum = 0;

		while( goIter != mWidgetsInfo.end() )
		{
			SizeDescription& desc = goIter->sizeDesc;

			flHSum += desc.size.w.fl();

			if( goIter == _data.last )
				break;

			++goIter;
		}

		return flHSum;
	}

	bool FlowContainer::isSpacer( const WidgetPtr _widget ) const
	{
		return _widget->castType< Spacer >( false ) != nullptr;
	}

	bool FlowContainer::calcPxWidthSum( const RowData& _data, CalcRow& _calcRow )
	{
		ListWidgetInfoIter goIter = _data.first;

		_calcRow.size = IntSize();
		
		bool isFirstAtRow = true;

		while( goIter != mWidgetsInfo.end() )
		{
			bool toNextRow = false;

			Widget* widget = goIter->widget;
			SizeDescription& desc = goIter->sizeDesc;

			IntSize widgetSize = getWidgetPxSize( _data, *goIter );

			if( _calcRow.size.height < widgetSize.height )
				_calcRow.size.height = widgetSize.height;
			
			// width here 

			bool ignoreNextRow = false;

			if( desc.size.w.isFl() )
			{
				if( isSpacer( widget ) )
					ignoreNextRow = true;
			}

			if( !ignoreNextRow && _data.autoLineBreak && _calcRow.size.width + widgetSize.width > _data.maxWidth )
			{
				toNextRow = true;

				if( ! isFirstAtRow )
					break;
			}

			_calcRow.size.width += widgetSize.width;

			_calcRow.last = goIter;

			if( isFirstAtRow && toNextRow )
				break;

			if( goIter == _data.last )
				break;

			++goIter;

			isFirstAtRow = false;
		}

		return true;
	}

	bool FlowContainer::getRowData( const RowInput& _in, RowData& result )
	{
		if( _in.from == mWidgetsInfo.end() )
			return false;

		result.pos = _in.cur;

		IntPoint pos = result.pos;

		result.first = _in.from;
		result.last = result.first;

		result.maxWidth = _in.maxWidth;

		ListWidgetInfoIter goIter = result.first;

		for( goIter = result.first; goIter != mWidgetsInfo.end(); ++goIter )
		{
			if( goIter->breakLine )
			{
				result.lineBreak = goIter;
				break;
			}
			else
				result.last = result.lineBreak = goIter;
		}

		result.autoLineBreak = result.lineBreak->autoLineBreak;

		// TODO : width
		result.size.width = result.maxWidth;

		int pxHSum = 0;

		float flHSum = calcFlWidthSum( result );


		CalcRow calcRow;
		calcPxWidthSum( result, calcRow );

		float freeHSpace = _in.maxWidth - calcRow.size.width;

		result.state = PS_ALL;

		if( freeHSpace >= 0 )
		{
			if( isCoeff( flHSum ) )
			{
				result.spacersCoeff = freeHSpace / flHSum;
			}
		}
		else
		{
			result.state = PS_SMALLER_SPACES;

			result.spacersCoeff = 0;

				//...
			//MYGUI_OUT( "SP" && freeHSpace );
		}
		
		calcPxWidthSum( result, calcRow );

		result.size.height = calcRow.size.height;

		// we reach end of row - jump over break line
		if( result.last == calcRow.last )
			result.last = result.lineBreak;
		else // no, it was the last posed
			result.last = calcRow.last;

		result.next = result.last;
		++result.next;

		//MYGUI_OUT( "T: ", result.size.height  );
		
		if( result.lineBreak != result.last && result.first != result.last
		 && isSpacer( result.lineBreak->widget ) )
		{
			result.size.height += result.lineBreak->sizeDesc.size.h.px();
		}

		return true;
	}

	void FlowContainer::placeWidgets( const RowData& _data )
	{
		ListWidgetInfoIter goIter = _data.first;

		IntPoint pos = _data.pos;

		for( ; goIter != mWidgetsInfo.end(); ++goIter )
		{
			Widget* widget = goIter->widget;
			SizeDescription& desc = goIter->sizeDesc;
			IntSize widgetSize = getWidgetPxSize( _data, *goIter );

			if( desc.size.w.isFl() )
			{
				widget->setSize( widgetSize );
			}

			widget->setPosition( pos );

			pos.left += widgetSize.width;

			if( goIter == _data.last )
				break;
		}

		if( _data.lineBreak != _data.last )
			pos.top += _data.lineBreak->sizeDesc.size.h.px();
	}

	float FlowContainer::calcFlHeightSum()
	{
		float result = 0.0f;

		float maxHFl = 0.0f;

		for( ListWidgetInfoIter goIter = mWidgetsInfo.begin(); goIter != mWidgetsInfo.end(); ++goIter )
		{
			if( goIter->sizeDesc.size.h.isFl() )
			{
				if( maxHFl < goIter->sizeDesc.size.h.fl() )
					maxHFl = goIter->sizeDesc.size.h.fl();
			}

			if( goIter->breakLine )
			{
				result += maxHFl;
				maxHFl = 0.0f;
			}
		}

		return result;
	}

	void FlowContainer::update()
	{
		IntSize contSize = getSize();

		RowData row;
		RowInput in;
		IntPoint curLocal( 0, 0 );
		ListWidgetInfoIter goIter = mWidgetsInfo.begin();

		while( goIter != mWidgetsInfo.end() )
		{
			// in
			in.cur = curLocal;
			in.from = goIter;
			in.maxWidth = contSize.width;

			bool res = getRowData( in, row );

			std::string s1 = row.first->widget->getName();
			std::string s2 = row.last->widget->getName();

			placeWidgets( row );

			curLocal.left = 0;
			curLocal.top += row.size.height;

			// next

			goIter = row.next;
		}

		// before this widgets were placed at horizontals

		float freeVSpace = contSize.height - curLocal.top;

		float flVSum = calcFlHeightSum();
		float vCoeff = 0;

		if( isCoeff( flVSum ) && freeVSpace > 0 )
			vCoeff = freeVSpace / flVSum;

		curLocal = IntPoint( 0, 0 );
		float moveDown = 0;

		for( goIter = mWidgetsInfo.begin(); goIter != mWidgetsInfo.end(); ++goIter )
		{
			if( goIter->sizeDesc.size.h.isFl() )
			{
				IntSize newSize = goIter->widget->getSize(); 

				newSize.height = goIter->sizeDesc.size.h.fl() * vCoeff;
				goIter->widget->setSize( newSize );
				moveDown += newSize.height;
			}
			else
			{
				IntPoint op = goIter->widget->getPosition(); 
				goIter->widget->setPosition( IntPoint( op.left, op.top + moveDown ) );
			}
		}
	}

} // namespace MyGUI

