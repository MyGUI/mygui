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

    public  class DDContainer : Widget
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
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event UpdateDropState

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseUpdateDropState( IntPtr _native, bool _advise );

		public delegate void HandleUpdateDropState(
			 DDContainer _sender ,
			 Widget _items ,
			ref DDWidgetState _state );
			
		private HandleUpdateDropState mEventUpdateDropState;
		public event HandleUpdateDropState EventUpdateDropState
		{
			add
			{
				if (ExportEventUpdateDropState.mDelegate == null)
				{
					ExportEventUpdateDropState.mDelegate = new ExportEventUpdateDropState.ExportHandle( OnExportUpdateDropState );
					ExportEventUpdateDropState.ExportDDContainerEvent_DelegateUpdateDropState( ExportEventUpdateDropState.mDelegate );
				}

				if (mEventUpdateDropState == null)
					ExportDDContainerEvent_AdviseUpdateDropState( mNative, true );
				mEventUpdateDropState += value;
			}
			remove
			{
				mEventUpdateDropState -= value;
				if (mEventUpdateDropState == null)
					ExportDDContainerEvent_AdviseUpdateDropState( mNative, false );
			}
		}

		private struct ExportEventUpdateDropState
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateUpdateDropState( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _items ,
				[In] ref DDWidgetState _state );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportUpdateDropState(
			 DDContainer _sender ,
			 Widget _items ,
			ref DDWidgetState _state )
		{
			if (_sender.mEventUpdateDropState != null)
				_sender.mEventUpdateDropState(
					 _sender ,
					 _items ,
					ref _state );
		}

		#endregion
		#region Request DragWidgetInfo

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseDragWidgetInfo( IntPtr _native, bool _advise );

		public delegate void HandleDragWidgetInfo(
			 DDContainer _sender ,
			ref Widget _item ,
			ref IntCoord _dimension );
			
		private HandleDragWidgetInfo mEventDragWidgetInfo;
		public event HandleDragWidgetInfo RequestDragWidgetInfo
		{
			add
			{
				if (ExportEventDragWidgetInfo.mDelegate == null)
				{
					ExportEventDragWidgetInfo.mDelegate = new ExportEventDragWidgetInfo.ExportHandle( OnExportDragWidgetInfo );
					ExportEventDragWidgetInfo.ExportDDContainerEvent_DelegateDragWidgetInfo( ExportEventDragWidgetInfo.mDelegate );
				}

				if (mEventDragWidgetInfo == null)
					ExportDDContainerEvent_AdviseDragWidgetInfo( mNative, true );
				mEventDragWidgetInfo += value;
			}
			remove
			{
				mEventDragWidgetInfo -= value;
				if (mEventDragWidgetInfo == null)
					ExportDDContainerEvent_AdviseDragWidgetInfo( mNative, false );
			}
		}

		private struct ExportEventDragWidgetInfo
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateDragWidgetInfo( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[In, Out] ref IntPtr _item ,
				[In, Out] ref IntCoord _dimension );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportDragWidgetInfo(
			 DDContainer _sender ,
			ref IntPtr _item ,
			ref IntCoord _dimension )
		{
            Widget __item = null;
            
			if (_sender.mEventDragWidgetInfo != null)
				_sender.mEventDragWidgetInfo(
					 _sender ,
					ref __item ,
					ref _dimension );
					
            _item = (__item == null) ? IntPtr.Zero : __item.GetNative();
		}

		#endregion
		#region Event ChangeDDState

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseChangeDDState( IntPtr _native, bool _advise );

		public delegate void HandleChangeDDState(
			 DDContainer _sender ,
			 DDItemState _state );
			
		private HandleChangeDDState mEventChangeDDState;
		public event HandleChangeDDState EventChangeDDState
		{
			add
			{
				if (ExportEventChangeDDState.mDelegate == null)
				{
					ExportEventChangeDDState.mDelegate = new ExportEventChangeDDState.ExportHandle( OnExportChangeDDState );
					ExportEventChangeDDState.ExportDDContainerEvent_DelegateChangeDDState( ExportEventChangeDDState.mDelegate );
				}

				if (mEventChangeDDState == null)
					ExportDDContainerEvent_AdviseChangeDDState( mNative, true );
				mEventChangeDDState += value;
			}
			remove
			{
				mEventChangeDDState -= value;
				if (mEventChangeDDState == null)
					ExportDDContainerEvent_AdviseChangeDDState( mNative, false );
			}
		}

		private struct ExportEventChangeDDState
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateChangeDDState( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[MarshalAs(UnmanagedType.I4)]  DDItemState _state );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeDDState(
			 DDContainer _sender ,
			 DDItemState _state )
		{
			if (_sender.mEventChangeDDState != null)
				_sender.mEventChangeDDState(
					 _sender ,
					 _state );
		}

		#endregion
		#region Event DropResult

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseDropResult( IntPtr _native, bool _advise );

		public delegate void HandleDropResult(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			 bool _result );
			
		private HandleDropResult mEventDropResult;
		public event HandleDropResult EventDropResult
		{
			add
			{
				if (ExportEventDropResult.mDelegate == null)
				{
					ExportEventDropResult.mDelegate = new ExportEventDropResult.ExportHandle( OnExportDropResult );
					ExportEventDropResult.ExportDDContainerEvent_DelegateDropResult( ExportEventDropResult.mDelegate );
				}

				if (mEventDropResult == null)
					ExportDDContainerEvent_AdviseDropResult( mNative, true );
				mEventDropResult += value;
			}
			remove
			{
				mEventDropResult -= value;
				if (mEventDropResult == null)
					ExportDDContainerEvent_AdviseDropResult( mNative, false );
			}
		}

		private struct ExportEventDropResult
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateDropResult( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)]  bool _result );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportDropResult(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			 bool _result )
		{
			if (_sender.mEventDropResult != null)
				_sender.mEventDropResult(
					 _sender ,
					ref _info ,
					 _result );
		}

		#endregion
		#region Event RequestDrop

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseRequestDrop( IntPtr _native, bool _advise );

		public delegate void HandleRequestDrop(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			ref bool _result );
			
		private HandleRequestDrop mEventRequestDrop;
		public event HandleRequestDrop EventRequestDrop
		{
			add
			{
				if (ExportEventRequestDrop.mDelegate == null)
				{
					ExportEventRequestDrop.mDelegate = new ExportEventRequestDrop.ExportHandle( OnExportRequestDrop );
					ExportEventRequestDrop.ExportDDContainerEvent_DelegateRequestDrop( ExportEventRequestDrop.mDelegate );
				}

				if (mEventRequestDrop == null)
					ExportDDContainerEvent_AdviseRequestDrop( mNative, true );
				mEventRequestDrop += value;
			}
			remove
			{
				mEventRequestDrop -= value;
				if (mEventRequestDrop == null)
					ExportDDContainerEvent_AdviseRequestDrop( mNative, false );
			}
		}

		private struct ExportEventRequestDrop
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateRequestDrop( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)] ref bool _result );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportRequestDrop(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			ref bool _result )
		{
			if (_sender.mEventRequestDrop != null)
				_sender.mEventRequestDrop(
					 _sender ,
					ref _info ,
					ref _result );
		}

		#endregion
		#region Event StartDrag

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseStartDrag( IntPtr _native, bool _advise );

		public delegate void HandleStartDrag(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			ref bool _result );
			
		private HandleStartDrag mEventStartDrag;
		public event HandleStartDrag EventStartDrag
		{
			add
			{
				if (ExportEventStartDrag.mDelegate == null)
				{
					ExportEventStartDrag.mDelegate = new ExportEventStartDrag.ExportHandle( OnExportStartDrag );
					ExportEventStartDrag.ExportDDContainerEvent_DelegateStartDrag( ExportEventStartDrag.mDelegate );
				}

				if (mEventStartDrag == null)
					ExportDDContainerEvent_AdviseStartDrag( mNative, true );
				mEventStartDrag += value;
			}
			remove
			{
				mEventStartDrag -= value;
				if (mEventStartDrag == null)
					ExportDDContainerEvent_AdviseStartDrag( mNative, false );
			}
		}

		private struct ExportEventStartDrag
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportDDContainerEvent_DelegateStartDrag( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)] ref bool _result );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportStartDrag(
			 DDContainer _sender ,
			ref DDItemInfo _info ,
			ref bool _result )
		{
			if (_sender.mEventStartDrag != null)
				_sender.mEventStartDrag(
					 _sender ,
					ref _info ,
					ref _result );
		}

		#endregion
		#region Method ResetDrag

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_ResetDrag( IntPtr _native );

		public void ResetDrag( )
		{
			ExportDDContainer_ResetDrag(  mNative );
		}

		#endregion
		#region Property NeedDragDrop

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportDDContainer_GetNeedDragDrop( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_SetNeedDragDrop( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedDragDrop
		{
			get { return  ExportDDContainer_GetNeedDragDrop( mNative )  ; }
			set { ExportDDContainer_SetNeedDragDrop( mNative,  value ); }
		}

		#endregion
		
    }

}
