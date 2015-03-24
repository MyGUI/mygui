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
    public  class ItemBox :
		DDContainer
    {
        #region ItemBox

        protected override string GetWidgetType() { return "ItemBox"; }

        internal static BaseWidget RequestWrapItemBox(BaseWidget _parent, IntPtr _widget)
        {
			ItemBox widget = new ItemBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateItemBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ItemBox widget = new ItemBox();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event NotifyItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseNotifyItem(IntPtr _native, bool _advise);

		public delegate void HandleNotifyItem(
			ItemBox _sender,
			ref IBNotifyItemData _info);
			
		private HandleNotifyItem mEventNotifyItem;
		public event HandleNotifyItem EventNotifyItem
		{
			add
			{
				if (ExportEventNotifyItem.mDelegate == null)
				{
					ExportEventNotifyItem.mDelegate = new ExportEventNotifyItem.ExportHandle(OnExportNotifyItem);
					ExportEventNotifyItem.ExportItemBoxEvent_DelegateNotifyItem(ExportEventNotifyItem.mDelegate);
				}

				if (mEventNotifyItem == null)
					ExportItemBoxEvent_AdviseNotifyItem(Native, true);
				mEventNotifyItem += value;
			}
			remove
			{
				mEventNotifyItem -= value;
				if (mEventNotifyItem == null)
					ExportItemBoxEvent_AdviseNotifyItem(Native, false);
			}
		}

		private struct ExportEventNotifyItem
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateNotifyItem(ExportHandle _delegate);
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
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventNotifyItem != null)
				sender.mEventNotifyItem(
					sender ,
					ref _info);
		}

		#endregion
		#region Event MouseItemActivate

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseMouseItemActivate(IntPtr _native, bool _advise);

		public delegate void HandleMouseItemActivate(
			ItemBox _sender,
			uint _index);
			
		private HandleMouseItemActivate mEventMouseItemActivate;
		public event HandleMouseItemActivate EventMouseItemActivate
		{
			add
			{
				if (ExportEventMouseItemActivate.mDelegate == null)
				{
					ExportEventMouseItemActivate.mDelegate = new ExportEventMouseItemActivate.ExportHandle(OnExportMouseItemActivate);
					ExportEventMouseItemActivate.ExportItemBoxEvent_DelegateMouseItemActivate(ExportEventMouseItemActivate.mDelegate);
				}

				if (mEventMouseItemActivate == null)
					ExportItemBoxEvent_AdviseMouseItemActivate(Native, true);
				mEventMouseItemActivate += value;
			}
			remove
			{
				mEventMouseItemActivate -= value;
				if (mEventMouseItemActivate == null)
					ExportItemBoxEvent_AdviseMouseItemActivate(Native, false);
			}
		}

		private struct ExportEventMouseItemActivate
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateMouseItemActivate(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMouseItemActivate(
			IntPtr _sender,
			uint _index)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventMouseItemActivate != null)
				sender.mEventMouseItemActivate(
					sender ,
					_index);
		}

		#endregion
		#region Event ChangeItemPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseChangeItemPosition(IntPtr _native, bool _advise);

		public delegate void HandleChangeItemPosition(
			ItemBox _sender,
			uint _index);
			
		private HandleChangeItemPosition mEventChangeItemPosition;
		public event HandleChangeItemPosition EventChangeItemPosition
		{
			add
			{
				if (ExportEventChangeItemPosition.mDelegate == null)
				{
					ExportEventChangeItemPosition.mDelegate = new ExportEventChangeItemPosition.ExportHandle(OnExportChangeItemPosition);
					ExportEventChangeItemPosition.ExportItemBoxEvent_DelegateChangeItemPosition(ExportEventChangeItemPosition.mDelegate);
				}

				if (mEventChangeItemPosition == null)
					ExportItemBoxEvent_AdviseChangeItemPosition(Native, true);
				mEventChangeItemPosition += value;
			}
			remove
			{
				mEventChangeItemPosition -= value;
				if (mEventChangeItemPosition == null)
					ExportItemBoxEvent_AdviseChangeItemPosition(Native, false);
			}
		}

		private struct ExportEventChangeItemPosition
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateChangeItemPosition(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeItemPosition(
			IntPtr _sender,
			uint _index)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventChangeItemPosition != null)
				sender.mEventChangeItemPosition(
					sender ,
					_index);
		}

		#endregion
		#region Event SelectItemAccept

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseSelectItemAccept(IntPtr _native, bool _advise);

		public delegate void HandleSelectItemAccept(
			ItemBox _sender,
			uint _index);
			
		private HandleSelectItemAccept mEventSelectItemAccept;
		public event HandleSelectItemAccept EventSelectItemAccept
		{
			add
			{
				if (ExportEventSelectItemAccept.mDelegate == null)
				{
					ExportEventSelectItemAccept.mDelegate = new ExportEventSelectItemAccept.ExportHandle(OnExportSelectItemAccept);
					ExportEventSelectItemAccept.ExportItemBoxEvent_DelegateSelectItemAccept(ExportEventSelectItemAccept.mDelegate);
				}

				if (mEventSelectItemAccept == null)
					ExportItemBoxEvent_AdviseSelectItemAccept(Native, true);
				mEventSelectItemAccept += value;
			}
			remove
			{
				mEventSelectItemAccept -= value;
				if (mEventSelectItemAccept == null)
					ExportItemBoxEvent_AdviseSelectItemAccept(Native, false);
			}
		}

		private struct ExportEventSelectItemAccept
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateSelectItemAccept(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _index);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportSelectItemAccept(
			IntPtr _sender,
			uint _index)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventSelectItemAccept != null)
				sender.mEventSelectItemAccept(
					sender ,
					_index);
		}

		#endregion
		#region Request DrawItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseDrawItem(IntPtr _native, bool _advise);

		public delegate void HandleDrawItem(
			ItemBox _sender,
			Widget _item,
			ref IBDrawItemInfo _info);
			
		private HandleDrawItem mEventDrawItem;
		public event HandleDrawItem RequestDrawItem
		{
			add
			{
				if (ExportEventDrawItem.mDelegate == null)
				{
					ExportEventDrawItem.mDelegate = new ExportEventDrawItem.ExportHandle(OnExportDrawItem);
					ExportEventDrawItem.ExportItemBoxEvent_DelegateDrawItem(ExportEventDrawItem.mDelegate);
				}

				if (mEventDrawItem == null)
					ExportItemBoxEvent_AdviseDrawItem(Native, true);
				mEventDrawItem += value;
			}
			remove
			{
				mEventDrawItem -= value;
				if (mEventDrawItem == null)
					ExportItemBoxEvent_AdviseDrawItem(Native, false);
			}
		}

		private struct ExportEventDrawItem
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateDrawItem(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				IntPtr _item,
				[In] ref IBDrawItemInfo _info);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportDrawItem(
			IntPtr _sender,
			IntPtr _item,
			ref IBDrawItemInfo _info)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventDrawItem != null)
				sender.mEventDrawItem(
					sender ,
					(Widget)BaseWidget.GetByNative(_item),
					ref _info);
		}

		#endregion
		#region Request CoordItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseCoordItem(IntPtr _native, bool _advise);

		public delegate void HandleCoordItem(
			ItemBox _sender,
			ref IntCoord _coord,
			bool _drag);
			
		private HandleCoordItem mEventCoordItem;
		public event HandleCoordItem RequestCoordItem
		{
			add
			{
				if (ExportEventCoordItem.mDelegate == null)
				{
					ExportEventCoordItem.mDelegate = new ExportEventCoordItem.ExportHandle(OnExportCoordItem);
					ExportEventCoordItem.ExportItemBoxEvent_DelegateCoordItem(ExportEventCoordItem.mDelegate);
				}

				if (mEventCoordItem == null)
					ExportItemBoxEvent_AdviseCoordItem(Native, true);
				mEventCoordItem += value;
			}
			remove
			{
				mEventCoordItem -= value;
				if (mEventCoordItem == null)
					ExportItemBoxEvent_AdviseCoordItem(Native, false);
			}
		}

		private struct ExportEventCoordItem
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateCoordItem(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				ref IntCoord _coord,
				[MarshalAs(UnmanagedType.U1)] bool _drag);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportCoordItem(
			IntPtr _sender,
			ref IntCoord _coord,
			bool _drag)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventCoordItem != null)
				sender.mEventCoordItem(
					sender ,
					ref _coord,
					_drag);
		}

		#endregion
		#region Request CreateWidgetItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBoxEvent_AdviseCreateWidgetItem(IntPtr _native, bool _advise);

		public delegate void HandleCreateWidgetItem(
			ItemBox _sender,
			Widget _item);
			
		private HandleCreateWidgetItem mEventCreateWidgetItem;
		public event HandleCreateWidgetItem RequestCreateWidgetItem
		{
			add
			{
				if (ExportEventCreateWidgetItem.mDelegate == null)
				{
					ExportEventCreateWidgetItem.mDelegate = new ExportEventCreateWidgetItem.ExportHandle(OnExportCreateWidgetItem);
					ExportEventCreateWidgetItem.ExportItemBoxEvent_DelegateCreateWidgetItem(ExportEventCreateWidgetItem.mDelegate);
				}

				if (mEventCreateWidgetItem == null)
					ExportItemBoxEvent_AdviseCreateWidgetItem(Native, true);
				mEventCreateWidgetItem += value;
			}
			remove
			{
				mEventCreateWidgetItem -= value;
				if (mEventCreateWidgetItem == null)
					ExportItemBoxEvent_AdviseCreateWidgetItem(Native, false);
			}
		}

		private struct ExportEventCreateWidgetItem
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportItemBoxEvent_DelegateCreateWidgetItem(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				IntPtr _item);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportCreateWidgetItem(
			IntPtr _sender,
			IntPtr _item)
		{
			ItemBox sender = (ItemBox)BaseWidget.GetByNative(_sender);

			if (sender.mEventCreateWidgetItem != null)
				sender.mEventCreateWidgetItem(
					sender ,
					(Widget)BaseWidget.GetByNative(_item));
		}

		#endregion
		#region Method GetWidgetByIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportItemBox_GetWidgetByIndex__index(IntPtr _native,
			uint _index);

		public Widget GetWidgetByIndex(
			uint _index)
		{
			return (Widget)BaseWidget.GetByNative(ExportItemBox_GetWidgetByIndex__index(Native,
				_index));
		}

		#endregion
		#region Method GetIndexByWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportItemBox_GetIndexByWidget__widget(IntPtr _native,
			IntPtr _widget);

		public uint GetIndexByWidget(
			Widget _widget)
		{
			return ExportItemBox_GetIndexByWidget__widget(Native,
				_widget.Native);
		}

		#endregion
		#region Method ClearIndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_ClearIndexSelected(IntPtr _native);

		public void ClearIndexSelected( )
		{
			ExportItemBox_ClearIndexSelected(Native);
		}

		#endregion
		#region Method RedrawAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_RedrawAllItems(IntPtr _native);

		public void RedrawAllItems( )
		{
			ExportItemBox_RedrawAllItems(Native);
		}

		#endregion
		#region Method RedrawItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_RedrawItemAt__index(IntPtr _native,
			uint _index);

		public void RedrawItemAt(
			uint _index)
		{
			ExportItemBox_RedrawItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method RemoveAllItems

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_RemoveAllItems(IntPtr _native);

		public void RemoveAllItems( )
		{
			ExportItemBox_RemoveAllItems(Native);
		}

		#endregion
		#region Method RemoveItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_RemoveItemAt__index(IntPtr _native,
			uint _index);

		public void RemoveItemAt(
			uint _index)
		{
			ExportItemBox_RemoveItemAt__index(Native,
				_index);
		}

		#endregion
		#region Method AddItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_AddItem(IntPtr _native);

		public void AddItem( )
		{
			ExportItemBox_AddItem(Native);
		}

		#endregion
		#region Method InsertItemAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_InsertItemAt__index(IntPtr _native,
			uint _index);

		public void InsertItemAt(
			uint _index)
		{
			ExportItemBox_InsertItemAt__index(Native,
				_index);
		}

		#endregion
		#region Property ViewOffset

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportItemBox_GetViewOffset(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_SetViewOffset(IntPtr _widget, [In] ref IntPoint _value);

		public IntPoint ViewOffset
		{
			get { return (IntPoint)Marshal.PtrToStructure(ExportItemBox_GetViewOffset(Native), typeof(IntPoint)); }
			set { ExportItemBox_SetViewOffset(Native, ref value); }
		}

		#endregion
		#region Property WidgetDrag

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportItemBox_GetWidgetDrag(IntPtr _native);

		public Widget WidgetDrag
		{
			get { return (Widget)BaseWidget.GetByNative(ExportItemBox_GetWidgetDrag(Native)); }
		}

		#endregion
		#region Property VerticalAlignment

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportItemBox_GetVerticalAlignment(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_SetVerticalAlignment(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool VerticalAlignment
		{
			get { return ExportItemBox_GetVerticalAlignment(Native); }
			set { ExportItemBox_SetVerticalAlignment(Native, value); }
		}

		#endregion
		#region Property IndexSelected

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportItemBox_GetIndexSelected(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportItemBox_SetIndexSelected(IntPtr _widget, uint _value);

		public uint IndexSelected
		{
			get { return ExportItemBox_GetIndexSelected(Native); }
			set { ExportItemBox_SetIndexSelected(Native, value); }
		}

		#endregion
		#region Property ItemCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportItemBox_GetItemCount(IntPtr _native);

		public uint ItemCount
		{
			get { return ExportItemBox_GetItemCount(Native); }
		}

		#endregion
		
    }
}
