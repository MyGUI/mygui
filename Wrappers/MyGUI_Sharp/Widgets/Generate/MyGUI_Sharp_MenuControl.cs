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
    public  class MenuControl :
		Widget
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
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event MenuCtrlClose

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControlEvent_AdviseMenuCtrlClose(IntPtr _native, bool _advise);

		public delegate void HandleMenuCtrlClose(
			MenuControl _sender);
			
		private HandleMenuCtrlClose mEventMenuCtrlClose;
		public event HandleMenuCtrlClose EventMenuCtrlClose
		{
			add
			{
				if (ExportEventMenuCtrlClose.mDelegate == null)
				{
					ExportEventMenuCtrlClose.mDelegate = new ExportEventMenuCtrlClose.ExportHandle(OnExportMenuCtrlClose);
					ExportEventMenuCtrlClose.ExportMenuControlEvent_DelegateMenuCtrlClose(ExportEventMenuCtrlClose.mDelegate);
				}

				if (mEventMenuCtrlClose == null)
					ExportMenuControlEvent_AdviseMenuCtrlClose(Native, true);
				mEventMenuCtrlClose += value;
			}
			remove
			{
				mEventMenuCtrlClose -= value;
				if (mEventMenuCtrlClose == null)
					ExportMenuControlEvent_AdviseMenuCtrlClose(Native, false);
			}
		}

		private struct ExportEventMenuCtrlClose
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportMenuControlEvent_DelegateMenuCtrlClose(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMenuCtrlClose(
			IntPtr _sender)
		{
			MenuControl sender = (MenuControl)BaseWidget.GetByNative(_sender);

			if (sender.mEventMenuCtrlClose != null)
				sender.mEventMenuCtrlClose(
					sender);
		}

		#endregion
		#region Event MenuCtrlAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControlEvent_AdviseMenuCtrlAccept(IntPtr _native, bool _advise);

		public delegate void HandleMenuCtrlAccept(
			MenuControl _sender,
			MenuItem _item);
			
		private HandleMenuCtrlAccept mEventMenuCtrlAccept;
		public event HandleMenuCtrlAccept EventMenuCtrlAccept
		{
			add
			{
				if (ExportEventMenuCtrlAccept.mDelegate == null)
				{
					ExportEventMenuCtrlAccept.mDelegate = new ExportEventMenuCtrlAccept.ExportHandle(OnExportMenuCtrlAccept);
					ExportEventMenuCtrlAccept.ExportMenuControlEvent_DelegateMenuCtrlAccept(ExportEventMenuCtrlAccept.mDelegate);
				}

				if (mEventMenuCtrlAccept == null)
					ExportMenuControlEvent_AdviseMenuCtrlAccept(Native, true);
				mEventMenuCtrlAccept += value;
			}
			remove
			{
				mEventMenuCtrlAccept -= value;
				if (mEventMenuCtrlAccept == null)
					ExportMenuControlEvent_AdviseMenuCtrlAccept(Native, false);
			}
		}

		private struct ExportEventMenuCtrlAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportMenuControlEvent_DelegateMenuCtrlAccept(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				IntPtr _item);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMenuCtrlAccept(
			IntPtr _sender,
			IntPtr _item)
		{
			MenuControl sender = (MenuControl)BaseWidget.GetByNative(_sender);

			if (sender.mEventMenuCtrlAccept != null)
				sender.mEventMenuCtrlAccept(
					sender ,
					(MenuItem)BaseWidget.GetByNative(_item));
		}

		#endregion
		#region Method SetItemType

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemType__item__type(IntPtr _native,
			IntPtr _item,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type);

		public void SetItemType(
			MenuItem _item,
			MenuItemType _type)
		{
			ExportMenuControl_SetItemType__item__type(Native,
				_item.Native,
				_type);
		}

		#endregion
		#region Method SetItemTypeAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemTypeAt__index__type(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type);

		public void SetItemTypeAt(
			uint _index,
			MenuItemType _type)
		{
			ExportMenuControl_SetItemTypeAt__index__type(Native,
				_index,
				_type);
		}

		#endregion
		#region Method GetItemType

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern MenuItemType ExportMenuControl_GetItemType__item(IntPtr _native,
			IntPtr _item);

		public MenuItemType GetItemType(
			MenuItem _item)
		{
			return ExportMenuControl_GetItemType__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetItemTypeAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern MenuItemType ExportMenuControl_GetItemTypeAt__index(IntPtr _native,
			uint _index);

		public MenuItemType GetItemTypeAt(
			uint _index)
		{
			return ExportMenuControl_GetItemTypeAt__index(Native,
				_index);
		}

		#endregion
		#region Method RemoveItemChild

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemChild__item(IntPtr _native,
			IntPtr _item);

		public void RemoveItemChild(
			MenuItem _item)
		{
			ExportMenuControl_RemoveItemChild__item(Native,
				_item.Native);
		}

		#endregion
		#region Method RemoveItemChildAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemChildAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemChildAt(
			uint _index)
		{
			ExportMenuControl_RemoveItemChildAt__index(Native,
				_index);
		}

		#endregion
		#region Method CreateItemChild

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_CreateItemChild__item(IntPtr _native,
			IntPtr _item);

		public MenuControl CreateItemChild(
			MenuItem _item)
		{
			return (MenuControl)BaseWidget.GetByNative(ExportMenuControl_CreateItemChild__item(Native,
				_item.Native));
		}

		#endregion
		#region Method CreateItemChildAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_CreateItemChildAt__index(IntPtr _native,
			uint _index);

		public MenuControl CreateItemChildAt(
			uint _index)
		{
			return (MenuControl)BaseWidget.GetByNative(ExportMenuControl_CreateItemChildAt__index(Native,
				_index));
		}

		#endregion
		#region Method GetItemChild

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemChild__item(IntPtr _native,
			IntPtr _item);

		public MenuControl GetItemChild(
			MenuItem _item)
		{
			return (MenuControl)BaseWidget.GetByNative(ExportMenuControl_GetItemChild__item(Native,
				_item.Native));
		}

		#endregion
		#region Method GetItemChildAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemChildAt__index(IntPtr _native,
			uint _index);

		public MenuControl GetItemChildAt(
			uint _index)
		{
			return (MenuControl)BaseWidget.GetByNative(ExportMenuControl_GetItemChildAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemChildVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemChildVisible__item__visible(IntPtr _native,
			IntPtr _item,
			[MarshalAs(UnmanagedType.U1)] bool _visible);

		public void SetItemChildVisible(
			MenuItem _item,
			bool _visible)
		{
			ExportMenuControl_SetItemChildVisible__item__visible(Native,
				_item.Native,
				_visible);
		}

		#endregion
		#region Method SetItemChildVisibleAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemChildVisibleAt__index__visible(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.U1)] bool _visible);

		public void SetItemChildVisibleAt(
			uint _index,
			bool _visible)
		{
			ExportMenuControl_SetItemChildVisibleAt__index__visible(Native,
				_index,
				_visible);
		}

		#endregion
		#region Method FindItemIndexWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_FindItemIndexWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public uint FindItemIndexWith(
			string _name)
		{
			return ExportMenuControl_FindItemIndexWith__name(Native,
				_name);
		}

		#endregion
		#region Method GetItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemName__item(IntPtr _native,
			IntPtr _item);

		public string GetItemName(
			MenuItem _item)
		{
			return Marshal.PtrToStringUni(ExportMenuControl_GetItemName__item(Native,
				_item.Native));
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemNameAt__index(IntPtr _native,
			uint _index);

		public string GetItemNameAt(
			uint _index)
		{
			return Marshal.PtrToStringUni(ExportMenuControl_GetItemNameAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemName__item__name(IntPtr _native,
			IntPtr _item,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemName(
			MenuItem _item,
			string _name)
		{
			ExportMenuControl_SetItemName__item__name(Native,
				_item.Native,
				_name);
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemNameAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemNameAt(
			uint _index,
			string _name)
		{
			ExportMenuControl_SetItemNameAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Method GetItemIndexById

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemIndexById__id(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public uint GetItemIndexById(
			string _id)
		{
			return ExportMenuControl_GetItemIndexById__id(Native,
				_id);
		}

		#endregion
		#region Method FindItemById

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_FindItemById__id__recursive(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _id,
			[MarshalAs(UnmanagedType.U1)] bool _recursive);

		public MenuItem FindItemById(
			string _id,
			bool _recursive)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_FindItemById__id__recursive(Native,
				_id,
				_recursive));
		}

		#endregion
		#region Method GetItemById

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemById__id(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public MenuItem GetItemById(
			string _id)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_GetItemById__id(Native,
				_id));
		}

		#endregion
		#region Method GetItemId

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemId__item(IntPtr _native,
			IntPtr _item);

		public string GetItemId(
			MenuItem _item)
		{
			return Marshal.PtrToStringAnsi(ExportMenuControl_GetItemId__item(Native,
				_item.Native));
		}

		#endregion
		#region Method GetItemIdAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemIdAt__index(IntPtr _native,
			uint _index);

		public string GetItemIdAt(
			uint _index)
		{
			return Marshal.PtrToStringAnsi(ExportMenuControl_GetItemIdAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemId

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemId__item__id(IntPtr _native,
			IntPtr _item,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public void SetItemId(
			MenuItem _item,
			string _id)
		{
			ExportMenuControl_SetItemId__item__id(Native,
				_item.Native,
				_id);
		}

		#endregion
		#region Method SetItemIdAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetItemIdAt__index__id(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public void SetItemIdAt(
			uint _index,
			string _id)
		{
			ExportMenuControl_SetItemIdAt__index__id(Native,
				_index,
				_id);
		}

		#endregion
		#region Method FindItemWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_FindItemWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public MenuItem FindItemWith(
			string _name)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_FindItemWith__name(Native,
				_name));
		}

		#endregion
		#region Method FindItemIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_FindItemIndex__item(IntPtr _native,
			IntPtr _item);

		public uint FindItemIndex(
			MenuItem _item)
		{
			return ExportMenuControl_FindItemIndex__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetItemIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemIndex__item(IntPtr _native,
			IntPtr _item);

		public uint GetItemIndex(
			MenuItem _item)
		{
			return ExportMenuControl_GetItemIndex__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetItemAt__index(IntPtr _native,
			uint _index);

		public MenuItem GetItemAt(
			uint _index)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_GetItemAt__index(Native,
				_index));
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveAllItems(IntPtr _native);

		public void RemoveAllItems( )
		{
			ExportMenuControl_RemoveAllItems(Native);
		}

		#endregion
		#region Method RemoveItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItem__item(IntPtr _native,
			IntPtr _item);

		public void RemoveItem(
			MenuItem _item)
		{
			ExportMenuControl_RemoveItem__item(Native,
				_item.Native);
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_RemoveItemAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemAt(
			uint _index)
		{
			ExportMenuControl_RemoveItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_AddItem__name__type__id(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public MenuItem AddItem(
			string _name,
			MenuItemType _type,
			string _id)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_AddItem__name__type__id(Native,
				_name,
				_type,
				_id));
		}

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_AddItem__name__type(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type);

		public MenuItem AddItem(
			string _name,
			MenuItemType _type)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_AddItem__name__type(Native,
				_name,
				_type));
		}
		
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_AddItem__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public MenuItem AddItem(
			string _name)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_AddItem__name(Native,
				_name));
		}
		
		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_InsertItemAt__index__name__type__id(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type,
			[MarshalAs(UnmanagedType.LPStr)] string _id);

		public MenuItem InsertItemAt(
			uint _index,
			string _name,
			MenuItemType _type,
			string _id)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_InsertItemAt__index__name__type__id(Native,
				_index,
				_name,
				_type,
				_id));
		}

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_InsertItemAt__index__name__type(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name,
			[MarshalAs(UnmanagedType.I4)] MenuItemType _type);

		public MenuItem InsertItemAt(
			uint _index,
			string _name,
			MenuItemType _type)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_InsertItemAt__index__name__type(Native,
				_index,
				_name,
				_type));
		}

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_InsertItemAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public MenuItem InsertItemAt(
			uint _index,
			string _name)
		{
			return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_InsertItemAt__index__name(Native,
				_index,
				_name));
		}

		#endregion
		#region Method SetVisibleSmooth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetVisibleSmooth__value(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _value);

		public void SetVisibleSmooth(
			bool _value)
		{
			ExportMenuControl_SetVisibleSmooth__value(Native,
				_value);
		}

		#endregion
		#region Property VerticalAlignment

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportMenuControl_GetVerticalAlignment(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetVerticalAlignment(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool VerticalAlignment
		{
			get { return ExportMenuControl_GetVerticalAlignment(Native); }
			set { ExportMenuControl_SetVerticalAlignment(Native, value); }
		}

		#endregion
		#region Property MenuItemParent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuControl_GetMenuItemParent(IntPtr _native);

		public MenuItem MenuItemParent
		{
			get { return (MenuItem)BaseWidget.GetByNative(ExportMenuControl_GetMenuItemParent(Native)); }
		}

		#endregion
		#region Property PopupAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportMenuControl_GetPopupAccept(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuControl_SetPopupAccept(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool PopupAccept
		{
			get { return ExportMenuControl_GetPopupAccept(Native); }
			set { ExportMenuControl_SetPopupAccept(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuControl_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportMenuControl_GetItemCount(Native); }
		}

		#endregion
		
    }
}
