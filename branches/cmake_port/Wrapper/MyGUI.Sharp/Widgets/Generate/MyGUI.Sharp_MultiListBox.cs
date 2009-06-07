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

        public MultiListBox()
            : base()
        {
        }

        internal MultiListBox(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal MultiListBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MultiList"; }

        internal static BaseWidget RequestWrapMultiListBox(BaseWidget _parent, IntPtr _widget)
        {
            return new MultiListBox(_parent, _widget);
        }

        internal static BaseWidget RequestCreateMultiListBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new MultiListBox(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Request OperatorLess

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventOperatorLess == null) ExportMultiListBoxEvent_AdviseOperatorLess( mNative, true );
				mEventOperatorLess += value;
			}
			remove
			{
				mEventOperatorLess -= value;
				if (mEventOperatorLess == null) ExportMultiListBoxEvent_AdviseOperatorLess( mNative, false );
			}
		}


		private struct ExportEventOperatorLess
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMultiListBoxEvent_DelegateOperatorLess( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MultiListBox _sender ,
				  uint _column ,
				[MarshalAs(UnmanagedType.LPWStr)]  string _firstItem ,
				[MarshalAs(UnmanagedType.LPWStr)]  string _secondItem ,
				[MarshalAs(UnmanagedType.U1)] ref bool _less );
				
			private static ExportHandle mDelegate;
			public ExportEventOperatorLess( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMultiListBoxEvent_DelegateOperatorLess( mDelegate );
			}
		}
		static ExportEventOperatorLess mExportOperatorLess =
			new ExportEventOperatorLess(new ExportEventOperatorLess.ExportHandle( OnExportOperatorLess ));

		private static void OnExportOperatorLess(
			 MultiListBox _sender ,
			 uint _column ,
			 string _firstItem ,
			 string _secondItem ,
			ref bool _less )
		{
			if (_sender.mEventOperatorLess != null)
				_sender.mEventOperatorLess(
					 _sender ,
					 _column ,
					 _firstItem ,
					 _secondItem ,
					ref _less );
		}

		#endregion



   		#region Event ListChangePosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMultiListBoxEvent_DelegateListChangePosition( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MultiListBox _sender ,
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMultiListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MultiListBox _sender ,
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_ClearIndexSelected( IntPtr _native );

		public void ClearIndexSelected( )
		{
			ExportMultiListBox_ClearIndexSelected(  mNative );
		}

		#endregion



   		#region Property IndexSelected

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetIndexSelected( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportMultiListBox_GetIndexSelected( mNative )  ; }
			set { ExportMultiListBox_SetIndexSelected( mNative,  value ); }
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportMultiListBox_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportMultiListBox_GetItemCount( mNative )  ; }
		}

		#endregion



   		#region Method GetColumnDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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



   		#region Method SortByColumn

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportMultiListBox_GetColumnWidthAt_column( IntPtr _native ,
			  uint _column );

		public int GetColumnWidthAt(
			uint _column )
		{
			return  ExportMultiListBox_GetColumnWidthAt_column( mNative , 
				 _column )  ;
		}

		#endregion



   		#region Method GetColumnNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMultiListBox_GetColumnNameAt_column( IntPtr _native ,
			  uint _column );

		public string GetColumnNameAt(
			uint _column )
		{
			return  Marshal.PtrToStringUni(  ExportMultiListBox_GetColumnNameAt_column( mNative , 
				 _column )  )  ;
		}

		#endregion



   		#region Method SetColumnWidthAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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



   		#region Method SetColumnNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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



   		#region Method RemoveAllColumns

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListBox_RemoveAllColumns( IntPtr _native );

		public void RemoveAllColumns( )
		{
			ExportMultiListBox_RemoveAllColumns(  mNative );
		}

		#endregion



   		#region Method RemoveColumnAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMultiListBox_GetColumnCount( IntPtr _native );

		public uint ColumnCount
		{
			get { return  ExportMultiListBox_GetColumnCount( mNative )  ; }
		}

		#endregion



   


   

		
    }

}
