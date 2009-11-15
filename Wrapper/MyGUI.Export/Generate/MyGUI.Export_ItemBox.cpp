/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeItemBoxEvent_NotifyItem
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<const MyGUI::IBNotifyItemData &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			const MyGUI::IBNotifyItemData & _info )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<const MyGUI::IBNotifyItemData &>::To( _info ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateNotifyItem( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseNotifyItem( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->eventNotifyItem = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_MouseItemActivate
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateMouseItemActivate( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseMouseItemActivate( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->eventMouseItemActivate = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_ChangeItemPosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateChangeItemPosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseChangeItemPosition( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->eventChangeItemPosition = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_SelectItemAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateSelectItemAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseSelectItemAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->eventSelectItemAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_DrawItem
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<MyGUI::Widget *>::Type ,
			Convert<const MyGUI::IBDrawItemInfo &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			MyGUI::Widget * _item ,
			const MyGUI::IBDrawItemInfo & _info )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _item ) ,
				Convert<const MyGUI::IBDrawItemInfo &>::To( _info ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateDrawItem( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseDrawItem( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->requestDrawItem = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_CoordItem
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<MyGUI::types::TCoord< int > &>::Type ,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			MyGUI::types::TCoord< int > & _coord ,
			bool _drag )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<MyGUI::types::TCoord< int > &>::To( _coord ) ,
				Convert<bool>::To( _drag ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateCoordItem( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseCoordItem( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->requestCoordItem = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeItemBoxEvent_CreateWidgetItem
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ItemBox *>::Type ,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ItemBox * _sender ,
			MyGUI::Widget * _item )
		{
			mExportHandle(
				Convert<MyGUI::ItemBox *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _item ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_DelegateCreateWidgetItem( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportItemBoxEvent_AdviseCreateWidgetItem( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ItemBox* >(_widget)->requestCreateWidgetItem = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   


   


   


   


   


   


   	namespace ScopeItemBoxMethod_ResetDrag
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_ResetDrag( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->resetDrag( );
		}
	}



   	namespace ScopeItemBoxMethod_GetWidgetByIndex
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportItemBox_GetWidgetByIndex_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::ItemBox * >(_native)->getWidgetByIndex(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeItemBoxMethod_GetWidgetDrag
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportItemBox_GetWidgetDrag( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::ItemBox * >(_native)->getWidgetDrag( ) );
		}
	}



   	namespace ScopeItemBoxMethod_GetIndexByWidget
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetIndexByWidget_widget( MyGUI::Widget* _native,
			Convert<MyGUI::Widget *>::Type _widget )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getIndexByWidget(
				Convert<MyGUI::Widget *>::From( _widget ) ));
		}
	}



   	namespace ScopeItemBoxProperty_ItemBoxAlignVert
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportItemBox_GetItemBoxAlignVert( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ItemBox * >(_native)->getItemBoxAlignVert( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetItemBoxAlignVert( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setItemBoxAlignVert( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeItemBoxMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportItemBox_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::ItemBox * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeItemBoxMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeItemBoxMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeItemBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->clearIndexSelected( );
		}
	}



   	namespace ScopeItemBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeItemBoxMethod_RedrawAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RedrawAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->redrawAllItems( );
		}
	}



   	namespace ScopeItemBoxMethod_RedrawItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RedrawItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->redrawItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeItemBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeItemBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeItemBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_AddItem_data( MyGUI::Widget* _native,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->addItem(
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_AddItem( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->addItem( );
		}
	}



   	namespace ScopeItemBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_InsertItemAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_InsertItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeItemBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getItemCount( ) );
		}
	}



   


   


   


}
