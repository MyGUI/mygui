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

    public  class MenuCtrl : Widget
    {

        #region MenuCtrl

        public MenuCtrl()
            : base()
        {
        }

        internal MenuCtrl(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal MenuCtrl(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MenuCtrl"; }

        internal static BaseWidget RequestWrapMenuCtrl(BaseWidget _parent, IntPtr _widget)
        {
            return new MenuCtrl(_parent, _widget);
        }

        internal static BaseWidget RequestCreateMenuCtrl(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new MenuCtrl(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event MenuCtrlClose

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrlEvent_AdviseMenuCtrlClose( IntPtr _native, bool _advise );

		public delegate void HandleMenuCtrlClose(
			 MenuCtrl _sender );
			
		private HandleMenuCtrlClose mEventMenuCtrlClose;
		public event HandleMenuCtrlClose EventMenuCtrlClose
		{
			add
			{
				if (mEventMenuCtrlClose == null) ExportMenuCtrlEvent_AdviseMenuCtrlClose( mNative, true );
				mEventMenuCtrlClose += value;
			}
			remove
			{
				mEventMenuCtrlClose -= value;
				if (mEventMenuCtrlClose == null) ExportMenuCtrlEvent_AdviseMenuCtrlClose( mNative, false );
			}
		}


		private struct ExportEventMenuCtrlClose
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMenuCtrlEvent_DelegateMenuCtrlClose( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MenuCtrl _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventMenuCtrlClose( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMenuCtrlEvent_DelegateMenuCtrlClose( mDelegate );
			}
		}
		static ExportEventMenuCtrlClose mExportMenuCtrlClose =
			new ExportEventMenuCtrlClose(new ExportEventMenuCtrlClose.ExportHandle( OnExportMenuCtrlClose ));

		private static void OnExportMenuCtrlClose(
			 MenuCtrl _sender )
		{
			if (_sender.mEventMenuCtrlClose != null)
				_sender.mEventMenuCtrlClose(
					 _sender );
		}

		#endregion



   		#region Event MenuCtrlAccept

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrlEvent_AdviseMenuCtrlAccept( IntPtr _native, bool _advise );

		public delegate void HandleMenuCtrlAccept(
			 MenuCtrl _sender ,
			 MenuItem _item );
			
		private HandleMenuCtrlAccept mEventMenuCtrlAccept;
		public event HandleMenuCtrlAccept EventMenuCtrlAccept
		{
			add
			{
				if (mEventMenuCtrlAccept == null) ExportMenuCtrlEvent_AdviseMenuCtrlAccept( mNative, true );
				mEventMenuCtrlAccept += value;
			}
			remove
			{
				mEventMenuCtrlAccept -= value;
				if (mEventMenuCtrlAccept == null) ExportMenuCtrlEvent_AdviseMenuCtrlAccept( mNative, false );
			}
		}


		private struct ExportEventMenuCtrlAccept
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMenuCtrlEvent_DelegateMenuCtrlAccept( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MenuCtrl _sender ,
				[MarshalAs(UnmanagedType.Interface)]  MenuItem _item );
				
			private static ExportHandle mDelegate;
			public ExportEventMenuCtrlAccept( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMenuCtrlEvent_DelegateMenuCtrlAccept( mDelegate );
			}
		}
		static ExportEventMenuCtrlAccept mExportMenuCtrlAccept =
			new ExportEventMenuCtrlAccept(new ExportEventMenuCtrlAccept.ExportHandle( OnExportMenuCtrlAccept ));

		private static void OnExportMenuCtrlAccept(
			 MenuCtrl _sender ,
			 MenuItem _item )
		{
			if (_sender.mEventMenuCtrlAccept != null)
				_sender.mEventMenuCtrlAccept(
					 _sender ,
					 _item );
		}

		#endregion



   		#region Method GetMenuItemParent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_GetMenuItemParent( IntPtr _native );

		public MenuItem GetMenuItemParent( )
		{
			return  ExportMenuCtrl_GetMenuItemParent( mNative )  ;
		}

		#endregion



   		#region Property PopupAccept

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportMenuCtrl_GetPopupAccept( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetPopupAccept( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool PopupAccept
		{
			get { return  ExportMenuCtrl_GetPopupAccept( mNative )  ; }
			set { ExportMenuCtrl_SetPopupAccept( mNative,  value ); }
		}

		#endregion



   		#region Method SetItemType

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemType_item_type( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public void SetItemType(
			MenuItem _item ,
			MenuItemType _type )
		{
			ExportMenuCtrl_SetItemType_item_type( mNative , 
				 _item ,
				 _type );
		}

		#endregion



   		#region Method SetItemTypeAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemTypeAt_index_type( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public void SetItemTypeAt(
			uint _index ,
			MenuItemType _type )
		{
			ExportMenuCtrl_SetItemTypeAt_index_type( mNative , 
				 _index ,
				 _type );
		}

		#endregion



   		#region Method GetItemType

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MenuItemType ExportMenuCtrl_GetItemType_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public MenuItemType GetItemType(
			MenuItem _item )
		{
			return  ExportMenuCtrl_GetItemType_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemTypeAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MenuItemType ExportMenuCtrl_GetItemTypeAt_index( IntPtr _native ,
			  uint _index );

		public MenuItemType GetItemTypeAt(
			uint _index )
		{
			return  ExportMenuCtrl_GetItemTypeAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveItemChild

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_RemoveItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void RemoveItemChild(
			MenuItem _item )
		{
			ExportMenuCtrl_RemoveItemChild_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemChildAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_RemoveItemChildAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemChildAt(
			uint _index )
		{
			ExportMenuCtrl_RemoveItemChildAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method CreateItemChild

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuCtrl_CreateItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public MenuCtrl CreateItemChild(
			MenuItem _item )
		{
			return  ExportMenuCtrl_CreateItemChild_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method CreateItemChildAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuCtrl_CreateItemChildAt_index( IntPtr _native ,
			  uint _index );

		public MenuCtrl CreateItemChildAt(
			uint _index )
		{
			return  ExportMenuCtrl_CreateItemChildAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method GetItemChild

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuCtrl_GetItemChild_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public MenuCtrl GetItemChild(
			MenuItem _item )
		{
			return  ExportMenuCtrl_GetItemChild_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemChildAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuCtrl_GetItemChildAt_index( IntPtr _native ,
			  uint _index );

		public MenuCtrl GetItemChildAt(
			uint _index )
		{
			return  ExportMenuCtrl_GetItemChildAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   


   


   		#region Method SetItemChildVisible

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemChildVisible_item_visible( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetItemChildVisible(
			MenuItem _item ,
			bool _visible )
		{
			ExportMenuCtrl_SetItemChildVisible_item_visible( mNative , 
				 _item ,
				 _visible );
		}

		#endregion



   		#region Method SetItemChildVisibleAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemChildVisibleAt_index_visible( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetItemChildVisibleAt(
			uint _index ,
			bool _visible )
		{
			ExportMenuCtrl_SetItemChildVisibleAt_index_visible( mNative , 
				 _index ,
				 _visible );
		}

		#endregion



   		#region Method FindItemIndexWith

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuCtrl_FindItemIndexWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public uint FindItemIndexWith(
			string _name )
		{
			return  ExportMenuCtrl_FindItemIndexWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method GetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuCtrl_GetItemName_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public string GetItemName(
			MenuItem _item )
		{
			return  Marshal.PtrToStringUni(  ExportMenuCtrl_GetItemName_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuCtrl_GetItemNameAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemNameAt(
			uint _index )
		{
			return  Marshal.PtrToStringUni(  ExportMenuCtrl_GetItemNameAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemName_item_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemName(
			MenuItem _item ,
			string _name )
		{
			ExportMenuCtrl_SetItemName_item_name( mNative , 
				 _item ,
				 _name );
		}

		#endregion



   		#region Method SetItemNameAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemNameAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public void SetItemNameAt(
			uint _index ,
			string _name )
		{
			ExportMenuCtrl_SetItemNameAt_index_name( mNative , 
				 _index ,
				 _name );
		}

		#endregion



   		#region Method GetItemIndexById

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuCtrl_GetItemIndexById_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public uint GetItemIndexById(
			string _id )
		{
			return  ExportMenuCtrl_GetItemIndexById_id( mNative , 
				 _id )  ;
		}

		#endregion



   		#region Method GetItemById

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_GetItemById_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public MenuItem GetItemById(
			string _id )
		{
			return  ExportMenuCtrl_GetItemById_id( mNative , 
				 _id )  ;
		}

		#endregion



   		#region Method GetItemId

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuCtrl_GetItemId_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public string GetItemId(
			MenuItem _item )
		{
			return  Marshal.PtrToStringAnsi(  ExportMenuCtrl_GetItemId_item( mNative , 
				 _item )  )  ;
		}

		#endregion



   		#region Method GetItemIdAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuCtrl_GetItemIdAt_index( IntPtr _native ,
			  uint _index );

		public string GetItemIdAt(
			uint _index )
		{
			return  Marshal.PtrToStringAnsi(  ExportMenuCtrl_GetItemIdAt_index( mNative , 
				 _index )  )  ;
		}

		#endregion



   		#region Method SetItemId

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemId_item_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public void SetItemId(
			MenuItem _item ,
			string _id )
		{
			ExportMenuCtrl_SetItemId_item_id( mNative , 
				 _item ,
				 _id );
		}

		#endregion



   		#region Method SetItemIdAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemIdAt_index_id( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public void SetItemIdAt(
			uint _index ,
			string _id )
		{
			ExportMenuCtrl_SetItemIdAt_index_id( mNative , 
				 _index ,
				 _id );
		}

		#endregion



   		#region Method GetItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMenuCtrl_GetItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public object GetItemData(
			MenuItem _item )
		{
			return ExportMenuCtrl_GetItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method GetItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMenuCtrl_GetItemDataAt_index( IntPtr _native ,
			  uint _index );

		public object GetItemDataAt(
			uint _index )
		{
			return ExportMenuCtrl_GetItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method ClearItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_ClearItemData_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void ClearItemData(
			MenuItem _item )
		{
			ExportMenuCtrl_ClearItemData_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method ClearItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_ClearItemDataAt_index( IntPtr _native ,
			  uint _index );

		public void ClearItemDataAt(
			uint _index )
		{
			ExportMenuCtrl_ClearItemDataAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemData_item_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemData(
			MenuItem _item ,
			object _data )
		{
			ExportMenuCtrl_SetItemData_item_data( mNative , 
				 _item ,
				 _data );
		}

		#endregion



   		#region Method SetItemDataAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetItemDataAt_index_data( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.IUnknown)]  object _data );

		public void SetItemDataAt(
			uint _index ,
			object _data )
		{
			ExportMenuCtrl_SetItemDataAt_index_data( mNative , 
				 _index ,
				 _data );
		}

		#endregion



   		#region Method FindItemWith

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_FindItemWith_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem FindItemWith(
			string _name )
		{
			return  ExportMenuCtrl_FindItemWith_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method FindItemIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuCtrl_FindItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public uint FindItemIndex(
			MenuItem _item )
		{
			return  ExportMenuCtrl_FindItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuCtrl_GetItemIndex_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public uint GetItemIndex(
			MenuItem _item )
		{
			return  ExportMenuCtrl_GetItemIndex_item( mNative , 
				 _item )  ;
		}

		#endregion



   		#region Method GetItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_GetItemAt_index( IntPtr _native ,
			  uint _index );

		public MenuItem GetItemAt(
			uint _index )
		{
			return  ExportMenuCtrl_GetItemAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method RemoveAllItems

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_RemoveAllItems( IntPtr _native );

		public void RemoveAllItems( )
		{
			ExportMenuCtrl_RemoveAllItems(  mNative );
		}

		#endregion



   		#region Method RemoveItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_RemoveItem_item( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _item );

		public void RemoveItem(
			MenuItem _item )
		{
			ExportMenuCtrl_RemoveItem_item( mNative , 
				 _item );
		}

		#endregion



   		#region Method RemoveItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_RemoveItemAt_index( IntPtr _native ,
			  uint _index );

		public void RemoveItemAt(
			uint _index )
		{
			ExportMenuCtrl_RemoveItemAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_AddItem_name_type_id_data( IntPtr _native ,
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
			return  ExportMenuCtrl_AddItem_name_type_id_data( mNative , 
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_AddItem_name_type_id( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _id );

		public MenuItem AddItem(
			string _name ,
			MenuItemType _type ,
			string _id )
		{
			return  ExportMenuCtrl_AddItem_name_type_id( mNative , 
				 _name ,
				 _type ,
				 _id )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_AddItem_name_type( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public MenuItem AddItem(
			string _name ,
			MenuItemType _type )
		{
			return  ExportMenuCtrl_AddItem_name_type( mNative , 
				 _name ,
				 _type )  ;
		}
		
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_AddItem_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem AddItem(
			string _name )
		{
			return  ExportMenuCtrl_AddItem_name( mNative , 
				 _name )  ;
		}
		
		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_InsertItem_to_name_type_id_data( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _to ,
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
			return  ExportMenuCtrl_InsertItem_to_name_type_id_data( mNative , 
				 _to ,
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		#endregion



   		#region Method InsertItemAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_InsertItemAt_index_name_type_id_data( IntPtr _native ,
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
			return  ExportMenuCtrl_InsertItemAt_index_name_type_id_data( mNative , 
				 _index ,
				 _name ,
				 _type ,
				 _id ,
				 _data )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_InsertItemAt_index_name_type_id( IntPtr _native ,
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
			return  ExportMenuCtrl_InsertItemAt_index_name_type_id( mNative , 
				 _index ,
				 _name ,
				 _type ,
				 _id )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_InsertItemAt_index_name_type( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name ,
			[MarshalAs(UnmanagedType.I4)]  MenuItemType _type );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name ,
			MenuItemType _type )
		{
			return  ExportMenuCtrl_InsertItemAt_index_name_type( mNative , 
				 _index ,
				 _name ,
				 _type )  ;
		}

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuItem ExportMenuCtrl_InsertItemAt_index_name( IntPtr _native ,
			  uint _index ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MenuItem InsertItemAt(
			uint _index ,
			string _name )
		{
			return  ExportMenuCtrl_InsertItemAt_index_name( mNative , 
				 _index ,
				 _name )  ;
		}

		#endregion



   		#region Property ItemCount

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuCtrl_GetItemCount( IntPtr _native );

		public uint ItemCount
		{
			get { return  ExportMenuCtrl_GetItemCount( mNative )  ; }
		}

		#endregion



   		#region Method SetVisibleSmooth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuCtrl_SetVisibleSmooth_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetVisibleSmooth(
			bool _value )
		{
			ExportMenuCtrl_SetVisibleSmooth_value( mNative , 
				 _value );
		}

		#endregion



   


   


   


   

		
    }

}
