/*!
	@file
	@author		Alexander Ptakhin
	@date		04/2009
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_FlowContainer.h"

namespace MyGUI
{
	FlowContainer::WidgetInfo::WidgetInfo( WidgetPtr _widget )
		:	Container::BaseWidgetInfo( _widget ),
			mWasLoaded( false )
	{
	}

	void FlowContainer::WidgetInfo::_load()
	{
		if( !mWasLoaded )
		{
			// nothing to load
			if( !widget->isUserString( "MGI_Size" ) && ! widget->isUserString( "MGI_BreakLine" ) )
				return;

			lineBreak = utility::parseBool( widget->getUserString( "MGI_BreakLine" ) );
			
			autoLineBreak = utility::parseBool( widget->getUserString( "MGI_AutolLineBreak" ) );

			size.fromString( widget->getUserString( "MGI_Size" ) );
			minSize.fromString( widget->getUserString( "MGI_MinSize" ) );

			mWasLoaded = true;
		}
	}

	FlowContainer::FlowContainer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name )
		:	Container( _style, _coord, _align, _info, _parent, _croppedParent, _creator, _name )
	{
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

	void FlowContainer::updateAllWidgetInfos()
	{
		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			updateWidgetInfo( *widgetIter );
		}
	}

	void FlowContainer::updateWidgetInfo( WidgetInfo& _widgetInfo )
	{
		_widgetInfo._load();

		_widgetInfo.minSize.px( getWidgetMinSize( _widgetInfo ) );

		IntSize t = _widgetInfo.widget->_getTextSize();

		if( _widgetInfo.size.isNull() )
			_widgetInfo.size.px( _widgetInfo.widget->getSize() );		
	}
	
	IntSize FlowContainer::getWidgetMinSize( const WidgetInfo& _info ) const
	{
		WidgetPtr widget = _info.widget;

		IntSize s = widget->getTextSize();

		std::string w = widget->getName();

		return widget->getTextSize();
	}

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
		return IntSize( getWidgetPxWidth( _data, _info ), _info.size.h.px() );
	}

	template< class T >
	bool FlowContainer::_isWidgetComply( int _widgetTags, const T& _size, const WidgetInfo& _info ) const
	{
		if( _widgetTags == WT_ALL )
			return true;

		if( ( _widgetTags & WT_NOT_SPACER ) && !isSpacer( _info.widget ) )
			return true;

		if( ( _widgetTags & WT_FREE ) && _size.dim().isFreeSpaceFl() )
			return true;

		if( ( _widgetTags & WT_PARENT ) && _size.dim().isParentFl() )
			return true;

		if( ( _widgetTags & WT_SPACER ) && isSpacer( _info.widget ) )
			return true;

		return false;
	}

	bool FlowContainer::isWidgetWidthComply( int _widgetTags, const WidgetInfo& _info ) const
	{
		WidgetParamWrap< WidgetParamWrap_Width > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _isWidgetComply( _widgetTags, sizeWrap, _info );
	}

	bool FlowContainer::isWidgetHeightComply( int _widgetTags, const WidgetInfo& _info ) const
	{
		WidgetParamWrap< WidgetParamWrap_Height > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _isWidgetComply( _widgetTags, sizeWrap, _info );
	}
	
	template< class T >
	int FlowContainer::_getWidgetPxDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info )
	{
		int px = 0;

		if( _isWidgetComply( _sizeData.state, _size, _info )  )
		{
			// spacer is "px" will be minimum in any case
			if( isSpacer( _info.widget ) && isCoeff( _sizeData.spacersCoeff ) )
			{
				if( _size.dim().isFl() )
					return _size.dim().fl() * _sizeData.spacersCoeff;
				else
					return ( (float)_size.dim().px() ) * _sizeData.spacersCoeff;
			}

			if( _size.dim().isPx() )
				return _size.dim().px();

			if( _size.dim().isParentFl() && isCoeff( _sizeData.parentCoeff ) )
				return _size.dim().fl() * _sizeData.parentCoeff;

			if( _size.dim().isFreeSpaceFl() && isCoeff( _sizeData.freeCoeff ) )
				return _size.dim().fl() * _sizeData.freeCoeff;
		}

		// can't be smaller than minimum size
		return std::max( px, _size.minDim().px() );
	}

	int FlowContainer::getWidgetPxWidth( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Width > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _getWidgetPxDimension( _data, sizeWrap, _info );
	}

	int FlowContainer::getWidgetPxHeight( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Height > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _getWidgetPxDimension( _data, sizeWrap, _info );
	}

	template< class T >
	float FlowContainer::_getWidgetFlDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info )
	{
		if( _sizeData.state & WT_SPACER )
		{
			if( isSpacer( _info.widget ) )
				return _size.minDim().fl();
		}

		if( _sizeData.state & WT_PARENT )
		{
			if( _size.dim().isParentFl() )
				return _size.minDim().fl();
		}

		return _size.dim().fl();
	}

	float FlowContainer::getWidgetFlWidth( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Width > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _getWidgetFlDimension( _data, sizeWrap, _info );
	}

	float FlowContainer::getWidgetFlHeight( const RowData& _data, const WidgetInfo& _info )
	{
		WidgetParamWrap< WidgetParamWrap_Height > sizeWrap( _info.size, _info.minSize, _info.maxSize );
		return _getWidgetFlDimension( _data, sizeWrap, _info );
	}

	float FlowContainer::calcFlWidthSum( const RowData& _data, FloatMode _mode )
	{
		ListWidgetInfoIter goIter = _data.first;
		float flHSum = 0;

		while( goIter != mWidgetsInfo.end() )
		{
			bool count = goIter->size.w.isFloatMode( _mode );

			if( count )
				flHSum += goIter->size.w.fl();

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

	bool FlowContainer::calcPxWidthSum( const RowData& _data, CalcRow& _calcRow, int _widgetTags )
	{
		ListWidgetInfoIter goIter = _data.first;
		bool isFirstAtRow = true;

		_calcRow.size = IntSize();	

		bool breakRow = _widgetTags == WT_ALL;

		while( goIter != mWidgetsInfo.end() )
		{
			bool toNextRow = false;

			Widget* widget = goIter->widget;

			bool count = isWidgetWidthComply( _widgetTags, *goIter );

			if( count )
			{
				IntSize widgetSize = getWidgetPxSize( _data, *goIter );

				goIter->setCurrentSize( widgetSize );

				if( _calcRow.size.height < widgetSize.height )
					_calcRow.size.height = widgetSize.height;
				
				// width here 

				bool ignoreNextRow = false;

				if( breakRow && !ignoreNextRow && _data.autoLineBreak && _calcRow.size.width + widgetSize.width > _data.maxWidth )
				{
					toNextRow = true;

					if( ! isFirstAtRow )
						break;
					else if( isSpacer( widget ) )
					{
						isFirstAtRow = false;
						++goIter;

						if( goIter == _data.last )
							break;

						continue;
					}
				}

				_calcRow.size.width += widgetSize.width;
			}

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
			if( goIter->lineBreak )
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

		result.parentCoeff = result.size.width;

		result.state = WT_ALL;

		CalcRow calcRow;
		calcPxWidthSum( result, calcRow, WT_ALL );

		float flFreeHSum = calcFlWidthSum( result, FM_FREE_SPACE );
		float freeHSpace = _in.maxWidth - calcRow.size.width;		

		// We have too much space :)
		if( freeHSpace >= 0 )
		{
			// We have float spacers - calculate coefficient
			if( isCoeff( flFreeHSum ) )
			{
				result.freeCoeff = freeHSpace / flFreeHSum;
			}
		}
		else
		{
			// make spacers smaller
			result.state ^= WT_SPACER;

			//result.freeCoeff = 0;
			result.spacersCoeff = 0;

			calcPxWidthSum( result, calcRow, WT_ALL );

			freeHSpace = _in.maxWidth - calcRow.size.width;

			calcPxWidthSum( result, calcRow, WT_SPACER );
			flFreeHSum = result.size.width;

			if( freeHSpace > 0 )
			{
				result.freeCoeff = freeHSpace / flFreeHSum;
				MYGUI_OUT_SPACES( "Ok", freeHSpace, flFreeHSum, result.first->widget->getCaption() );
			}
			else
			{
				MYGUI_OUT( "Space!" );
			}
		}
		
		calcPxWidthSum( result, calcRow, WT_ALL );

		result.size.height = calcRow.size.height;

		// we've reached end of row - jump over break line
		if( result.last == calcRow.last )
			result.last = result.lineBreak;
		else // no, it was the last posed
			result.last = calcRow.last;

		result.next = result.last;
		++result.next;

		// hacks, hacks, hacks
		if( result.lineBreak != result.last && result.first != result.last
		 && isSpacer( result.lineBreak->widget ) )
		{
			result.size.height += result.lineBreak->size.h.px();
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
			//SizeDescription& desc = goIter->sizeDesc;

			/*IntSize widgetSize = getWidgetPxSize( _data, *goIter );

			if( desc.size.w.isFl() )
			{
				widget->setSize( widgetSize );
			}*/

			IntSize widgetSize = goIter->getCurrentSize();

			widget->setSize( widgetSize );

			widget->setPosition( pos );

			pos.left += widgetSize.width;

			if( goIter == _data.last )
				break;
		}

		if( _data.lineBreak != _data.last )
			pos.top += _data.lineBreak->size.h.px();
	}

	float FlowContainer::calcFlHeightSum( FloatMode _mode )
	{
		float result = 0.0f;
		float maxHFl = 0.0f;

		for( ListWidgetInfoIter goIter = mWidgetsInfo.begin(); goIter != mWidgetsInfo.end(); ++goIter )
		{
			if( goIter->size.h.isFloatMode( _mode ) )
			{
				if( goIter->size.h.isFl() )
				{
					if( maxHFl < goIter->size.h.fl() )
						maxHFl = goIter->size.h.fl();
				}
			}

			if( goIter->lineBreak )
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

		if( contSize.width <= 0 || contSize.height <= 0 )
			return;

		RowData row;
		RowInput in;
		IntPoint curLocal( 0, 0 );
		ListWidgetInfoIter goIter = mWidgetsInfo.begin();

		while( goIter != mWidgetsInfo.end() )
		{
			in.cur = curLocal;
			in.from = goIter;
			in.maxWidth = contSize.width;

			bool result = getRowData( in, row );

			std::string s1 = row.first->widget->getName();
			std::string s2 = row.last->widget->getName();

			placeWidgets( row );

			curLocal.left = 0;
			curLocal.top += row.size.height;

			// next

			goIter = row.next;
		}

		// before this widgets have to be placed at horizontals, now align them at verticals

		float freeVSpace = contSize.height - curLocal.top;

		float flVSum = calcFlHeightSum( FM_FREE_SPACE );
		float vCoeff = 0;

		if( isCoeff( flVSum ) && freeVSpace > 0 )
			vCoeff = freeVSpace / flVSum;

		curLocal = IntPoint( 0, 0 );
		float moveDown = 0;

		for( goIter = mWidgetsInfo.begin(); goIter != mWidgetsInfo.end(); ++goIter )
		{
			if( goIter->size.h.isFl() )
			{
				IntSize newSize = goIter->widget->getSize(); 

				newSize.height = goIter->size.h.fl() * vCoeff;
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

