/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#ifndef __MYGUI_FLOW_CONTAINER_H__
#define __MYGUI_FLOW_CONTAINER_H__

#include "MyGUI_Container.h"
#include "MyGUI_Window.h"

namespace MyGUI
{
	namespace factory
	{
		class FlowContainerFactory;
	}

	class FlowContainer;
	typedef FlowContainer* FlowContainerPtr;

	/** Flow container. Data stores from left to right. 
	Modes:
	Break lines:
	Wider: If no place for widget, set it next row.
	*/
	class /*MYGUI_EXPORT*/ FlowContainer : public Container
	{
	protected:

		friend class factory::FlowContainerFactory;

		MYGUI_RTTI_CHILD_HEADER( FlowContainer, Widget );

		FlowContainer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		void update();

		struct WidgetInfo : Container::BaseWidgetInfo
		{
			WidgetInfo( WidgetPtr _widget ) 
			:	Container::BaseWidgetInfo( _widget ),
				breakLine( false ),
				sizeDesc( _widget ),
				autoBreakLine( true ){ }
			SizeDescription sizeDesc;
			bool breakLine;
			bool autoBreakLine;// only if a brake line!
		};
		
		void _destroyChildWidget( WidgetPtr _widget );

		IntSize getMinSize( const WidgetInfo& info ) const;

		WidgetInfo* getWidgetInfo( WidgetPtr _widget );

	public:
		void add( WidgetPtr _widget );

		void remove( WidgetPtr _widget );

		void addLineBreak( int yOffset );

		template< typename T >
		T* createWidget( const std::string & _skin, Align _align, const std::string & _name = "" )
		{
			T* widget = static_cast< T* >( createWidgetT( T::getClassTypeName(), _skin, IntCoord(), _align, _name ) );
			return widget;
		}

		template< typename T >
		T* createWidget( const std::string & _skin, IntSize size, Align _align, const std::string & _name = "" )
		{
			T* widget = static_cast< T* >( createWidgetT( T::getClassTypeName(), _skin, IntCoord( IntPoint(), size ), _align, _name ) );
			return widget;
		}

		template< typename T >
		T* createWidget( const std::string & _skin, const std::string & _name = "" )
		{
			T* widget = static_cast< T* >( createWidgetT( T::getClassTypeName(), _skin, IntCoord(), Align::Default, _name ) );
			return widget;
		}

		SpacerPtr addSpacer( const IntSize& pxSize );
		SpacerPtr addSpacer( const FloatSize& flSize );

		SizeDescription* getSizeDescription( WidgetPtr _widget );


	
	protected:
		typedef std::list< WidgetInfo > ListWidgetInfo;

		ListWidgetInfo mWidgetsInfo;

		bool mHasLineBrakes;
	};

} // namespace MyGUI

#endif // __MYGUI_FLOW_CONTAINER_H__
