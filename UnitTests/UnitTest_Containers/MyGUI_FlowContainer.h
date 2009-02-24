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

/*

BUGS:
Float height.

TODO


*/

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
	public:

		struct WidgetInfo : Container::BaseWidgetInfo
		{
			WidgetInfo( WidgetPtr _widget ) 
			:	Container::BaseWidgetInfo( _widget ),
				breakLine( false ),
				sizeDesc( _widget ),
				autoBreakLine( true ){ }
			SizeDescription sizeDesc;
			bool breakLine;
			bool autoBreakLine;// only if a break line!
		};

	protected:

		friend class factory::FlowContainerFactory;

		MYGUI_RTTI_CHILD_HEADER( FlowContainer, Widget );

	//protected:

		FlowContainer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		void update();

		WidgetPtr baseCreateWidget( WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name );

	
		void _destroyChildWidget( WidgetPtr _widget );

		IntSize getMinSize( const WidgetInfo& info ) const;

		WidgetInfo* getWidgetInfo( WidgetPtr _widget );

		//IntSize validateSize( const IntSize& size ) const;

	public:

		void add( WidgetPtr _widget );

		void remove( WidgetPtr _widget );

		SizeDescription* getSizeDescription( WidgetPtr _widget );
	
	protected:
		typedef std::list< WidgetInfo > ListWidgetInfo;

		ListWidgetInfo mWidgetsInfo;

		bool mHasLineBreaks;
	};

} // namespace MyGUI

#endif // __MYGUI_FLOW_CONTAINER_H__
