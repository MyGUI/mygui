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

   	namespace ScopeComboBoxEvent_ComboChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ComboBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ComboBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ComboBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_DelegateComboChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_AdviseComboChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ComboBox* >(_widget)->eventComboChangePosition = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeComboBoxEvent_ComboAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ComboBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ComboBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ComboBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_DelegateComboAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_AdviseComboAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::ComboBox* >(_widget)->eventComboAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeComboBoxProperty_MaxListHeight
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_GetMaxListHeight( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ComboBox * >(_native)->getMaxListHeight( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetMaxListHeight( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setMaxListHeight( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeComboBoxProperty_SmoothShow
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportComboBox_GetSmoothShow( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ComboBox * >(_native)->getSmoothShow( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetSmoothShow( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setSmoothShow( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeComboBoxProperty_ComboModeDrop
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportComboBox_GetComboModeDrop( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ComboBox * >(_native)->getComboModeDrop( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetComboModeDrop( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setComboModeDrop( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeComboBoxMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ComboBox * >(_native)->beginToItemSelected( );
		}
	}



   	namespace ScopeComboBoxMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemLast( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ComboBox * >(_native)->beginToItemLast( );
		}
	}



   	namespace ScopeComboBoxMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemFirst( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ComboBox * >(_native)->beginToItemFirst( );
		}
	}



   	namespace ScopeComboBoxMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ComboBox * >(_native)->beginToItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeComboBoxMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportComboBox_GetItemNameAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::ComboBox * >(_native)->getItemNameAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeComboBoxMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetItemNameAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setItemNameAt(
				Convert<size_t>::From( _index ) ,
				Convert<const Ogre::UTFString &>::From( _name ) );
		}
	}



   	namespace ScopeComboBoxMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportComboBox_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::ComboBox * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeComboBoxMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ComboBox * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeComboBoxMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeComboBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ComboBox * >(_native)->clearIndexSelected( );
		}
	}



   	namespace ScopeComboBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ComboBox * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ComboBox * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeComboBoxMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_FindItemIndexWith_name( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ComboBox * >(_native)->findItemIndexWith(
				Convert<const Ogre::UTFString &>::From( _name ) ));
		}
	}



   	namespace ScopeComboBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ComboBox * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeComboBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ComboBox * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeComboBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_AddItem_name_data( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ComboBox * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_AddItem_name( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::ComboBox * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From( _name ) );
		}
	}



   	namespace ScopeComboBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_InsertItemAt_index_name_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ComboBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const Ogre::UTFString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_InsertItemAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::ComboBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const Ogre::UTFString &>::From( _name ) );
		}
	}



   	namespace ScopeComboBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ComboBox * >(_native)->getItemCount( ) );
		}
	}



   


   


}
