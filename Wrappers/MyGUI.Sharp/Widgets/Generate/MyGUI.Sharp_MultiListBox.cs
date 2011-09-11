/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public  class MultiListBox : Widget
    {

        #region MultiListBox

        protected override string GetWidgetType() { return "MultiListBox"; }

        internal static BaseWidget RequestWrapMultiListBox(BaseWidget _parent, IntPtr _widget)
        {
			MultiListBox widget = new MultiListBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMultiListBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MultiListBox widget = new MultiListBox();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint



   		#region Event ListChangePosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBoxEvent_AdviseListChangePosition( IntPtr _native, bool _advise );

		public delegate void HandleListChangePosition(
			 MultiListBox _sender ,
			 uint _index );
			
		private HandleListChangePosition mEventListChangePosition;
		public event HandleListChangePosition EventListChangePosition
		{
			add
			{
				if (mEventListChangePosition == null) ExportMultiListBoxEvent_AdviseListChangePosition( mNative, true );
				mEventListChangePosition += value;
			}
			remove
			{
				mEventListChangePosition -= value;
				if (mEventListChangePosition == null) ExportMultiListBoxEvent_AdviseListChangePosition( mNative, false );
			}
		}


		private struct ExportEventListChangePosition
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMultiListBoxEvent_DelegateListChangePosition( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  MultiListBox _sender ,
				  uint _index );
				
			private static ExportHandle mDelegate;
			public ExportEventListChangePosition( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMultiListBoxEvent_DelegateListChangePosition( mDelegate );
			}
		}
		static ExportEventListChangePosition mExportListChangePosition =
			new ExportEventListChangePosition(new ExportEventListChangePosition.ExportHandle( OnExportListChangePosition ));

		private static void OnExportListChangePosition(
			 MultiListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListChangePosition != null)
				_sender.mEventListChangePosition(
					 _sender ,
					 _index );
		}

		#endregion



   		#region Event ListSelectAccept

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBoxEvent_AdviseListSelectAccept( IntPtr _native, bool _advise );

		public delegate void HandleListSelectAccept(
			 MultiListBox _sender ,
			 uint _index );
			
		private HandleListSelectAccept mEventListSelectAccept;
		public event HandleListSelectAccept EventListSelectAccept
		{
			add
			{
				if (mEventListSelectAccept == null) ExportMultiListBoxEvent_AdviseListSelectAccept( mNative, true );
				mEventListSelectAccept += value;
			}
			remove
			{
				mEventListSelectAccept -= value;
				if (mEventListSelectAccept == null) ExportMultiListBoxEvent_AdviseListSelectAccept( mNative, false );
			}
		}


		private struct ExportEventListSelectAccept
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMultiListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  MultiListBox _sender ,
				  uint _index );
				
			private static ExportHandle mDelegate;
			public ExportEventListSelectAccept( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMultiListBoxEvent_DelegateListSelectAccept( mDelegate );
			}
		}
		static ExportEventListSelectAccept mExportListSelectAccept =
			new ExportEventListSelectAccept(new ExportEventListSelectAccept.ExportHandle( OnExportListSelectAccept ));

		private static void OnExportListSelectAccept(
			 MultiListBox _sender ,
			 uint _index )
		{
			if (_sender.mEventListSelectAccept != null)
				_sender.mEventListSelectAccept(
					 _sender ,
					 _index );
		}

		#endregion



   		#region Method GetSubItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMultiListBox_GetSubItemDataAt_column_index( IntPtr _native ,
			  uint _column ,
			  uint _index );

		public object GetSubItemDataAt(
			uint _column ,
			uint _index )
		{
			return ExportMultiListBox_GetSubItemDataAt_column_index( mNative , 
				 _column ,
				 _index );
		}

		#endregion



   		#region Method ClearSubItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearSubItemDataAt_column_index( IntPtr _native ,
			  uint _column ,
			  uint _index );

		public void ClearSubItemDataAt(
			uint _column ,
			uint _index )
		{
			ExportMultiListBox_ClearSubItemDataAt_column_index( mNative , 
				 _column ,
				 _index );
		}

		#endregion



   		#region Method SetSubItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetSubItemDataAt_column_index_data( IntPtr _native ,
			  uint _column ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetSubItemDataAt(
			uint _column ,
			uint _index ,
			object _data )
		{
			ExportMultiListBox_SetSubItemDataAt_column_index_data( mNative , 
				 _column ,
				 _index ,
				 _data );
		}

		#endregion



   		#region Method FindSubItemWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_FindSubItemWith_column_name( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindSubItemWith(
			uint _column ,
			string _name )
		{
			return  ExportMultiListBox_FindSubItemWith_column_name( mNative , 
				 _column ,
				 _name )  ;
		}

		#endregion



   		#region Method GetSubItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetSubItemNameAt_column_index( IntPtr _native ,
			  uint _column ,
			  uint _index );

		public string GetSubItemNameAt(
			uint _column ,
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetSubItemNameAt_column_index( mNative , 
				 _column ,
				 _index )  )  ;
		}

		#endregion



   		#region Method SetSubItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetSubItemNameAt_column_index_name( IntPtr _native ,
			  uint _column ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetSubItemNameAt(
			uint _column ,
			uint _index ,
			string _name )
		{
			ExportMultiListBox_SetSubItemNameAt_column_index_name( mNative , 
				 _column ,
				 _index ,
				 _name );
		}

		#endregion



   		#region Method GetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMultiListBox_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportMultiListBox_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportMultiListBox_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportMultiListBox_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Method ClearIndexSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearIndexSelected( IntPtr _native );

		public void ClearIndexSelected( )
		{
			ExportMultiListBox_ClearIndexSelected(  mNative );
		}

		#endregion



   		#region Property IndexSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetIndexSelected( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportMultiListBox_GetIndexSelected( mNative )  ; }
			set { ExportMultiListBox_SetIndexSelected( mNative,  value ); }
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportMultiListBox_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Method SwapItemsAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SwapItemsAt_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapItemsAt(
			uint _index1 ,
			uint _index2 )
		{
			ExportMultiListBox_SwapItemsAt_index1_index2( mNative , 
				 _index1 ,
				 _index2 );
		}

		#endregion



   		#region Method RemoveAllItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportMultiListBox_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportMultiListBox_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddItem_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void AddItem(
			string _name ,
			object _data )
		{
			ExportMultiListBox_AddItem_name_data( mNative , 
				 _name ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void AddItem(
			string _name )
		{
			ExportMultiListBox_AddItem_name( mNative , 
				 _name );
		}

		#endregion



   		#region Method InsertItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertItemAt_index_name_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void InsertItemAt(
			uint _index ,
			string _name ,
			object _data )
		{
			ExportMultiListBox_InsertItemAt_index_name_data( mNative , 
				 _index ,
				 _name ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void InsertItemAt(
			uint _index ,
			string _name )
		{
			ExportMultiListBox_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Property ItemCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportMultiListBox_GetItemCount( mNative )  ; }
		}

		#endregion



   		#region Method GetColumnDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMultiListBox_GetColumnDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetColumnDataAt(
			uint _index )
		{
			return ExportMultiListBox_GetColumnDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearColumnDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearColumnDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearColumnDataAt(
			uint _index )
		{
			ExportMultiListBox_ClearColumnDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetColumnDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetColumnDataAt(
			uint _index ,
			object _data )
		{
			ExportMultiListBox_SetColumnDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Method SetColumnResizingPolicyAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnResizingPolicyAt_index_value( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.I4)]  ResizingPolicy _value );

		public void SetColumnResizingPolicyAt(
			uint _index ,
			ResizingPolicy _value )
		{
			ExportMultiListBox_SetColumnResizingPolicyAt_index_value( mNative , 
				 _index ,
				 _value );
		}

		#endregion



   		#region Method SetColumnResizingPolicy

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnResizingPolicy_item_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.I4)]  ResizingPolicy _value );

		public void SetColumnResizingPolicy(
			MultiListItem _item ,
			ResizingPolicy _value )
		{
			ExportMultiListBox_SetColumnResizingPolicy_item_value( mNative , 
				 _item ,
				 _value );
		}

		#endregion



   		#region Method GetColumnIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetColumnIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint GetColumnIndex(
			MultiListItem _item )
		{
			return  ExportMultiListBox_GetColumnIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method SortByColumn

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SortByColumn_column_backward( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.U1)]  bool _backward );

		public void SortByColumn(
			uint _column ,
			bool _backward )
		{
			ExportMultiListBox_SortByColumn_column_backward( mNative , 
				 _column ,
				 _backward );
		}

		#endregion



   		#region Method GetColumnWidthAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportMultiListBox_GetColumnWidthAt_column( IntPtr _native ,
			  uint _column );

		public int GetColumnWidthAt(
			uint _column )
		{
			return  ExportMultiListBox_GetColumnWidthAt_column( mNative , 
				 _column )  ;
		}

		#endregion



   		#region Method GetColumnName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetColumnName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public string GetColumnName(
			MultiListItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetColumnName_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetColumnNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetColumnNameAt_column( IntPtr _native ,
			  uint _column );

		public string GetColumnNameAt(
			uint _column )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetColumnNameAt_column( mNative , 
				 _column )  )  ;
		}

		#endregion



   		#region Method SetColumnWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnWidth_item_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			  int _width );

		public void SetColumnWidth(
			MultiListItem _item ,
			int _width )
		{
			ExportMultiListBox_SetColumnWidth_item_width( mNative , 
				 _item ,
				 _width );
		}

		#endregion



   		#region Method SetColumnWidthAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnWidthAt_column_width( IntPtr _native ,
			  uint _column ,
			  int _width );

		public void SetColumnWidthAt(
			uint _column ,
			int _width )
		{
			ExportMultiListBox_SetColumnWidthAt_column_width( mNative , 
				 _column ,
				 _width );
		}

		#endregion



   		#region Method SetColumnName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetColumnName(
			MultiListItem _item ,
			string _name )
		{
			ExportMultiListBox_SetColumnName_item_name( mNative , 
				 _item ,
				 _name );
		}

		#endregion



   		#region Method SetColumnNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnNameAt_column_name( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetColumnNameAt(
			uint _column ,
			string _name )
		{
			ExportMultiListBox_SetColumnNameAt_column_name( mNative , 
				 _column ,
				 _name );
		}

		#endregion



   		#region Method SwapColumnsAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SwapColumnsAt_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapColumnsAt(
			uint _index1 ,
			uint _index2 )
		{
			ExportMultiListBox_SwapColumnsAt_index1_index2( mNative , 
				 _index1 ,
				 _index2 );
		}

		#endregion



   		#region Method RemoveAllColumns

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllColumns( IntPtr _native );

		public void RemoveAllColumns( )
		{
			ExportMultiListBox_RemoveAllColumns(  mNative );
		}

		#endregion



   		#region Method RemoveColumnAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveColumnAt_column( IntPtr _native ,
			  uint _column );

		public void RemoveColumnAt(
			uint _column )
		{
			ExportMultiListBox_RemoveColumnAt_column( mNative , 
				 _column );
		}

		#endregion



   		#region Method AddColumn

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddColumn_name_width_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void AddColumn(
			string _name ,
			int _width ,
			object _data )
		{
			ExportMultiListBox_AddColumn_name_width_data( mNative , 
				 _name ,
				 _width ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddColumn_name_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width );

		public void AddColumn(
			string _name ,
			int _width )
		{
			ExportMultiListBox_AddColumn_name_width( mNative , 
				 _name ,
				 _width );
		}

		#endregion



   		#region Method InsertColumnAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertColumnAt_column_name_width_data( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void InsertColumnAt(
			uint _column ,
			string _name ,
			int _width ,
			object _data )
		{
			ExportMultiListBox_InsertColumnAt_column_name_width_data( mNative , 
				 _column ,
				 _name ,
				 _width ,
				 _data );
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertColumnAt_column_name_width( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width );

		public void InsertColumnAt(
			uint _column ,
			string _name ,
			int _width )
		{
			ExportMultiListBox_InsertColumnAt_column_name_width( mNative , 
				 _column ,
				 _name ,
				 _width );
		}

		#endregion



   		#region Property ColumnCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetColumnCount( IntPtr _native );

		public uint ColumnCount
		{
			get { return  ExportMultiListBox_GetColumnCount( mNative )  ; }
		}

		#endregion



   


   


   


   


   


   


   


   


   

		
    }

}
