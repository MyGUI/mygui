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

    public  class TabControl : Widget
    {

        #region TabControl

        protected override string GetWidgetType() { return "TabControl"; }

        internal static BaseWidget RequestWrapTabControl(BaseWidget _parent, IntPtr _widget)
        {
			TabControl widget = new TabControl();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateTabControl(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			TabControl widget = new TabControl();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event TabChangeSelect

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControlEvent_AdviseTabChangeSelect( IntPtr _native, bool _advise );

		public delegate void HandleTabChangeSelect(
			 TabControl _sender ,
			 uint _index );
			
		private HandleTabChangeSelect mEventTabChangeSelect;
		public event HandleTabChangeSelect EventTabChangeSelect
		{
			add
			{
				if (mEventTabChangeSelect == null) ExportTabControlEvent_AdviseTabChangeSelect( mNative, true );
				mEventTabChangeSelect += value;
			}
			remove
			{
				mEventTabChangeSelect -= value;
				if (mEventTabChangeSelect == null) ExportTabControlEvent_AdviseTabChangeSelect( mNative, false );
			}
		}


		private struct ExportEventTabChangeSelect
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportTabControlEvent_DelegateTabChangeSelect( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  TabControl _sender ,
				  uint _index );
				
			private static ExportHandle mDelegate;
			public ExportEventTabChangeSelect( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportTabControlEvent_DelegateTabChangeSelect( mDelegate );
			}
		}
		static ExportEventTabChangeSelect mExportTabChangeSelect =
			new ExportEventTabChangeSelect(new ExportEventTabChangeSelect.ExportHandle( OnExportTabChangeSelect ));

		private static void OnExportTabChangeSelect(
			 TabControl _sender ,
			 uint _index )
		{
			if (_sender.mEventTabChangeSelect != null)
				_sender.mEventTabChangeSelect(
					 _sender ,
					 _index );
		}

		#endregion



   		#region Property SmoothShow

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportTabControl_GetSmoothShow( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetSmoothShow( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool SmoothShow
		{
			get { return  ExportTabControl_GetSmoothShow( mNative )  ; }
			set { ExportTabControl_SetSmoothShow( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonAutoWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportTabControl_GetButtonAutoWidth( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonAutoWidth( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ButtonAutoWidth
		{
			get { return  ExportTabControl_GetButtonAutoWidth( mNative )  ; }
			set { ExportTabControl_SetButtonAutoWidth( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonDefaultWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonDefaultWidth( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonDefaultWidth( IntPtr _widget,   int _value );

		public int ButtonDefaultWidth
		{
			get { return  ExportTabControl_GetButtonDefaultWidth( mNative )  ; }
			set { ExportTabControl_SetButtonDefaultWidth( mNative,  value ); }
		}

		#endregion



   		#region Method GetButtonWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonWidth_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public int GetButtonWidth(
			TabItem _item )
		{
			return  ExportTabControl_GetButtonWidth_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetButtonWidthAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonWidthAt_index( IntPtr _native ,
			  uint _index );

		public int GetButtonWidthAt(
			uint _index )
		{
			return  ExportTabControl_GetButtonWidthAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method SetButtonWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonWidth_item_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			  int _width );

		public void SetButtonWidth(
			TabItem _item ,
			int _width )
		{
			ExportTabControl_SetButtonWidth_item_width( mNative , 
				 _item ,
				 _width );
		}

		#endregion



   		#region Method SetButtonWidthAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonWidthAt_index_width( IntPtr _native ,
			  uint _index ,
			  int _width );

		public void SetButtonWidthAt(
			uint _index ,
			int _width )
		{
			ExportTabControl_SetButtonWidthAt_index_width( mNative , 
				 _index ,
				 _width );
		}

		#endregion



   		#region Method BeginToItemSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemSelected( IntPtr _native );

		public void BeginToItemSelected( )
		{
			ExportTabControl_BeginToItemSelected(  mNative );
		}

		#endregion



   		#region Method BeginToItemLast

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemLast( IntPtr _native );

		public void BeginToItemLast( )
		{
			ExportTabControl_BeginToItemLast(  mNative );
		}

		#endregion



   		#region Method BeginToItemFirst

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemFirst( IntPtr _native );

		public void BeginToItemFirst( )
		{
			ExportTabControl_BeginToItemFirst(  mNative );
		}

		#endregion



   		#region Method BeginToItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void BeginToItem(
			TabItem _item )
		{
			ExportTabControl_BeginToItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method BeginToItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemAt_index( IntPtr _native ,
			  uint _index );

		public void BeginToItemAt(
			uint _index )
		{
			ExportTabControl_BeginToItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method GetItemName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public string GetItemName(
			TabItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportTabControl_GetItemName_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportTabControl_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemName(
			TabItem _item ,
			string _name )
		{
			ExportTabControl_SetItemName_item_name( mNative , 
				 _item ,
				 _name );
		}

		#endregion



   		#region Method SetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportTabControl_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Method GetItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportTabControl_GetItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public object GetItemData(
			TabItem _item )
		{
			return ExportTabControl_GetItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method GetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportTabControl_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportTabControl_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_ClearItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void ClearItemData(
			TabItem _item )
		{
			ExportTabControl_ClearItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method ClearItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportTabControl_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemData_item_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemData(
			TabItem _item ,
			object _data )
		{
			ExportTabControl_SetItemData_item_data( mNative , 
				 _item ,
				 _data );
		}

		#endregion



   		#region Method SetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportTabControl_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Property ItemSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_GetItemSelected( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemSelected( IntPtr _widget, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  TabItem _value );

		public TabItem ItemSelected
		{
			get { return  ExportTabControl_GetItemSelected( mNative )  ; }
			set { ExportTabControl_SetItemSelected( mNative,  value ); }
		}

		#endregion



   		#region Property IndexSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetIndexSelected( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportTabControl_GetIndexSelected( mNative )  ; }
			set { ExportTabControl_SetIndexSelected( mNative,  value ); }
		}

		#endregion



   		#region Method SwapItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SwapItems_index1_index2( IntPtr _native ,
			  uint _index1 ,
			  uint _index2 );

		public void SwapItems(
			uint _index1 ,
			uint _index2 )
		{
			ExportTabControl_SwapItems_index1_index2( mNative , 
				 _index1 ,
				 _index2 );
		}

		#endregion



   		#region Method FindItemWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_FindItemWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem FindItemWith(
			string _name )
		{
			return  ExportTabControl_FindItemWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndexWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_FindItemIndexWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindItemIndexWith(
			string _name )
		{
			return  ExportTabControl_FindItemIndexWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_FindItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint FindItemIndex(
			TabItem _item )
		{
			return  ExportTabControl_FindItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint GetItemIndex(
			TabItem _item )
		{
			return  ExportTabControl_GetItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_GetItemAt_index( IntPtr _native ,
			  uint _index );

		public TabItem GetItemAt(
			uint _index )
		{
			return  ExportTabControl_GetItemAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveAllItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportTabControl_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void RemoveItem(
			TabItem _item )
		{
			ExportTabControl_RemoveItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportTabControl_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_AddItem_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem AddItem(
			string _name ,
			object _data )
		{
			return  ExportTabControl_AddItem_name_data( mNative , 
				 _name ,
				 _data )  ;
		}
		
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem AddItem(
			string _name )
		{
			return  ExportTabControl_AddItem_name( mNative , 
				 _name )  ;
		}
		
		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_InsertItem_to_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _to ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem InsertItem(
			TabItem _to ,
			string _name ,
			object _data )
		{
			return  ExportTabControl_InsertItem_to_name_data( mNative , 
				 _to ,
				 _name ,
				 _data )  ;
		}

		#endregion



   		#region Method InsertItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_InsertItemAt_index_name_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem InsertItemAt(
			uint _index ,
			string _name ,
			object _data )
		{
			return  ExportTabControl_InsertItemAt_index_name_data( mNative , 
				 _index ,
				 _name ,
				 _data )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern TabItem ExportTabControl_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem InsertItemAt(
			uint _index ,
			string _name )
		{
			return  ExportTabControl_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name )  ;
		}

		#endregion



   		#region Property ItemCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportTabControl_GetItemCount( mNative )  ; }
		}

		#endregion



   


   


   


   


   


   


   


   


   

		
    }

}
