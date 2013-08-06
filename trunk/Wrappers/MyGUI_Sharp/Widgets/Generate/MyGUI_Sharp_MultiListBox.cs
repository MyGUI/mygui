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
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Request OperatorLess

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBoxEvent_AdviseOperatorLess( IntPtr _native, bool _advise );

		public delegate void HandleOperatorLess(
			 MultiListBox _sender ,
			 uint _column ,
			 string _firstItem ,
			 string _secondItem ,
			ref bool _less );
			
		private HandleOperatorLess mEventOperatorLess;
		public event HandleOperatorLess RequestOperatorLess
		{
			add
			{
				if (ExportEventOperatorLess.mDelegate == null)
				{
					ExportEventOperatorLess.mDelegate = new ExportEventOperatorLess.ExportHandle( OnExportOperatorLess );
					ExportEventOperatorLess.ExportMultiListBoxEvent_DelegateOperatorLess( ExportEventOperatorLess.mDelegate );
				}

				if (mEventOperatorLess == null)
					ExportMultiListBoxEvent_AdviseOperatorLess( Native, true );
				mEventOperatorLess += value;
			}
			remove
			{
				mEventOperatorLess -= value;
				if (mEventOperatorLess == null)
					ExportMultiListBoxEvent_AdviseOperatorLess( Native, false );
			}
		}

		private struct ExportEventOperatorLess
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportMultiListBoxEvent_DelegateOperatorLess( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender ,
				  uint _column ,
				[MarshalAs(UnmanagedType.LPWStr)]  string _firstItem ,
				[MarshalAs(UnmanagedType.LPWStr)]  string _secondItem ,
				[MarshalAs(UnmanagedType.U1)] ref bool _less );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportOperatorLess(
			IntPtr _sender ,
			 uint _column ,
			 string _firstItem ,
			 string _secondItem ,
			ref bool _less )
		{
			 MultiListBox sender = (  MultiListBox )BaseWidget.GetByNative( _sender );

			if (sender.mEventOperatorLess != null)
				sender.mEventOperatorLess(
					sender ,
					 _column ,
					 _firstItem ,
					 _secondItem ,
					ref _less );
		}

		#endregion
		#region Event ListChangePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBoxEvent_AdviseListChangePosition( IntPtr _native, bool _advise );

		public delegate void HandleListChangePosition(
			 MultiListBox _sender ,
			 uint _index );
			
		private HandleListChangePosition mEventListChangePosition;
		public event HandleListChangePosition EventListChangePosition
		{
			add
			{
				if (ExportEventListChangePosition.mDelegate == null)
				{
					ExportEventListChangePosition.mDelegate = new ExportEventListChangePosition.ExportHandle( OnExportListChangePosition );
					ExportEventListChangePosition.ExportMultiListBoxEvent_DelegateListChangePosition( ExportEventListChangePosition.mDelegate );
				}

				if (mEventListChangePosition == null)
					ExportMultiListBoxEvent_AdviseListChangePosition( Native, true );
				mEventListChangePosition += value;
			}
			remove
			{
				mEventListChangePosition -= value;
				if (mEventListChangePosition == null)
					ExportMultiListBoxEvent_AdviseListChangePosition( Native, false );
			}
		}

		private struct ExportEventListChangePosition
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportMultiListBoxEvent_DelegateListChangePosition( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListChangePosition(
			IntPtr _sender ,
			 uint _index )
		{
			 MultiListBox sender = (  MultiListBox )BaseWidget.GetByNative( _sender );

			if (sender.mEventListChangePosition != null)
				sender.mEventListChangePosition(
					sender ,
					 _index );
		}

		#endregion
		#region Event ListSelectAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBoxEvent_AdviseListSelectAccept( IntPtr _native, bool _advise );

		public delegate void HandleListSelectAccept(
			 MultiListBox _sender ,
			 uint _index );
			
		private HandleListSelectAccept mEventListSelectAccept;
		public event HandleListSelectAccept EventListSelectAccept
		{
			add
			{
				if (ExportEventListSelectAccept.mDelegate == null)
				{
					ExportEventListSelectAccept.mDelegate = new ExportEventListSelectAccept.ExportHandle( OnExportListSelectAccept );
					ExportEventListSelectAccept.ExportMultiListBoxEvent_DelegateListSelectAccept( ExportEventListSelectAccept.mDelegate );
				}

				if (mEventListSelectAccept == null)
					ExportMultiListBoxEvent_AdviseListSelectAccept( Native, true );
				mEventListSelectAccept += value;
			}
			remove
			{
				mEventListSelectAccept -= value;
				if (mEventListSelectAccept == null)
					ExportMultiListBoxEvent_AdviseListSelectAccept( Native, false );
			}
		}

		private struct ExportEventListSelectAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportMultiListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender ,
				  uint _index );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListSelectAccept(
			IntPtr _sender ,
			 uint _index )
		{
			 MultiListBox sender = (  MultiListBox )BaseWidget.GetByNative( _sender );

			if (sender.mEventListSelectAccept != null)
				sender.mEventListSelectAccept(
					sender ,
					 _index );
		}

		#endregion
		#region Method FindSubItemWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_FindSubItemWith_column_name( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindSubItemWith(
			uint _column ,
			string _name )
		{
			return  ExportMultiListBox_FindSubItemWith_column_name( Native , 
				 _column ,
				 _name )  ;
		}

		#endregion
		#region Method GetSubItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetSubItemNameAt_column_index( IntPtr _native ,
			  uint _column ,
			  uint _index );

		public string GetSubItemNameAt(
			uint _column ,
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetSubItemNameAt_column_index( Native , 
				 _column ,
				 _index )  )  ;
		}

		#endregion
		#region Method SetSubItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetSubItemNameAt_column_index_name( IntPtr _native ,
			  uint _column ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetSubItemNameAt(
			uint _column ,
			uint _index ,
			string _name )
		{
			ExportMultiListBox_SetSubItemNameAt_column_index_name( Native , 
				 _column ,
				 _index ,
				 _name );
		}

		#endregion
		#region Method ClearIndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearIndexSelected( IntPtr _native );

		public void ClearIndexSelected( )
		{
			ExportMultiListBox_ClearIndexSelected( Native );
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetItemNameAt_index( Native , 
				 _index )  )  ;
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportMultiListBox_SetItemNameAt_index_name( Native , 
				 _index ,
				 _name );
		}

		#endregion
		#region Method SwapItemsAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SwapItemsAt_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapItemsAt(
			uint _index1 ,
			uint _index2 )
		{
			ExportMultiListBox_SwapItemsAt_index1_index2( Native , 
				 _index1 ,
				 _index2 );
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportMultiListBox_RemoveAllItems( Native );
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportMultiListBox_RemoveItemAt_index( Native , 
				 _index );
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void AddItem(
			string _name )
		{
			ExportMultiListBox_AddItem_name( Native , 
				 _name );
		}

		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void InsertItemAt(
			uint _index ,
			string _name )
		{
			ExportMultiListBox_InsertItemAt_index_name( Native , 
				 _index ,
				 _name );
		}

		#endregion
		#region Method SetColumnResizingPolicyAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnResizingPolicyAt_index_value( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.I4)]  ResizingPolicy _value );

		public void SetColumnResizingPolicyAt(
			uint _index ,
			ResizingPolicy _value )
		{
			ExportMultiListBox_SetColumnResizingPolicyAt_index_value( Native , 
				 _index ,
				 _value );
		}

		#endregion
		#region Method SetColumnResizingPolicy

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnResizingPolicy_item_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.I4)]  ResizingPolicy _value );

		public void SetColumnResizingPolicy(
			MultiListItem _item ,
			ResizingPolicy _value )
		{
			ExportMultiListBox_SetColumnResizingPolicy_item_value( Native , 
				 _item ,
				 _value );
		}

		#endregion
		#region Method GetColumnIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetColumnIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint GetColumnIndex(
			MultiListItem _item )
		{
			return  ExportMultiListBox_GetColumnIndex_item( Native , 
				 _item )  ;
		}

		#endregion
		#region Method SortByColumn

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SortByColumn_column_backward( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.U1)]  bool _backward );

		public void SortByColumn(
			uint _column ,
			bool _backward )
		{
			ExportMultiListBox_SortByColumn_column_backward( Native , 
				 _column ,
				 _backward );
		}

		#endregion
		#region Method GetColumnWidthAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportMultiListBox_GetColumnWidthAt_column( IntPtr _native ,
			  uint _column );

		public int GetColumnWidthAt(
			uint _column )
		{
			return  ExportMultiListBox_GetColumnWidthAt_column( Native , 
				 _column )  ;
		}

		#endregion
		#region Method GetColumnName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetColumnName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public string GetColumnName(
			MultiListItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetColumnName_item( Native , 
				 _item )  )  ;
		}

		#endregion
		#region Method GetColumnNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetColumnNameAt_column( IntPtr _native ,
			  uint _column );

		public string GetColumnNameAt(
			uint _column )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetColumnNameAt_column( Native , 
				 _column )  )  ;
		}

		#endregion
		#region Method SetColumnWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnWidth_item_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			  int _width );

		public void SetColumnWidth(
			MultiListItem _item ,
			int _width )
		{
			ExportMultiListBox_SetColumnWidth_item_width( Native , 
				 _item ,
				 _width );
		}

		#endregion
		#region Method SetColumnWidthAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnWidthAt_column_width( IntPtr _native ,
			  uint _column ,
			  int _width );

		public void SetColumnWidthAt(
			uint _column ,
			int _width )
		{
			ExportMultiListBox_SetColumnWidthAt_column_width( Native , 
				 _column ,
				 _width );
		}

		#endregion
		#region Method SetColumnName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetColumnName(
			MultiListItem _item ,
			string _name )
		{
			ExportMultiListBox_SetColumnName_item_name( Native , 
				 _item ,
				 _name );
		}

		#endregion
		#region Method SetColumnNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetColumnNameAt_column_name( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetColumnNameAt(
			uint _column ,
			string _name )
		{
			ExportMultiListBox_SetColumnNameAt_column_name( Native , 
				 _column ,
				 _name );
		}

		#endregion
		#region Method SwapColumnsAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SwapColumnsAt_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapColumnsAt(
			uint _index1 ,
			uint _index2 )
		{
			ExportMultiListBox_SwapColumnsAt_index1_index2( Native , 
				 _index1 ,
				 _index2 );
		}

		#endregion
		#region Method RemoveAllColumns

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllColumns( IntPtr _native );

		public void RemoveAllColumns( )
		{
			ExportMultiListBox_RemoveAllColumns( Native );
		}

		#endregion
		#region Method RemoveColumnAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveColumnAt_column( IntPtr _native ,
			  uint _column );

		public void RemoveColumnAt(
			uint _column )
		{
			ExportMultiListBox_RemoveColumnAt_column( Native , 
				 _column );
		}

		#endregion
		#region Method AddColumn

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_AddColumn_name_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width );

		public void AddColumn(
			string _name ,
			int _width )
		{
			ExportMultiListBox_AddColumn_name_width( Native , 
				 _name ,
				 _width );
		}

		#endregion
		#region Method InsertColumnAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_InsertColumnAt_column_name_width( IntPtr _native ,
			  uint _column ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			  int _width );

		public void InsertColumnAt(
			uint _column ,
			string _name ,
			int _width )
		{
			ExportMultiListBox_InsertColumnAt_column_name_width( Native , 
				 _column ,
				 _name ,
				 _width );
		}

		#endregion
		#region Property IndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetIndexSelected( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportMultiListBox_GetIndexSelected( Native )  ; }
			set { ExportMultiListBox_SetIndexSelected( Native,  value ); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportMultiListBox_GetItemCount( Native )  ; }
		}

		#endregion
		#region Property ColumnCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetColumnCount( IntPtr _native );

		public uint ColumnCount
		{
			get { return  ExportMultiListBox_GetColumnCount( Native )  ; }
		}

		#endregion
		
    }

}
