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
		:	Container( _style, _coord, _align, _info, _parent, _croppedParent, _creator, _name ),
			mHasLineBrakes( false )
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

	void FlowContainer::addLineBreak( int yOffset )
	{
		SpacerPtr spacer = createWidget< Spacer >( "Spacer", MyGUI::Align::Default );

		WidgetInfo* info = getWidgetInfo( spacer );
		info->sizeDesc.setSize( IntSize( INT_SIZE_UNBOUND.width, yOffset ) );
		info->sizeDesc.setMinSize( IntSize( INT_SIZE_UNBOUND.width, yOffset ) );
		info->sizeDesc.setMaxSize( IntSize( INT_SIZE_UNBOUND.width, yOffset ) );
		info->breakLine = true;

		update();

		mHasLineBrakes = true;
	}

	void FlowContainer::update()
	{
		IntPoint curLocal( 0, 0 );

		IntSize contSize = getSize();

		int curMaxHeightRow = 0;
		bool autoBrakeLine = false;

		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			IntSize maxAtRow( 0, 0 );

			autoBrakeLine = false;

			ListWidgetInfo::iterator startRow = widgetIter;
			ListWidgetInfo::iterator endRow = widgetIter;
			ListWidgetInfo::iterator breakLine = widgetIter;

			IntSize pxSum;
			FloatSize flSum;

			for( ListWidgetInfo::iterator drow = startRow; drow != mWidgetsInfo.end(); ++drow )
			{
				if( ! drow->sizeDesc.isChanged() )
				{
					drow->sizeDesc.setSize( drow->widget->getSize() );

					if( drow->widget->castType< Window >( false ) )
						drow->sizeDesc.setSize( drow->widget->getClientCoord().size() );
				}

				if( drow->breakLine )
				{
					breakLine = drow;
					break;
				}
				else
				{
					if( drow->sizeDesc.isPxSize() )
						pxSum.width += drow->sizeDesc.getPxSize().width;

					if( drow->sizeDesc.isFlSize() )
						flSum.width += drow->sizeDesc.getFlSize().width;

					endRow = drow;
				}
			}

			//if( endRow != mWidgetsInfo.end() )
			//	++endRow;

			autoBrakeLine = breakLine->autoBreakLine;

			IntSize freeSpace( contSize.width - pxSum.width, contSize.height - pxSum.height );
			FloatSize flCoeff;
			
			if( fabs( flSum.width ) > 0.0001 && freeSpace.width > 0 )
				flCoeff.width = ( (float)freeSpace.width ) / flSum.width;

			bool ignoreSpaces = false;

			for( ListWidgetInfo::iterator crow = startRow; ; ++crow )
			{
				IntSize childSize = crow->sizeDesc.getSize();

				if( autoBrakeLine && crow != endRow && curLocal.left + childSize.width > contSize.width )
				{
					curLocal.left = 0;
					curLocal.top += maxAtRow.height;
					maxAtRow = IntSize();
					ignoreSpaces = true;
				}

				if( ignoreSpaces  )
				{
					if( crow->widget->castType< Spacer >( false ) != 0 )
					{
					}
					else
						ignoreSpaces = false;
				}

				if( crow->sizeDesc.isPxSize() )
				{
					crow->widget->setPosition( curLocal );
				}

				if( crow->sizeDesc.isFlSize() )
				{
					childSize.width = (int)( flCoeff.width * crow->sizeDesc.getFlSize().width );
					crow->widget->setSize( IntSize( childSize.width, childSize.height ) );
				}

				if( maxAtRow.height < childSize.height )
					maxAtRow.height = childSize.height;

				if( ! ignoreSpaces )
					curLocal.left += childSize.width;

				if( crow == endRow )
					break;
			}

			widgetIter = endRow;

			if( widgetIter == mWidgetsInfo.end() )
				break;

			curLocal.left = 0;
			//curLocal.top += maxAtRow.height;
			curLocal.top += maxAtRow.height + breakLine->sizeDesc.getSize().height;

		}
	}

	IntSize FlowContainer::getMinSize( const WidgetInfo& info ) const
	{
		IntSize result;

		if( ! info.widget->getCaption().empty() )
		{
			result = info.sizeDesc.getMinSize();

			if( result == IntSize( -1, -1 ) )
			{
				result = info.widget->getSize();
			}
		}
		else
		{
			result = info.sizeDesc.getMinSize();

			if( result == IntSize( -1, -1 ) )
			{
				result = info.widget->getSize();
			}
		}		

		return result;
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

	void FlowContainer::add( WidgetPtr _widget )
	{
		WidgetInfo info( _widget );
		mWidgetsInfo.push_back( info );

		update();
	}

	void FlowContainer::remove( WidgetPtr _widget )
	{
		_destroyChildWidget( _widget );
	}

	SpacerPtr FlowContainer::addSpacer( const IntSize& pxSize )
	{
		SpacerPtr spacer = createWidget< Spacer >( "Spacer", pxSize, MyGUI::Align::Default );
		SizeDescription* szDesc = getSizeDescription( spacer );
		szDesc->setSize( pxSize );
		szDesc->setMinSize( pxSize );
		szDesc->setMaxSize( pxSize );
		return spacer;
	}

	SpacerPtr FlowContainer::addSpacer( const FloatSize& flSize )
	{
		SpacerPtr spacer = createWidget< Spacer >( "Spacer", MyGUI::Align::Default );

		SizeDescription* szDesc = getSizeDescription( spacer );
		szDesc->setSize( flSize );

		return spacer;
	}

} // namespace MyGUI

