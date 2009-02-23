using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class DDContainer
    {
   		#region Request DragWidgetInfo

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainerEvent_AdviseDragWidgetInfo( IntPtr _widget, bool _advise );

		public delegate void HandleDragWidgetInfo(
			 DDContainer _sender ,
			 ref Widget _item ,
			 ref IntCoord _dimension );
			
		private HandleDragWidgetInfo mEventDragWidgetInfo;
		public event HandleDragWidgetInfo RequestDragWidgetInfo
		{
			add
			{
				if (mEventDragWidgetInfo == null) ExportDDContainerEvent_AdviseDragWidgetInfo( mNative, true );
				mEventDragWidgetInfo += value;
			}
			remove
			{
				mEventDragWidgetInfo -= value;
				if (mEventDragWidgetInfo == null) ExportDDContainerEvent_AdviseDragWidgetInfo( mNative, false );
			}
		}


		private struct ExportEventDragWidgetInfo
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDDContainerEvent_DelegateDragWidgetInfo( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  DDContainer _sender ,
    			 [In, Out] ref IntPtr _item ,
	    		 [In, Out] ref IntCoord _dimension );
				
			private static ExportHandle mDelegate;
			public ExportEventDragWidgetInfo( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportDDContainerEvent_DelegateDragWidgetInfo( mDelegate );
			}
		}
		static ExportEventDragWidgetInfo mExportDragWidgetInfo =
			new ExportEventDragWidgetInfo(new ExportEventDragWidgetInfo.ExportHandle( OnExportDragWidgetInfo ));

		private static void OnExportDragWidgetInfo(
			 DDContainer _sender ,
			 ref IntPtr _item ,
			 ref IntCoord _dimension )
		{
            Widget widget = null;
			if (_sender.mEventDragWidgetInfo != null)
				_sender.mEventDragWidgetInfo(
					 _sender ,
                     ref widget,
					 ref _dimension );
            _item = (widget == null) ? IntPtr.Zero : widget.GetNative();
		}

		#endregion
    }

} // namespace MyGUI.Sharp
