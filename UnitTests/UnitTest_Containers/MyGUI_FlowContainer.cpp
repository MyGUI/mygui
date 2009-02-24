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
			mHasLineBreaks( false )
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

	void FlowContainer::update()
	{
		IntPoint curLocal( 0, 0 );

		IntSize contSize = getSize();

		int curMaxHeightRow = 0;
		bool autoBrakeLine = false;

		bool hasLineBreaks = false;

		std::ofstream f( "mygui_flow_container_test_log.txt" );

		f << "StartSize: " << contSize << std::endl;

		f << "First" << std::endl;

		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			IntSize maxAtRow( 0, 0 );

			autoBrakeLine = false;

			ListWidgetInfo::iterator startRow = widgetIter;
			ListWidgetInfo::iterator endRow = widgetIter;
			ListWidgetInfo::iterator breakLine = widgetIter;

			int pxHSum = 0;
			int minPxHSpacerSize = 0;;
			float flHSum = 0.0f;

			f << "  C1" << std::endl;

			for( ListWidgetInfo::iterator drow = startRow; drow != mWidgetsInfo.end(); ++drow )
			{
				f << "    w";

				if( drow->widget->castType< Spacer >( false ) )
					f << " - is spacer";

				if( drow->breakLine )
					f << " - is break line";

				f << std::endl;

				if( ! drow->sizeDesc.isChanged() )
				{
					f << "      w-resize: was " << drow->sizeDesc.getSize();

					drow->sizeDesc.setSize( drow->widget->getSize() );

					if( drow->widget->castType< Window >( false ) )
						drow->sizeDesc.setSize( drow->widget->getClientCoord().size() );

					drow->sizeDesc.setSize( drow->widget->getSize() );

					f << " set to " << drow->sizeDesc.getSize() << std::endl;
				}

				if( drow->breakLine )
				{
					hasLineBreaks = true;
					breakLine = drow;
					break;
				}
				else
				{
					if( drow->sizeDesc.isPxSize() )
						pxHSum += drow->sizeDesc.getPxSize().width;

					if( drow->sizeDesc.isFlSize() )
						flHSum += drow->sizeDesc.getFlSize().width;

					if( drow->widget->castType< Spacer >( false ) )
						minPxHSpacerSize += drow->sizeDesc.getSize().width - drow->sizeDesc.getMinSize().width;

					endRow = drow;

					f << "      pxHSum " << pxHSum << " flHSum " << flHSum << std::endl;
				}
			}

			//if( endRow != mWidgetsInfo.end() )
			//	++endRow;

			autoBrakeLine = breakLine->autoBreakLine;

			float freeHSpace = contSize.width - pxHSum;

			f << "  FreeHSpace " << freeHSpace << std::endl;

			bool minimSpacers = false;
			float minimSpacerCoeff = 0.0f;

			if( freeHSpace < 0 )
			{
				freeHSpace += minPxHSpacerSize;

				if( freeHSpace >= 0 )
				{
					minimSpacers = true;
					minimSpacerCoeff = 1 + ((float) (minPxHSpacerSize - freeHSpace)) / ((float) minPxHSpacerSize );
				}
				else
				{

				}
			}

			FloatSize flCoeff;
			
			if( fabs( flHSum ) > 0.0001 && freeHSpace > 0 )
				flCoeff.width = freeHSpace / flHSum;

			f << "  Float coeff " << flCoeff.width << std::endl;

			bool ignoreSpaces = false;

			f << "  C2" << std::endl;

			for( ListWidgetInfo::iterator crow = startRow; ; ++crow )
			{
				f << "   w" << std::endl;

				IntSize childSize = crow->sizeDesc.getSize();

				if( ignoreSpaces && crow->widget->castType< Spacer >( false ) == nullptr )
					ignoreSpaces = false;

				if( autoBrakeLine && curLocal.left + childSize.width > contSize.width )
				{
					bool nextRow = true;

					if( crow->widget->castType< Spacer >( false ) != nullptr )
					{
						childSize.width = crow->sizeDesc.getMinSize().width;

						crow->widget->setSize( childSize );
						
						if( curLocal.left + childSize.width <= contSize.width )
						{
							
							nextRow = false;
						}
					}

					if( nextRow )
					{
						f << "   no space - next row: " << std::endl;
						curLocal.left = 0;
						curLocal.top += maxAtRow.height;
						maxAtRow = IntSize();
						ignoreSpaces = true;
					}
				}

				if( crow->sizeDesc.isPxSize() )
				{
					f << "  px size " << curLocal << std::endl;
					crow->widget->setPosition( curLocal );
				}

				if( crow->sizeDesc.isFlSize() )
				{
					childSize.width = (int)( flCoeff.width * crow->sizeDesc.getFlSize().width );
					f << "  (fl) size " << childSize.width << std::endl;
					crow->widget->setSize( childSize );
				}

				if( minimSpacers && crow->widget->castType< Spacer >( false ) != nullptr )
				{
					childSize.width = (crow->sizeDesc.getSize().width) / ( minimSpacerCoeff );
					crow->widget->setSize( childSize );
				}

				if( maxAtRow.height < childSize.height && breakLine != crow )
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

			//breakLine->widget->setPosition( curLocal.left, curLocal.top );

			//curLocal.top += breakLine->sizeDesc.getSize().height;

			f << "  Next non-breakline row" << std::endl << std::endl;

		}

		int pxVSum = 0;
		float flVSum = 0.0f;

		int maxHeightAtRow = 0;
		float maxFlVSum = 0;

		f << "Second" << std::endl;

		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			f << "  w" << std::endl;

			//if( widgetIter->sizeDesc.isFlSize() )
			//	flVSum += widgetIter->sizeDesc.getFlSize().height;
			if( widgetIter->sizeDesc.getFlSize().height > maxFlVSum )
				maxFlVSum = widgetIter->sizeDesc.getFlSize().height;

			if( widgetIter->sizeDesc.getPxSize().height > maxHeightAtRow )
				maxHeightAtRow = widgetIter->sizeDesc.getPxSize().height;

			f << "    maxFlVSum "  << maxFlVSum << " maxHeightAtRow " << maxHeightAtRow << std::endl;

			if( widgetIter->breakLine )
			{
				int s = widgetIter->sizeDesc.getSize().height;
				pxVSum += maxHeightAtRow + widgetIter->sizeDesc.getSize().height;
				flVSum += maxFlVSum;

				f << "    Line ended: pxVSum "  << pxVSum << " flVSum " << flVSum << std::endl;

				maxHeightAtRow = 0;
				maxFlVSum = 0;
			}
			else
			{
				//if( widgetIter->sizeDesc.getPxSize().height > maxHeightAtRow )
				//	maxHeightAtRow = widgetIter->sizeDesc.getPxSize().height;

			}
		}

		pxVSum += maxHeightAtRow;
		flVSum += maxFlVSum;

		float freeVSpace = ( float ) contSize.height - pxVSum;

		float vCoeff = 0;

		if( fabs( flVSum ) > 0.0001 && freeVSpace > 0 )
			vCoeff = freeVSpace / flVSum;

		curLocal = IntPoint( 0, 0 );
		float moveDown = 0;

		f << "FreeVSpace: " << freeVSpace << " VCoeff: " << vCoeff << std::endl << std::endl;;

		f << "Third" << std::endl;

		for( ListWidgetInfo::iterator widgetIter = mWidgetsInfo.begin(); widgetIter != mWidgetsInfo.end(); ++widgetIter )
		{
			f << "  w" << std::endl;

			if( widgetIter->sizeDesc.isFlSize() )
			{
				IntSize os = widgetIter->widget->getSize(); 
				os.height = widgetIter->sizeDesc.getFlSize().height * vCoeff;
				widgetIter->widget->setSize( IntSize( os.width, os.height ) );
				moveDown += os.height;

				f << "    Size: " << os << std::endl;
				f << "    MoveDown: " << moveDown << std::endl;
			}
			else
			{
				IntPoint op = widgetIter->widget->getPosition(); 
				widgetIter->widget->setPosition( IntPoint( op.left, op.top + moveDown ) );
				f << "    Pos: " << widgetIter->widget->getPosition() << std::endl;
			}
		}

		mHasLineBreaks = hasLineBreaks;
	}

	//IntSize FlowContainer::validateSize( const IntSize& size ) const
	//{
	//	IntSize result;

	//	if( size.width >= MyGUI::INT_SIZE_UNBOUND.width )
	//		size.width = 2000;

	//	if( size.height >= MyGUI::INT_SIZE_UNBOUND.height )
	//		size.height = 2000;

	//	return result;
	//}

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

} // namespace MyGUI

