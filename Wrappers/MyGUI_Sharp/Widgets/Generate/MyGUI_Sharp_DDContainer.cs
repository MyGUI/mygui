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
    public  class DDContainer :
		Widget
    {
        #region DDContainer

        protected override string GetWidgetType() { return "DDContainer"; }

        internal static BaseWidget RequestWrapDDContainer(BaseWidget _parent, IntPtr _widget)
        {
			DDContainer widget = new DDContainer();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateDDContainer(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			DDContainer widget = new DDContainer();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event UpdateDropState

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseUpdateDropState(IntPtr _native, bool _advise);

		public delegate void HandleUpdateDropState(
			DDContainer _sender,
			Widget _items,
			ref DDWidgetState _state);
			
		private HandleUpdateDropState mEventUpdateDropState;
		public event HandleUpdateDropState EventUpdateDropState
		{
			add
			{
				if (ExportEventUpdateDropState.mDelegate == null)
				{
					ExportEventUpdateDropState.mDelegate = new ExportEventUpdateDropState.ExportHandle(OnExportUpdateDropState);
					ExportEventUpdateDropState.ExportDDContainerEvent_DelegateUpdateDropState(ExportEventUpdateDropState.mDelegate);
				}

				if (mEventUpdateDropState == null)
					ExportDDContainerEvent_AdviseUpdateDropState(Native, true);
				mEventUpdateDropState += value;
			}
			remove
			{
				mEventUpdateDropState -= value;
				if (mEventUpdateDropState == null)
					ExportDDContainerEvent_AdviseUpdateDropState(Native, false);
			}
		}

		private struct ExportEventUpdateDropState
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateUpdateDropState(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				IntPtr _items,
				[In] ref DDWidgetState _state);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportUpdateDropState(
			IntPtr _sender,
			IntPtr _items,
			ref DDWidgetState _state)
		{
			DDContainer sender = (DDContainer)BaseWidget.GetByNative(_sender);

			if (sender.mEventUpdateDropState != null)
				sender.mEventUpdateDropState(
					sender ,
					(Widget)BaseWidget.GetByNative(_items),
					ref _state);
		}

		#endregion
		#region Event ChangeDDState

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseChangeDDState(IntPtr _native, bool _advise);

		public delegate void HandleChangeDDState(
			DDContainer _sender,
			DDItemState _state);
			
		private HandleChangeDDState mEventChangeDDState;
		public event HandleChangeDDState EventChangeDDState
		{
			add
			{
				if (ExportEventChangeDDState.mDelegate == null)
				{
					ExportEventChangeDDState.mDelegate = new ExportEventChangeDDState.ExportHandle(OnExportChangeDDState);
					ExportEventChangeDDState.ExportDDContainerEvent_DelegateChangeDDState(ExportEventChangeDDState.mDelegate);
				}

				if (mEventChangeDDState == null)
					ExportDDContainerEvent_AdviseChangeDDState(Native, true);
				mEventChangeDDState += value;
			}
			remove
			{
				mEventChangeDDState -= value;
				if (mEventChangeDDState == null)
					ExportDDContainerEvent_AdviseChangeDDState(Native, false);
			}
		}

		private struct ExportEventChangeDDState
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateChangeDDState(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[MarshalAs(UnmanagedType.I4)] DDItemState _state);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeDDState(
			IntPtr _sender,
			DDItemState _state)
		{
			DDContainer sender = (DDContainer)BaseWidget.GetByNative(_sender);

			if (sender.mEventChangeDDState != null)
				sender.mEventChangeDDState(
					sender ,
					_state);
		}

		#endregion
		#region Event DropResult

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseDropResult(IntPtr _native, bool _advise);

		public delegate void HandleDropResult(
			DDContainer _sender,
			ref DDItemInfo _info,
			bool _result);
			
		private HandleDropResult mEventDropResult;
		public event HandleDropResult EventDropResult
		{
			add
			{
				if (ExportEventDropResult.mDelegate == null)
				{
					ExportEventDropResult.mDelegate = new ExportEventDropResult.ExportHandle(OnExportDropResult);
					ExportEventDropResult.ExportDDContainerEvent_DelegateDropResult(ExportEventDropResult.mDelegate);
				}

				if (mEventDropResult == null)
					ExportDDContainerEvent_AdviseDropResult(Native, true);
				mEventDropResult += value;
			}
			remove
			{
				mEventDropResult -= value;
				if (mEventDropResult == null)
					ExportDDContainerEvent_AdviseDropResult(Native, false);
			}
		}

		private struct ExportEventDropResult
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateDropResult(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[In] ref DDItemInfo _info,
				[MarshalAs(UnmanagedType.U1)] bool _result);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportDropResult(
			IntPtr _sender,
			ref DDItemInfo _info,
			bool _result)
		{
			DDContainer sender = (DDContainer)BaseWidget.GetByNative(_sender);

			if (sender.mEventDropResult != null)
				sender.mEventDropResult(
					sender ,
					ref _info,
					_result);
		}

		#endregion
		#region Event RequestDrop

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseRequestDrop(IntPtr _native, bool _advise);

		public delegate void HandleRequestDrop(
			DDContainer _sender,
			ref DDItemInfo _info,
			ref bool _result);
			
		private HandleRequestDrop mEventRequestDrop;
		public event HandleRequestDrop EventRequestDrop
		{
			add
			{
				if (ExportEventRequestDrop.mDelegate == null)
				{
					ExportEventRequestDrop.mDelegate = new ExportEventRequestDrop.ExportHandle(OnExportRequestDrop);
					ExportEventRequestDrop.ExportDDContainerEvent_DelegateRequestDrop(ExportEventRequestDrop.mDelegate);
				}

				if (mEventRequestDrop == null)
					ExportDDContainerEvent_AdviseRequestDrop(Native, true);
				mEventRequestDrop += value;
			}
			remove
			{
				mEventRequestDrop -= value;
				if (mEventRequestDrop == null)
					ExportDDContainerEvent_AdviseRequestDrop(Native, false);
			}
		}

		private struct ExportEventRequestDrop
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateRequestDrop(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[In] ref DDItemInfo _info,
				[MarshalAs(UnmanagedType.U1)] ref bool _result);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportRequestDrop(
			IntPtr _sender,
			ref DDItemInfo _info,
			ref bool _result)
		{
			DDContainer sender = (DDContainer)BaseWidget.GetByNative(_sender);

			if (sender.mEventRequestDrop != null)
				sender.mEventRequestDrop(
					sender ,
					ref _info,
					ref _result);
		}

		#endregion
		#region Event StartDrag

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseStartDrag(IntPtr _native, bool _advise);

		public delegate void HandleStartDrag(
			DDContainer _sender,
			ref DDItemInfo _info,
			ref bool _result);
			
		private HandleStartDrag mEventStartDrag;
		public event HandleStartDrag EventStartDrag
		{
			add
			{
				if (ExportEventStartDrag.mDelegate == null)
				{
					ExportEventStartDrag.mDelegate = new ExportEventStartDrag.ExportHandle(OnExportStartDrag);
					ExportEventStartDrag.ExportDDContainerEvent_DelegateStartDrag(ExportEventStartDrag.mDelegate);
				}

				if (mEventStartDrag == null)
					ExportDDContainerEvent_AdviseStartDrag(Native, true);
				mEventStartDrag += value;
			}
			remove
			{
				mEventStartDrag -= value;
				if (mEventStartDrag == null)
					ExportDDContainerEvent_AdviseStartDrag(Native, false);
			}
		}

		private struct ExportEventStartDrag
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateStartDrag(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[In] ref DDItemInfo _info,
				[MarshalAs(UnmanagedType.U1)] ref bool _result);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportStartDrag(
			IntPtr _sender,
			ref DDItemInfo _info,
			ref bool _result)
		{
			DDContainer sender = (DDContainer)BaseWidget.GetByNative(_sender);

			if (sender.mEventStartDrag != null)
				sender.mEventStartDrag(
					sender ,
					ref _info,
					ref _result);
		}

		#endregion
		#region Method ResetDrag

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_ResetDrag(IntPtr _native);

		public void ResetDrag( )
		{
			ExportDDContainer_ResetDrag(Native);
		}

		#endregion
		#region Property NeedDragDrop

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportDDContainer_GetNeedDragDrop(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_SetNeedDragDrop(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool NeedDragDrop
		{
			get { return ExportDDContainer_GetNeedDragDrop(Native); }
			set { ExportDDContainer_SetNeedDragDrop(Native, value); }
		}

		#endregion
		
    }
}
