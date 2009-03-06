/*!
	@file
	@author		Alexander Ptakhin
	@date		03/2009
	@module
*/
#ifndef __MYGUI_FLOW_CONTAINER_H__
#define __MYGUI_FLOW_CONTAINER_H__

#include "MyGUI_Container.h"
#include "MyGUI_Window.h"

/*

BUGS:
=)

TODO
min/max
xml-loading
layout editor implementing

*/

namespace MyGUI
{
	namespace factory
	{
		class FlowContainerFactory;
	}

	class WidgetParamWrap_Width{};
	class WidgetParamWrap_Height{};

	template< typename T >
	struct WidgetParamWrap
	{
		const SizeDescription& desc;
		const SizeParam& dim() const{ MYGUI_EXCEPT( "Wrong templation!" ); }
		const SizeParam& minDim() const{ MYGUI_EXCEPT( "Wrong templation!" ); }
		const SizeParam& maxDim() const{ MYGUI_EXCEPT( "Wrong templation!" ); } 

		WidgetParamWrap( const SizeDescription& _desc )	:	desc( _desc ){}
	};

	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Width >::dim() const
	{
		return desc.size.w;
	}
	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Width >::minDim() const
	{
		return desc.minSize.w;
	}
	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Width >::maxDim() const
	{
		return desc.maxSize.w;
	}

	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Height >::dim() const
	{
		return desc.size.h;
	}
	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Height >::minDim() const
	{
		return desc.minSize.h;
	}
	template<> const SizeParam& WidgetParamWrap< WidgetParamWrap_Height >::maxDim() const
	{
		return desc.maxSize.h;
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
				autoLineBreak( true ){ }
			SizeDescription sizeDesc;
			bool breakLine;
			bool autoLineBreak;// only if a break line! or not only =)
		};

	protected:

		friend class factory::FlowContainerFactory;

		MYGUI_RTTI_CHILD_HEADER( FlowContainer, Widget );

	//protected:

		FlowContainer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name );

		void update();

		WidgetPtr baseCreateWidget( WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name );

	
		void _destroyChildWidget( WidgetPtr _widget );

		WidgetInfo* getWidgetInfo( WidgetPtr _widget );

		typedef std::list< WidgetInfo > ListWidgetInfo;
		typedef std::list< WidgetInfo >::iterator ListWidgetInfoIter;

		enum PoseState
		{
			PS_ALL,
			PS_SMALLER_SPACES,
			PS_SMALLER_ALL,
		};

		struct SizeData
		{
			float spacersCoeff;
			float mainCoeff;

			PoseState state;

			SizeData() : mainCoeff( 0 ),
				spacersCoeff( 0 ), state( PS_ALL ){}
		};

		struct LocalColumnData : public SizeData
		{
			
		};

		struct RowData : public SizeData
		{
			IntPoint pos;
			ListWidgetInfoIter first;
			ListWidgetInfoIter last; // last non-space 
			ListWidgetInfoIter lineBreak;// which breaks
			ListWidgetInfoIter next;
			bool autoLineBreak;
			IntSize size;
			int maxWidth;

			LocalColumnData vdata;

			RowData() : autoLineBreak( false ), maxWidth( 0 ) {}
		};

		struct RowInput
		{
			IntPoint cur;
			ListWidgetInfoIter from;
			int maxWidth;
		};

		bool isCoeff( float _coeff ) const { return fabs( _coeff ) > 0.00001; }

		bool isSpacer( const WidgetPtr _widget ) const;

		bool getRowData( const RowInput& _in, RowData& _result );

		void placeWidgets( const RowData& _data );

		struct CalcRow
		{
			IntSize size;
			ListWidgetInfoIter last;
		};

		float calcFlWidthSum( const RowData& _data );
		bool calcPxWidthSum( const RowData& _data, CalcRow& _calcRow );
		float calcFlHeightSum();

		template< class T >
		int _getWidgetPxDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info );

		int getWidgetPxWidth( const RowData& _data, const WidgetInfo& _info );

		int getWidgetPxHeight( const RowData& _data, const WidgetInfo& _info );

		template< class T >
		float _getWidgetFlDimension( const SizeData& _sizeData, const T& _size, const WidgetInfo& _info );

		float getWidgetFlWidth( const RowData& _data, const WidgetInfo& _info );

		float getWidgetFlHeight( const RowData& _data, const WidgetInfo& _info );

		IntSize getWidgetPxSize( const RowData& _data, const WidgetInfo& _info );

		// TODO:
		IntSize getWidgetMinSize( const WidgetInfo& info ) const;

	public:

		void add( WidgetPtr _widget );

		void remove( WidgetPtr _widget );

		SizeDescription* getSizeDescription( WidgetPtr _widget );

		void updateWidgetInfo( WidgetPtr _widget );

		void updateWidgetInfo( WidgetInfo& _widgetInfo );
	
	protected:
		
		ListWidgetInfo mWidgetsInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_FLOW_CONTAINER_H__
