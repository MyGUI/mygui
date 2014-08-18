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
    public  class TabControl :
		Widget
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
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event TabChangeSelect

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControlEvent_AdviseTabChangeSelect(IntPtr _native, bool _advise);

		public delegate void HandleTabChangeSelect(
			TabControl _sender,
			uint _index);
			
		private HandleTabChangeSelect mEventTabChangeSelect;
		public event HandleTabChangeSelect EventTabChangeSelect
		{
			add
			{
				if (ExportEventTabChangeSelect.mDelegate == null)
				{
					ExportEventTabChangeSelect.mDelegate = new ExportEventTabChangeSelect.ExportHandle(OnExportTabChangeSelect);
					ExportEventTabChangeSelect.ExportTabControlEvent_DelegateTabChangeSelect(ExportEventTabChangeSelect.mDelegate);
				}

				if (mEventTabChangeSelect == null)
					ExportTabControlEvent_AdviseTabChangeSelect(Native, true);
				mEventTabChangeSelect += value;
			}
			remove
			{
				mEventTabChangeSelect -= value;
				if (mEventTabChangeSelect == null)
					ExportTabControlEvent_AdviseTabChangeSelect(Native, false);
			}
		}

		private struct ExportEventTabChangeSelect
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportTabControlEvent_DelegateTabChangeSelect(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportTabChangeSelect(
			IntPtr _sender,
			uint _index)
		{
			TabControl sender = (TabControl)BaseWidget.GetByNative(_sender);

			if (sender.mEventTabChangeSelect != null)
				sender.mEventTabChangeSelect(
					sender ,
					_index);
		}

		#endregion
		#region Method GetButtonWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonWidth__item(IntPtr _native,
			IntPtr _item);

		public int GetButtonWidth(
			TabItem _item)
		{
			return ExportTabControl_GetButtonWidth__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetButtonWidthAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonWidthAt__index(IntPtr _native,
			uint _index);

		public int GetButtonWidthAt(
			uint _index)
		{
			return ExportTabControl_GetButtonWidthAt__index(Native,
				_index);
		}

		#endregion
		#region Method SetButtonWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonWidth__item__width(IntPtr _native,
			IntPtr _item,
			int _width);

		public void SetButtonWidth(
			TabItem _item,
			int _width)
		{
			ExportTabControl_SetButtonWidth__item__width(Native,
				_item.Native,
				_width);
		}

		#endregion
		#region Method SetButtonWidthAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonWidthAt__index__width(IntPtr _native,
			uint _index,
			int _width);

		public void SetButtonWidthAt(
			uint _index,
			int _width)
		{
			ExportTabControl_SetButtonWidthAt__index__width(Native,
				_index,
				_width);
		}

		#endregion
		#region Method BeginToItemSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemSelected(IntPtr _native);

		public void BeginToItemSelected( )
		{
			ExportTabControl_BeginToItemSelected(Native);
		}

		#endregion
		#region Method BeginToItemLast

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemLast(IntPtr _native);

		public void BeginToItemLast( )
		{
			ExportTabControl_BeginToItemLast(Native);
		}

		#endregion
		#region Method BeginToItemFirst

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemFirst(IntPtr _native);

		public void BeginToItemFirst( )
		{
			ExportTabControl_BeginToItemFirst(Native);
		}

		#endregion
		#region Method BeginToItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItem__item(IntPtr _native,
			IntPtr _item);

		public void BeginToItem(
			TabItem _item)
		{
			ExportTabControl_BeginToItem__item(Native,
				_item.Native);
		}

		#endregion
		#region Method BeginToItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_BeginToItemAt__index(IntPtr _native,
			uint _index);

		public void BeginToItemAt(
			uint _index)
		{
			ExportTabControl_BeginToItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method GetItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemName__item(IntPtr _native,
			IntPtr _item);

		public string GetItemName(
			TabItem _item)
		{
			return Marshal.PtrToStringUni(ExportTabControl_GetItemName__item(Native,
				_item.Native));
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemNameAt__index(IntPtr _native,
			uint _index);

		public string GetItemNameAt(
			uint _index)
		{
			return Marshal.PtrToStringUni(ExportTabControl_GetItemNameAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemName__item__name(IntPtr _native,
			IntPtr _item,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemName(
			TabItem _item,
			string _name)
		{
			ExportTabControl_SetItemName__item__name(Native,
				_item.Native,
				_name);
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemNameAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemNameAt(
			uint _index,
			string _name)
		{
			ExportTabControl_SetItemNameAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Method SwapItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SwapItems__index1__index2(IntPtr _native,
			uint _index1,
			uint _index2);

		public void SwapItems(
			uint _index1,
			uint _index2)
		{
			ExportTabControl_SwapItems__index1__index2(Native,
				_index1,
				_index2);
		}

		#endregion
		#region Method FindItemWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_FindItemWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public TabItem FindItemWith(
			string _name)
		{
			return (TabItem)BaseWidget.GetByNative(ExportTabControl_FindItemWith__name(Native,
				_name));
		}

		#endregion
		#region Method FindItemIndexWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_FindItemIndexWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public uint FindItemIndexWith(
			string _name)
		{
			return ExportTabControl_FindItemIndexWith__name(Native,
				_name);
		}

		#endregion
		#region Method FindItemIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_FindItemIndex__item(IntPtr _native,
			IntPtr _item);

		public uint FindItemIndex(
			TabItem _item)
		{
			return ExportTabControl_FindItemIndex__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetItemIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetItemIndex__item(IntPtr _native,
			IntPtr _item);

		public uint GetItemIndex(
			TabItem _item)
		{
			return ExportTabControl_GetItemIndex__item(Native,
				_item.Native);
		}

		#endregion
		#region Method GetItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemAt__index(IntPtr _native,
			uint _index);

		public TabItem GetItemAt(
			uint _index)
		{
			return (TabItem)BaseWidget.GetByNative(ExportTabControl_GetItemAt__index(Native,
				_index));
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveAllItems(IntPtr _native);

		public void RemoveAllItems( )
		{
			ExportTabControl_RemoveAllItems(Native);
		}

		#endregion
		#region Method RemoveItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveItem__item(IntPtr _native,
			IntPtr _item);

		public void RemoveItem(
			TabItem _item)
		{
			ExportTabControl_RemoveItem__item(Native,
				_item.Native);
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_RemoveItemAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemAt(
			uint _index)
		{
			ExportTabControl_RemoveItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_AddItem__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public TabItem AddItem(
			string _name)
		{
			return (TabItem)BaseWidget.GetByNative(ExportTabControl_AddItem__name(Native,
				_name));
		}

		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_InsertItemAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public TabItem InsertItemAt(
			uint _index,
			string _name)
		{
			return (TabItem)BaseWidget.GetByNative(ExportTabControl_InsertItemAt__index__name(Native,
				_index,
				_name));
		}

		#endregion
		#region Property SmoothShow

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportTabControl_GetSmoothShow(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetSmoothShow(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool SmoothShow
		{
			get { return ExportTabControl_GetSmoothShow(Native); }
			set { ExportTabControl_SetSmoothShow(Native, value); }
		}

		#endregion
		#region Property ButtonAutoWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportTabControl_GetButtonAutoWidth(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonAutoWidth(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool ButtonAutoWidth
		{
			get { return ExportTabControl_GetButtonAutoWidth(Native); }
			set { ExportTabControl_SetButtonAutoWidth(Native, value); }
		}

		#endregion
		#region Property ButtonDefaultWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabControl_GetButtonDefaultWidth(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetButtonDefaultWidth(IntPtr _widget, int _value);

		public int ButtonDefaultWidth
		{
			get { return ExportTabControl_GetButtonDefaultWidth(Native); }
			set { ExportTabControl_SetButtonDefaultWidth(Native, value); }
		}

		#endregion
		#region Property ItemSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTabControl_GetItemSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetItemSelected(IntPtr _widget, TabItem _value);

		public TabItem ItemSelected
		{
			get { return (TabItem)BaseWidget.GetByNative(ExportTabControl_GetItemSelected(Native)); }
			set { ExportTabControl_SetItemSelected(Native, value); }
		}

		#endregion
		#region Property IndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetIndexSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabControl_SetIndexSelected(IntPtr _widget, uint _value);

		public uint IndexSelected
		{
			get { return ExportTabControl_GetIndexSelected(Native); }
			set { ExportTabControl_SetIndexSelected(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportTabControl_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportTabControl_GetItemCount(Native); }
		}

		#endregion
		
    }
}
