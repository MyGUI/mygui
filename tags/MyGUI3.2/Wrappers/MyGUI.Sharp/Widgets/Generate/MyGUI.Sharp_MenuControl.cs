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

    public  class MenuControl : Widget
    {

        #region MenuControl

        protected override string GetWidgetType() { return "MenuControl"; }

        internal static BaseWidget RequestWrapMenuControl(BaseWidget _parent, IntPtr _widget)
        {
			MenuControl widget = new MenuControl();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMenuControl(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MenuControl widget = new MenuControl();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event MenuCtrlClose

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControlEvent_AdviseMenuCtrlClose( IntPtr _native, bool _advise );

		public delegate void HandleMenuCtrlClose(
			 MenuControl _sender );
			
		private HandleMenuCtrlClose mEventMenuCtrlClose;
		public event HandleMenuCtrlClose EventMenuCtrlClose
		{
			add
			{
				if (mEventMenuCtrlClose == null) ExportMenuControlEvent_AdviseMenuCtrlClose( mNative, true );
				mEventMenuCtrlClose += value;
			}
			remove
			{
				mEventMenuCtrlClose -= value;
				if (mEventMenuCtrlClose == null) ExportMenuControlEvent_AdviseMenuCtrlClose( mNative, false );
			}
		}


		private struct ExportEventMenuCtrlClose
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMenuControlEvent_DelegateMenuCtrlClose( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  MenuControl _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventMenuCtrlClose( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMenuControlEvent_DelegateMenuCtrlClose( mDelegate );
			}
		}
		static ExportEventMenuCtrlClose mExportMenuCtrlClose =
			new ExportEventMenuCtrlClose(new ExportEventMenuCtrlClose.ExportHandle( OnExportMenuCtrlClose ));

		private static void OnExportMenuCtrlClose(
			 MenuControl _sender )
		{
			if (_sender.mEventMenuCtrlClose != null)
				_sender.mEventMenuCtrlClose(
					 _sender );
		}

		#endregion



   		#region Event MenuCtrlAccept

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControlEvent_AdviseMenuCtrlAccept( IntPtr _native, bool _advise );

		public delegate void HandleMenuCtrlAccept(
			 MenuControl _sender ,
			 MenuItem _item );
			
		private HandleMenuCtrlAccept mEventMenuCtrlAccept;
		public event HandleMenuCtrlAccept EventMenuCtrlAccept
		{
			add
			{
				if (mEventMenuCtrlAccept == null) ExportMenuControlEvent_AdviseMenuCtrlAccept( mNative, true );
				mEventMenuCtrlAccept += value;
			}
			remove
			{
				mEventMenuCtrlAccept -= value;
				if (mEventMenuCtrlAccept == null) ExportMenuControlEvent_AdviseMenuCtrlAccept( mNative, false );
			}
		}


		private struct ExportEventMenuCtrlAccept
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMenuControlEvent_DelegateMenuCtrlAccept( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  MenuControl _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  MenuItem _item );
				
			private static ExportHandle mDelegate;
			public ExportEventMenuCtrlAccept( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMenuControlEvent_DelegateMenuCtrlAccept( mDelegate );
			}
		}
		static ExportEventMenuCtrlAccept mExportMenuCtrlAccept =
			new ExportEventMenuCtrlAccept(new ExportEventMenuCtrlAccept.ExportHandle( OnExportMenuCtrlAccept ));

		private static void OnExportMenuCtrlAccept(
			 MenuControl _sender ,
			 MenuItem _item )
		{
			if (_sender.mEventMenuCtrlAccept != null)
				_sender.mEventMenuCtrlAccept(
					 _sender ,
					 _item );
		}

		#endregion



   		#region Property VerticalAlignment

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportMenuControl_GetVerticalAlignment( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetVerticalAlignment( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VerticalAlignment
		{
			get { return  ExportMenuControl_GetVerticalAlignment( mNative )  ; }
			set { ExportMenuControl_SetVerticalAlignment( mNative,  value ); }
		}

		#endregion



   		#region Method GetMenuItemParent

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_GetMenuItemParent( IntPtr _native );

		public MenuItem GetMenuItemParent( )
		{
			return  ExportMenuControl_GetMenuItemParent( mNative )  ;
		}

		#endregion



   		#region Property PopupAccept

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportMenuControl_GetPopupAccept( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetPopupAccept( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool PopupAccept
		{
			get { return  ExportMenuControl_GetPopupAccept( mNative )  ; }
			set { ExportMenuControl_SetPopupAccept( mNative,  value ); }
		}

		#endregion



   		#region Method SetItemType

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemType_item_type( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public void SetItemType(
			MenuItem _item ,
			MenuItemType _type )
		{
			ExportMenuControl_SetItemType_item_type( mNative , 
				 _item ,
				 _type );
		}

		#endregion



   		#region Method SetItemTypeAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemTypeAt_index_type( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public void SetItemTypeAt(
			uint _index ,
			MenuItemType _type )
		{
			ExportMenuControl_SetItemTypeAt_index_type( mNative , 
				 _index ,
				 _type );
		}

		#endregion



   		#region Method GetItemType

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MenuItemType ExportMenuControl_GetItemType_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public MenuItemType GetItemType(
			MenuItem _item )
		{
			return  ExportMenuControl_GetItemType_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemTypeAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MenuItemType ExportMenuControl_GetItemTypeAt_index( IntPtr _native ,
			  uint _index );

		public MenuItemType GetItemTypeAt(
			uint _index )
		{
			return  ExportMenuControl_GetItemTypeAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveItemChild

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void RemoveItemChild(
			MenuItem _item )
		{
			ExportMenuControl_RemoveItemChild_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemChildAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemChildAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemChildAt(
			uint _index )
		{
			ExportMenuControl_RemoveItemChildAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method CreateItemChild

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuControl ExportMenuControl_CreateItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public MenuControl CreateItemChild(
			MenuItem _item )
		{
			return  ExportMenuControl_CreateItemChild_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method CreateItemChildAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuControl ExportMenuControl_CreateItemChildAt_index( IntPtr _native ,
			  uint _index );

		public MenuControl CreateItemChildAt(
			uint _index )
		{
			return  ExportMenuControl_CreateItemChildAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method GetItemChild

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuControl ExportMenuControl_GetItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public MenuControl GetItemChild(
			MenuItem _item )
		{
			return  ExportMenuControl_GetItemChild_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemChildAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuControl ExportMenuControl_GetItemChildAt_index( IntPtr _native ,
			  uint _index );

		public MenuControl GetItemChildAt(
			uint _index )
		{
			return  ExportMenuControl_GetItemChildAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   


   


   		#region Method SetItemChildVisible

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemChildVisible_item_visible( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetItemChildVisible(
			MenuItem _item ,
			bool _visible )
		{
			ExportMenuControl_SetItemChildVisible_item_visible( mNative , 
				 _item ,
				 _visible );
		}

		#endregion



   		#region Method SetItemChildVisibleAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemChildVisibleAt_index_visible( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetItemChildVisibleAt(
			uint _index ,
			bool _visible )
		{
			ExportMenuControl_SetItemChildVisibleAt_index_visible( mNative , 
				 _index ,
				 _visible );
		}

		#endregion



   		#region Method FindItemIndexWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_FindItemIndexWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindItemIndexWith(
			string _name )
		{
			return  ExportMenuControl_FindItemIndexWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method GetItemName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public string GetItemName(
			MenuItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportMenuControl_GetItemName_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMenuControl_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemName(
			MenuItem _item ,
			string _name )
		{
			ExportMenuControl_SetItemName_item_name( mNative , 
				 _item ,
				 _name );
		}

		#endregion



   		#region Method SetItemNameAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportMenuControl_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Method GetItemIndexById

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemIndexById_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public uint GetItemIndexById(
			string _id )
		{
			return  ExportMenuControl_GetItemIndexById_id( mNative , 
				 _id )  ;
		}

		#endregion



   		#region Method FindItemById

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_FindItemById_id_recursive( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id ,
			[MarshalAs(UnmanagedType.U1)]  bool _recursive );

		public MenuItem FindItemById(
			string _id ,
			bool _recursive )
		{
			return  ExportMenuControl_FindItemById_id_recursive( mNative , 
				 _id ,
				 _recursive )  ;
		}

		#endregion



   		#region Method GetItemById

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_GetItemById_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public MenuItem GetItemById(
			string _id )
		{
			return  ExportMenuControl_GetItemById_id( mNative , 
				 _id )  ;
		}

		#endregion



   		#region Method GetItemId

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemId_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public string GetItemId(
			MenuItem _item )
		{
			return  Marshal.PtrToStringAnsi(  ExportMenuControl_GetItemId_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemIdAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemIdAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemIdAt(
			uint _index )
		{
			return  Marshal.PtrToStringAnsi(  ExportMenuControl_GetItemIdAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemId

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemId_item_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public void SetItemId(
			MenuItem _item ,
			string _id )
		{
			ExportMenuControl_SetItemId_item_id( mNative , 
				 _item ,
				 _id );
		}

		#endregion



   		#region Method SetItemIdAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemIdAt_index_id( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public void SetItemIdAt(
			uint _index ,
			string _id )
		{
			ExportMenuControl_SetItemIdAt_index_id( mNative , 
				 _index ,
				 _id );
		}

		#endregion



   		#region Method GetItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMenuControl_GetItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public object GetItemData(
			MenuItem _item )
		{
			return ExportMenuControl_GetItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method GetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMenuControl_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportMenuControl_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_ClearItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void ClearItemData(
			MenuItem _item )
		{
			ExportMenuControl_ClearItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method ClearItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportMenuControl_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItemData

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemData_item_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemData(
			MenuItem _item ,
			object _data )
		{
			ExportMenuControl_SetItemData_item_data( mNative , 
				 _item ,
				 _data );
		}

		#endregion



   		#region Method SetItemDataAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportMenuControl_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Method FindItemWith

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_FindItemWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem FindItemWith(
			string _name )
		{
			return  ExportMenuControl_FindItemWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_FindItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint FindItemIndex(
			MenuItem _item )
		{
			return  ExportMenuControl_FindItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public uint GetItemIndex(
			MenuItem _item )
		{
			return  ExportMenuControl_GetItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_GetItemAt_index( IntPtr _native ,
			  uint _index );

		public MenuItem GetItemAt(
			uint _index )
		{
			return  ExportMenuControl_GetItemAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveAllItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportMenuControl_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _item );

		public void RemoveItem(
			MenuItem _item )
		{
			ExportMenuControl_RemoveItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportMenuControl_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_AddItem_name_type_id_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public MenuItem AddItem(
			string _name ,
			MenuItemType _type ,
			string _id ,
			object _data )
		{
			return  ExportMenuControl_AddItem_name_type_id_data( mNative , 
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_AddItem_name_type_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public MenuItem AddItem(
			string _name ,
			MenuItemType _type ,
			string _id )
		{
			return  ExportMenuControl_AddItem_name_type_id( mNative , 
				 _name ,
				 _type ,
				 _id )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_AddItem_name_type( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public MenuItem AddItem(
			string _name ,
			MenuItemType _type )
		{
			return  ExportMenuControl_AddItem_name_type( mNative , 
				 _name ,
				 _type )  ;
		}
		
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem AddItem(
			string _name )
		{
			return  ExportMenuControl_AddItem_name( mNative , 
				 _name )  ;
		}
		
		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_InsertItem_to_name_type_id_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _to ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public MenuItem InsertItem(
			MenuItem _to ,
			string _name ,
			MenuItemType _type ,
			string _id ,
			object _data )
		{
			return  ExportMenuControl_InsertItem_to_name_type_id_data( mNative , 
				 _to ,
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		#endregion



   		#region Method InsertItemAt

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_InsertItemAt_index_name_type_id_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name ,
			MenuItemType _type ,
			string _id ,
			object _data )
		{
			return  ExportMenuControl_InsertItemAt_index_name_type_id_data( mNative , 
				 _index ,
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_InsertItemAt_index_name_type_id( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name ,
			MenuItemType _type ,
			string _id )
		{
			return  ExportMenuControl_InsertItemAt_index_name_type_id( mNative , 
				 _index ,
				 _name ,
				 _type ,
				 _id )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_InsertItemAt_index_name_type( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name ,
			MenuItemType _type )
		{
			return  ExportMenuControl_InsertItemAt_index_name_type( mNative , 
				 _index ,
				 _name ,
				 _type )  ;
		}

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern MenuItem ExportMenuControl_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name )
		{
			return  ExportMenuControl_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name )  ;
		}

		#endregion



   		#region Property ItemCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportMenuControl_GetItemCount( mNative )  ; }
		}

		#endregion



   		#region Method SetVisibleSmooth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetVisibleSmooth_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetVisibleSmooth(
			bool _value )
		{
			ExportMenuControl_SetVisibleSmooth_value( mNative , 
				 _value );
		}

		#endregion



   


   


   


   

		
    }

}
