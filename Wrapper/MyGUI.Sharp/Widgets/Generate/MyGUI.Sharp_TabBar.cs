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

    public  class TabBar : Widget
    {

        #region TabBar

        public TabBar()
            : base()
        {
        }

        internal TabBar(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal TabBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Tab"; }

        internal static BaseWidget RequestWrapTabBar(BaseWidget _parent, IntPtr _widget)
        {
            return new TabBar(_parent, _widget);
        }

        internal static BaseWidget RequestCreateTabBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new TabBar(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event TabChangeSelect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBarEvent_AdviseTabChangeSelect( IntPtr _native, bool _advise );

		public delegate void HandleTabChangeSelect(
			 TabBar _sender ,
			 uint _index );
			
		private HandleTabChangeSelect mEventTabChangeSelect;
		public event HandleTabChangeSelect EventTabChangeSelect
		{
			add
			{
				if (mEventTabChangeSelect == null) ExportTabBarEvent_AdviseTabChangeSelect( mNative, true );
				mEventTabChangeSelect += value;
			}
			remove
			{
				mEventTabChangeSelect -= value;
				if (mEventTabChangeSelect == null) ExportTabBarEvent_AdviseTabChangeSelect( mNative, false );
			}
		}


		private struct ExportEventTabChangeSelect
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportTabBarEvent_DelegateTabChangeSelect( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  TabBar _sender ,
				  uint _index );
				
			private static ExportHandle mDelegate;
			public ExportEventTabChangeSelect( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportTabBarEvent_DelegateTabChangeSelect( mDelegate );
			}
		}
		static ExportEventTabChangeSelect mExportTabChangeSelect =
			new ExportEventTabChangeSelect(new ExportEventTabChangeSelect.ExportHandle( OnExportTabChangeSelect ));

		private static void OnExportTabChangeSelect(
			 TabBar _sender ,
			 uint _index )
		{
			if (_sender.mEventTabChangeSelect != null)
				_sender.mEventTabChangeSelect(
					 _sender ,
					 _index );
		}

		#endregion



   


   		#region Property SmoothShow

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportTabBar_GetSmoothShow( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetSmoothShow( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool SmoothShow
		{
			get { return  ExportTabBar_GetSmoothShow( mNative )  ; }
			set { ExportTabBar_SetSmoothShow( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonAutoWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportTabBar_GetButtonAutoWidth( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetButtonAutoWidth( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ButtonAutoWidth
		{
			get { return  ExportTabBar_GetButtonAutoWidth( mNative )  ; }
			set { ExportTabBar_SetButtonAutoWidth( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonDefaultWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabBar_GetButtonDefaultWidth( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetButtonDefaultWidth( IntPtr _widget,   int _value );

		public int ButtonDefaultWidth
		{
			get { return  ExportTabBar_GetButtonDefaultWidth( mNative )  ; }
			set { ExportTabBar_SetButtonDefaultWidth( mNative,  value ); }
		}

		#endregion



   		#region Method GetButtonWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabBar_GetButtonWidth_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public int GetButtonWidth(
			TabItem _item )
		{
			return  ExportTabBar_GetButtonWidth_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetButtonWidthAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabBar_GetButtonWidthAt_index( IntPtr _native ,
			  uint _index );

		public int GetButtonWidthAt(
			uint _index )
		{
			return  ExportTabBar_GetButtonWidthAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method SetButtonWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetButtonWidth_item_width( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			  int _width );

		public void SetButtonWidth(
			TabItem _item ,
			int _width )
		{
			ExportTabBar_SetButtonWidth_item_width( mNative , 
				 _item ,
				 _width );
		}

		#endregion



   		#region Method SetButtonWidthAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetButtonWidthAt_index_width( IntPtr _native ,
			  uint _index ,
			  int _width );

		public void SetButtonWidthAt(
			uint _index ,
			int _width )
		{
			ExportTabBar_SetButtonWidthAt_index_width( mNative , 
				 _index ,
				 _width );
		}

		#endregion



   		#region Method BeginToItemSelected

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_BeginToItemSelected( IntPtr _native );

		public void BeginToItemSelected( )
		{
			ExportTabBar_BeginToItemSelected(  mNative );
		}

		#endregion



   		#region Method BeginToItemLast

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_BeginToItemLast( IntPtr _native );

		public void BeginToItemLast( )
		{
			ExportTabBar_BeginToItemLast(  mNative );
		}

		#endregion



   		#region Method BeginToItemFirst

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_BeginToItemFirst( IntPtr _native );

		public void BeginToItemFirst( )
		{
			ExportTabBar_BeginToItemFirst(  mNative );
		}

		#endregion



   		#region Method BeginToItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_BeginToItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void BeginToItem(
			TabItem _item )
		{
			ExportTabBar_BeginToItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method BeginToItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_BeginToItemAt_index( IntPtr _native ,
			  uint _index );

		public void BeginToItemAt(
			uint _index )
		{
			ExportTabBar_BeginToItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method GetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabBar_GetItemName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public string GetItemName(
			TabItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportTabBar_GetItemName_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabBar_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportTabBar_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetItemName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemName(
			TabItem _item ,
			string _name )
		{
			ExportTabBar_SetItemName_item_name( mNative , 
				 _item ,
				 _name );
		}

		#endregion



   		#region Method SetItemNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportTabBar_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Method GetItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportTabBar_GetItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public object GetItemData(
			TabItem _item )
		{
			return ExportTabBar_GetItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method GetItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportTabBar_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportTabBar_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_ClearItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void ClearItemData(
			TabItem _item )
		{
			ExportTabBar_ClearItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method ClearItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportTabBar_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetItemData_item_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemData(
			TabItem _item ,
			object _data )
		{
			ExportTabBar_SetItemData_item_data( mNative , 
				 _item ,
				 _data );
		}

		#endregion



   		#region Method SetItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportTabBar_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Property ItemSelected

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_GetItemSelected( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetItemSelected( IntPtr _widget, [MarshalAs(UnmanagedType.Interface)]  TabItem _value );

		public TabItem ItemSelected
		{
			get { return  ExportTabBar_GetItemSelected( mNative )  ; }
			set { ExportTabBar_SetItemSelected( mNative,  value ); }
		}

		#endregion



   		#region Property IndexSelected

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabBar_GetIndexSelected( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_SetIndexSelected( IntPtr _widget,   uint _value );

		public uint IndexSelected
		{
			get { return  ExportTabBar_GetIndexSelected( mNative )  ; }
			set { ExportTabBar_SetIndexSelected( mNative,  value ); }
		}

		#endregion



   		#region Method FindItemWith

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_FindItemWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem FindItemWith(
			string _name )
		{
			return  ExportTabBar_FindItemWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndexWith

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabBar_FindItemIndexWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindItemIndexWith(
			string _name )
		{
			return  ExportTabBar_FindItemIndexWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabBar_FindItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public uint FindItemIndex(
			TabItem _item )
		{
			return  ExportTabBar_FindItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabBar_GetItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public uint GetItemIndex(
			TabItem _item )
		{
			return  ExportTabBar_GetItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_GetItemAt_index( IntPtr _native ,
			  uint _index );

		public TabItem GetItemAt(
			uint _index )
		{
			return  ExportTabBar_GetItemAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveAllItems

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportTabBar_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_RemoveItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void RemoveItem(
			TabItem _item )
		{
			ExportTabBar_RemoveItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabBar_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportTabBar_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_AddItem_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem AddItem(
			string _name ,
			object _data )
		{
			return  ExportTabBar_AddItem_name_data( mNative , 
				 _name ,
				 _data )  ;
		}
		
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem AddItem(
			string _name )
		{
			return  ExportTabBar_AddItem_name( mNative , 
				 _name )  ;
		}
		
		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_InsertItem_to_name_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _to ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem InsertItem(
			TabItem _to ,
			string _name ,
			object _data )
		{
			return  ExportTabBar_InsertItem_to_name_data( mNative , 
				 _to ,
				 _name ,
				 _data )  ;
		}

		#endregion



   		#region Method InsertItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_InsertItemAt_index_name_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public TabItem InsertItemAt(
			uint _index ,
			string _name ,
			object _data )
		{
			return  ExportTabBar_InsertItemAt_index_name_data( mNative , 
				 _index ,
				 _name ,
				 _data )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern TabItem ExportTabBar_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public TabItem InsertItemAt(
			uint _index ,
			string _name )
		{
			return  ExportTabBar_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name )  ;
		}

		#endregion



   		#region Property ItemCount

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabBar_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportTabBar_GetItemCount( mNative )  ; }
		}

		#endregion



   


   


   


   


   


   


   


   


   

		
    }

}
