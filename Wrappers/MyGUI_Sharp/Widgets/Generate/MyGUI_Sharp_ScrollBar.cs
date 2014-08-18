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
    public  class ScrollBar :
		Widget
    {
        #region ScrollBar

        protected override string GetWidgetType() { return "ScrollBar"; }

        internal static BaseWidget RequestWrapScrollBar(BaseWidget _parent, IntPtr _widget)
        {
			ScrollBar widget = new ScrollBar();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateScrollBar(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ScrollBar widget = new ScrollBar();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event ScrollChangePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBarEvent_AdviseScrollChangePosition(IntPtr _native, bool _advise);

		public delegate void HandleScrollChangePosition(
			ScrollBar _sender,
			uint _position);
			
		private HandleScrollChangePosition mEventScrollChangePosition;
		public event HandleScrollChangePosition EventScrollChangePosition
		{
			add
			{
				if (ExportEventScrollChangePosition.mDelegate == null)
				{
					ExportEventScrollChangePosition.mDelegate = new ExportEventScrollChangePosition.ExportHandle(OnExportScrollChangePosition);
					ExportEventScrollChangePosition.ExportScrollBarEvent_DelegateScrollChangePosition(ExportEventScrollChangePosition.mDelegate);
				}

				if (mEventScrollChangePosition == null)
					ExportScrollBarEvent_AdviseScrollChangePosition(Native, true);
				mEventScrollChangePosition += value;
			}
			remove
			{
				mEventScrollChangePosition -= value;
				if (mEventScrollChangePosition == null)
					ExportScrollBarEvent_AdviseScrollChangePosition(Native, false);
			}
		}

		private struct ExportEventScrollChangePosition
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportScrollBarEvent_DelegateScrollChangePosition(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				uint _position);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportScrollChangePosition(
			IntPtr _sender,
			uint _position)
		{
			ScrollBar sender = (ScrollBar)BaseWidget.GetByNative(_sender);

			if (sender.mEventScrollChangePosition != null)
				sender.mEventScrollChangePosition(
					sender ,
					_position);
		}

		#endregion
		#region Property MoveToClick

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportScrollBar_GetMoveToClick(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetMoveToClick(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool MoveToClick
		{
			get { return ExportScrollBar_GetMoveToClick(Native); }
			set { ExportScrollBar_SetMoveToClick(Native, value); }
		}

		#endregion
		#region Property MinTrackSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetMinTrackSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetMinTrackSize(IntPtr _widget, int _value);

		public int MinTrackSize
		{
			get { return ExportScrollBar_GetMinTrackSize(Native); }
			set { ExportScrollBar_SetMinTrackSize(Native, value); }
		}

		#endregion
		#region Property TrackSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetTrackSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetTrackSize(IntPtr _widget, int _value);

		public int TrackSize
		{
			get { return ExportScrollBar_GetTrackSize(Native); }
			set { ExportScrollBar_SetTrackSize(Native, value); }
		}

		#endregion
		#region Property LineSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetLineSize(IntPtr _native);

		public int LineSize
		{
			get { return ExportScrollBar_GetLineSize(Native); }
		}

		#endregion
		#region Property ScrollViewPage

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollViewPage(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollViewPage(IntPtr _widget, uint _value);

		public uint ScrollViewPage
		{
			get { return ExportScrollBar_GetScrollViewPage(Native); }
			set { ExportScrollBar_SetScrollViewPage(Native, value); }
		}

		#endregion
		#region Property ScrollPage

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollPage(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollPage(IntPtr _widget, uint _value);

		public uint ScrollPage
		{
			get { return ExportScrollBar_GetScrollPage(Native); }
			set { ExportScrollBar_SetScrollPage(Native, value); }
		}

		#endregion
		#region Property ScrollPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollPosition(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollPosition(IntPtr _widget, uint _value);

		public uint ScrollPosition
		{
			get { return ExportScrollBar_GetScrollPosition(Native); }
			set { ExportScrollBar_SetScrollPosition(Native, value); }
		}

		#endregion
		#region Property ScrollRange

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollRange(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollRange(IntPtr _widget, uint _value);

		public uint ScrollRange
		{
			get { return ExportScrollBar_GetScrollRange(Native); }
			set { ExportScrollBar_SetScrollRange(Native, value); }
		}

		#endregion
		#region Property VerticalAlignment

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportScrollBar_GetVerticalAlignment(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetVerticalAlignment(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool VerticalAlignment
		{
			get { return ExportScrollBar_GetVerticalAlignment(Native); }
			set { ExportScrollBar_SetVerticalAlignment(Native, value); }
		}

		#endregion
		
    }
}
