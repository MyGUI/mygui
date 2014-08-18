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
    public  class ListBox :
		Widget
    {
        #region ListBox

        protected override string GetWidgetType() { return "ListBox"; }

        internal static BaseWidget RequestWrapListBox(BaseWidget _parent, IntPtr _widget)
        {
			ListBox widget = new ListBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateListBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ListBox widget = new ListBox();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event NotifyItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseNotifyItem(IntPtr _native, bool _advise);

		public delegate void HandleNotifyItem(
			ListBox _sender,
			ref IBNotifyItemData _info);
			
		private HandleNotifyItem mEventNotifyItem;
		public event HandleNotifyItem EventNotifyItem
		{
			add
			{
				if (ExportEventNotifyItem.mDelegate == null)
				{
					ExportEventNotifyItem.mDelegate = new ExportEventNotifyItem.ExportHandle(OnExportNotifyItem);
					ExportEventNotifyItem.ExportListBoxEvent_DelegateNotifyItem(ExportEventNotifyItem.mDelegate);
				}

				if (mEventNotifyItem == null)
					ExportListBoxEvent_AdviseNotifyItem(Native, true);
				mEventNotifyItem += value;
			}
			remove
			{
				mEventNotifyItem -= value;
				if (mEventNotifyItem == null)
					ExportListBoxEvent_AdviseNotifyItem(Native, false);
			}
		}

		private struct ExportEventNotifyItem
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateNotifyItem(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[In] ref IBNotifyItemData _info);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportNotifyItem(
			IntPtr _sender,
			ref IBNotifyItemData _info)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventNotifyItem != null)
				sender.mEventNotifyItem(
					sender ,
					ref _info);
		}

		#endregion
		#region Event ListChangeScroll

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListChangeScroll(IntPtr _native, bool _advise);

		public delegate void HandleListChangeScroll(
			ListBox _sender,
			uint _position);
			
		private HandleListChangeScroll mEventListChangeScroll;
		public event HandleListChangeScroll EventListChangeScroll
		{
			add
			{
				if (ExportEventListChangeScroll.mDelegate == null)
				{
					ExportEventListChangeScroll.mDelegate = new ExportEventListChangeScroll.ExportHandle(OnExportListChangeScroll);
					ExportEventListChangeScroll.ExportListBoxEvent_DelegateListChangeScroll(ExportEventListChangeScroll.mDelegate);
				}

				if (mEventListChangeScroll == null)
					ExportListBoxEvent_AdviseListChangeScroll(Native, true);
				mEventListChangeScroll += value;
			}
			remove
			{
				mEventListChangeScroll -= value;
				if (mEventListChangeScroll == null)
					ExportListBoxEvent_AdviseListChangeScroll(Native, false);
			}
		}

		private struct ExportEventListChangeScroll
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListChangeScroll(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _position);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListChangeScroll(
			IntPtr _sender,
			uint _position)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventListChangeScroll != null)
				sender.mEventListChangeScroll(
					sender ,
					_position);
		}

		#endregion
		#region Event ListMouseItemFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListMouseItemFocus(IntPtr _native, bool _advise);

		public delegate void HandleListMouseItemFocus(
			ListBox _sender,
			uint _index);
			
		private HandleListMouseItemFocus mEventListMouseItemFocus;
		public event HandleListMouseItemFocus EventListMouseItemFocus
		{
			add
			{
				if (ExportEventListMouseItemFocus.mDelegate == null)
				{
					ExportEventListMouseItemFocus.mDelegate = new ExportEventListMouseItemFocus.ExportHandle(OnExportListMouseItemFocus);
					ExportEventListMouseItemFocus.ExportListBoxEvent_DelegateListMouseItemFocus(ExportEventListMouseItemFocus.mDelegate);
				}

				if (mEventListMouseItemFocus == null)
					ExportListBoxEvent_AdviseListMouseItemFocus(Native, true);
				mEventListMouseItemFocus += value;
			}
			remove
			{
				mEventListMouseItemFocus -= value;
				if (mEventListMouseItemFocus == null)
					ExportListBoxEvent_AdviseListMouseItemFocus(Native, false);
			}
		}

		private struct ExportEventListMouseItemFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListMouseItemFocus(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListMouseItemFocus(
			IntPtr _sender,
			uint _index)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventListMouseItemFocus != null)
				sender.mEventListMouseItemFocus(
					sender ,
					_index);
		}

		#endregion
		#region Event ListMouseItemActivate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListMouseItemActivate(IntPtr _native, bool _advise);

		public delegate void HandleListMouseItemActivate(
			ListBox _sender,
			uint _index);
			
		private HandleListMouseItemActivate mEventListMouseItemActivate;
		public event HandleListMouseItemActivate EventListMouseItemActivate
		{
			add
			{
				if (ExportEventListMouseItemActivate.mDelegate == null)
				{
					ExportEventListMouseItemActivate.mDelegate = new ExportEventListMouseItemActivate.ExportHandle(OnExportListMouseItemActivate);
					ExportEventListMouseItemActivate.ExportListBoxEvent_DelegateListMouseItemActivate(ExportEventListMouseItemActivate.mDelegate);
				}

				if (mEventListMouseItemActivate == null)
					ExportListBoxEvent_AdviseListMouseItemActivate(Native, true);
				mEventListMouseItemActivate += value;
			}
			remove
			{
				mEventListMouseItemActivate -= value;
				if (mEventListMouseItemActivate == null)
					ExportListBoxEvent_AdviseListMouseItemActivate(Native, false);
			}
		}

		private struct ExportEventListMouseItemActivate
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListMouseItemActivate(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListMouseItemActivate(
			IntPtr _sender,
			uint _index)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventListMouseItemActivate != null)
				sender.mEventListMouseItemActivate(
					sender ,
					_index);
		}

		#endregion
		#region Event ListChangePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListChangePosition(IntPtr _native, bool _advise);

		public delegate void HandleListChangePosition(
			ListBox _sender,
			uint _index);
			
		private HandleListChangePosition mEventListChangePosition;
		public event HandleListChangePosition EventListChangePosition
		{
			add
			{
				if (ExportEventListChangePosition.mDelegate == null)
				{
					ExportEventListChangePosition.mDelegate = new ExportEventListChangePosition.ExportHandle(OnExportListChangePosition);
					ExportEventListChangePosition.ExportListBoxEvent_DelegateListChangePosition(ExportEventListChangePosition.mDelegate);
				}

				if (mEventListChangePosition == null)
					ExportListBoxEvent_AdviseListChangePosition(Native, true);
				mEventListChangePosition += value;
			}
			remove
			{
				mEventListChangePosition -= value;
				if (mEventListChangePosition == null)
					ExportListBoxEvent_AdviseListChangePosition(Native, false);
			}
		}

		private struct ExportEventListChangePosition
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListChangePosition(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListChangePosition(
			IntPtr _sender,
			uint _index)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventListChangePosition != null)
				sender.mEventListChangePosition(
					sender ,
					_index);
		}

		#endregion
		#region Event ListSelectAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBoxEvent_AdviseListSelectAccept(IntPtr _native, bool _advise);

		public delegate void HandleListSelectAccept(
			ListBox _sender,
			uint _index);
			
		private HandleListSelectAccept mEventListSelectAccept;
		public event HandleListSelectAccept EventListSelectAccept
		{
			add
			{
				if (ExportEventListSelectAccept.mDelegate == null)
				{
					ExportEventListSelectAccept.mDelegate = new ExportEventListSelectAccept.ExportHandle(OnExportListSelectAccept);
					ExportEventListSelectAccept.ExportListBoxEvent_DelegateListSelectAccept(ExportEventListSelectAccept.mDelegate);
				}

				if (mEventListSelectAccept == null)
					ExportListBoxEvent_AdviseListSelectAccept(Native, true);
				mEventListSelectAccept += value;
			}
			remove
			{
				mEventListSelectAccept -= value;
				if (mEventListSelectAccept == null)
					ExportListBoxEvent_AdviseListSelectAccept(Native, false);
			}
		}

		private struct ExportEventListSelectAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportListBoxEvent_DelegateListSelectAccept(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportListSelectAccept(
			IntPtr _sender,
			uint _index)
		{
			ListBox sender = (ListBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventListSelectAccept != null)
				sender.mEventListSelectAccept(
					sender ,
					_index);
		}

		#endregion
		#region Method GetWidgetByIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportListBox_GetWidgetByIndex__index(IntPtr _native,
			uint _index);

		public Widget GetWidgetByIndex(
			uint _index)
		{
			return (Widget)BaseWidget.GetByNative(ExportListBox_GetWidgetByIndex__index(Native,
				_index));
		}

		#endregion
		#region Method GetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportListBox_GetItemNameAt__index(IntPtr _native,
			uint _index);

		public string GetItemNameAt(
			uint _index)
		{
			return Marshal.PtrToStringUni(ExportListBox_GetItemNameAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetItemNameAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetItemNameAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void SetItemNameAt(
			uint _index,
			string _name)
		{
			ExportListBox_SetItemNameAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Method SetScrollPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetScrollPosition__position(IntPtr _native,
			uint _position);

		public void SetScrollPosition(
			uint _position)
		{
			ExportListBox_SetScrollPosition__position(Native,
				_position);
		}

		#endregion
		#region Method SetScrollVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetScrollVisible__visible(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _visible);

		public void SetScrollVisible(
			bool _visible)
		{
			ExportListBox_SetScrollVisible__visible(Native,
				_visible);
		}

		#endregion
		#region Method IsItemSelectedVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportListBox_IsItemSelectedVisible__fill(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _fill);

		public bool IsItemSelectedVisible(
			bool _fill)
		{
			return ExportListBox_IsItemSelectedVisible__fill(Native,
				_fill);
		}

		#endregion
		#region Method IsItemVisibleAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportListBox_IsItemVisibleAt__index__fill(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.U1)] bool _fill);

		public bool IsItemVisibleAt(
			uint _index,
			bool _fill)
		{
			return ExportListBox_IsItemVisibleAt__index__fill(Native,
				_index,
				_fill);
		}

		#endregion
		#region Method BeginToItemSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemSelected(IntPtr _native);

		public void BeginToItemSelected( )
		{
			ExportListBox_BeginToItemSelected(Native);
		}

		#endregion
		#region Method BeginToItemLast

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemLast(IntPtr _native);

		public void BeginToItemLast( )
		{
			ExportListBox_BeginToItemLast(Native);
		}

		#endregion
		#region Method BeginToItemFirst

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemFirst(IntPtr _native);

		public void BeginToItemFirst( )
		{
			ExportListBox_BeginToItemFirst(Native);
		}

		#endregion
		#region Method BeginToItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_BeginToItemAt__index(IntPtr _native,
			uint _index);

		public void BeginToItemAt(
			uint _index)
		{
			ExportListBox_BeginToItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method ClearIndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_ClearIndexSelected(IntPtr _native);

		public void ClearIndexSelected( )
		{
			ExportListBox_ClearIndexSelected(Native);
		}

		#endregion
		#region Method FindItemIndexWith

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_FindItemIndexWith__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public uint FindItemIndexWith(
			string _name)
		{
			return ExportListBox_FindItemIndexWith__name(Native,
				_name);
		}

		#endregion
		#region Method SwapItemsAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SwapItemsAt__index1__index2(IntPtr _native,
			uint _index1,
			uint _index2);

		public void SwapItemsAt(
			uint _index1,
			uint _index2)
		{
			ExportListBox_SwapItemsAt__index1__index2(Native,
				_index1,
				_index2);
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_RemoveAllItems(IntPtr _native);

		public void RemoveAllItems( )
		{
			ExportListBox_RemoveAllItems(Native);
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_RemoveItemAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemAt(
			uint _index)
		{
			ExportListBox_RemoveItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_AddItem__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void AddItem(
			string _name)
		{
			ExportListBox_AddItem__name(Native,
				_name);
		}

		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_InsertItemAt__index__name(IntPtr _native,
			uint _index,
			[MarshalAs(UnmanagedType.LPWStr)] string _name);

		public void InsertItemAt(
			uint _index,
			string _name)
		{
			ExportListBox_InsertItemAt__index__name(Native,
				_index,
				_name);
		}

		#endregion
		#region Property OptimalHeight

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportListBox_GetOptimalHeight(IntPtr _native);

		public int OptimalHeight
		{
			get { return ExportListBox_GetOptimalHeight(Native); }
		}

		#endregion
		#region Property IndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_GetIndexSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportListBox_SetIndexSelected(IntPtr _widget, uint _value);

		public uint IndexSelected
		{
			get { return ExportListBox_GetIndexSelected(Native); }
			set { ExportListBox_SetIndexSelected(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportListBox_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportListBox_GetItemCount(Native); }
		}

		#endregion
		
    }
}
