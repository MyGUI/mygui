﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class DDContainer : Widget
    {

        #region DDContainer

        public DDContainer()
            : base()
        {
        }

        public DDContainer(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public DDContainer(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "DDContainer"; }

		#endregion
	
		
		//InsertPoint

   		#region Event UpdateDropState

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventUpdateDropState == null) ExportDDContainerEvent_AdviseUpdateDropState( mNative, true );
				mEventUpdateDropState += value;
			}
			remove
			{
				mEventUpdateDropState -= value;
				if (mEventUpdateDropState == null) ExportDDContainerEvent_AdviseUpdateDropState( mNative, false );
			}
		}


		private struct ExportEventUpdateDropState
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateUpdateDropState( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
				[MarshalAs(UnmanagedType.Interface)]  Widget _items ,
				[In] ref DDWidgetState _state );
				
			private static ExportHandle mDelegate;
			public ExportEventUpdateDropState( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateUpdateDropState( mDelegate );
			}
		}
		static ExportEventUpdateDropState mExportUpdateDropState =
			new ExportEventUpdateDropState(new ExportEventUpdateDropState.ExportHandle( OnExportUpdateDropState ));

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





   		#region Event ChangeDDState

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseChangeDDState( IntPtr _native, bool _advise );

		public delegate void HandleChangeDDState(
			 DDContainer _sender ,
			 DDItemState _state );
			
		private HandleChangeDDState mEventChangeDDState;
		public event HandleChangeDDState EventChangeDDState
		{
			add
			{
				if (mEventChangeDDState == null) ExportDDContainerEvent_AdviseChangeDDState( mNative, true );
				mEventChangeDDState += value;
			}
			remove
			{
				mEventChangeDDState -= value;
				if (mEventChangeDDState == null) ExportDDContainerEvent_AdviseChangeDDState( mNative, false );
			}
		}


		private struct ExportEventChangeDDState
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateChangeDDState( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
				[MarshalAs(UnmanagedType.I4)]  DDItemState _state );
				
			private static ExportHandle mDelegate;
			public ExportEventChangeDDState( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateChangeDDState( mDelegate );
			}
		}
		static ExportEventChangeDDState mExportChangeDDState =
			new ExportEventChangeDDState(new ExportEventChangeDDState.ExportHandle( OnExportChangeDDState ));

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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventDropResult == null) ExportDDContainerEvent_AdviseDropResult( mNative, true );
				mEventDropResult += value;
			}
			remove
			{
				mEventDropResult -= value;
				if (mEventDropResult == null) ExportDDContainerEvent_AdviseDropResult( mNative, false );
			}
		}


		private struct ExportEventDropResult
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateDropResult( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)]  bool _result );
				
			private static ExportHandle mDelegate;
			public ExportEventDropResult( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateDropResult( mDelegate );
			}
		}
		static ExportEventDropResult mExportDropResult =
			new ExportEventDropResult(new ExportEventDropResult.ExportHandle( OnExportDropResult ));

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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventRequestDrop == null) ExportDDContainerEvent_AdviseRequestDrop( mNative, true );
				mEventRequestDrop += value;
			}
			remove
			{
				mEventRequestDrop -= value;
				if (mEventRequestDrop == null) ExportDDContainerEvent_AdviseRequestDrop( mNative, false );
			}
		}


		private struct ExportEventRequestDrop
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateRequestDrop( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)] ref bool _result );
				
			private static ExportHandle mDelegate;
			public ExportEventRequestDrop( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateRequestDrop( mDelegate );
			}
		}
		static ExportEventRequestDrop mExportRequestDrop =
			new ExportEventRequestDrop(new ExportEventRequestDrop.ExportHandle( OnExportRequestDrop ));

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

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventStartDrag == null) ExportDDContainerEvent_AdviseStartDrag( mNative, true );
				mEventStartDrag += value;
			}
			remove
			{
				mEventStartDrag -= value;
				if (mEventStartDrag == null) ExportDDContainerEvent_AdviseStartDrag( mNative, false );
			}
		}


		private struct ExportEventStartDrag
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateStartDrag( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
				[In] ref DDItemInfo _info ,
				[MarshalAs(UnmanagedType.U1)] ref bool _result );
				
			private static ExportHandle mDelegate;
			public ExportEventStartDrag( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateStartDrag( mDelegate );
			}
		}
		static ExportEventStartDrag mExportStartDrag =
			new ExportEventStartDrag(new ExportEventStartDrag.ExportHandle( OnExportStartDrag ));

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



   		#region Property NeedDragDrop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportDDContainer_GetNeedDragDrop( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_SetNeedDragDrop( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedDragDrop
		{
			get { return ExportDDContainer_GetNeedDragDrop( mNative ); }
			set { ExportDDContainer_SetNeedDragDrop( mNative,  value ); }
		}

		#endregion


		
    }

}
