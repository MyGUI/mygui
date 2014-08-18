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
    public  class Window :
		TextBox
    {
        #region Window

        protected override string GetWidgetType() { return "Window"; }

        internal static BaseWidget RequestWrapWindow(BaseWidget _parent, IntPtr _widget)
        {
			Window widget = new Window();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateWindow(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			Window widget = new Window();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event WindowChangeCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindowEvent_AdviseWindowChangeCoord(IntPtr _native, bool _advise);

		public delegate void HandleWindowChangeCoord(
			Window _sender);
			
		private HandleWindowChangeCoord mEventWindowChangeCoord;
		public event HandleWindowChangeCoord EventWindowChangeCoord
		{
			add
			{
				if (ExportEventWindowChangeCoord.mDelegate == null)
				{
					ExportEventWindowChangeCoord.mDelegate = new ExportEventWindowChangeCoord.ExportHandle(OnExportWindowChangeCoord);
					ExportEventWindowChangeCoord.ExportWindowEvent_DelegateWindowChangeCoord(ExportEventWindowChangeCoord.mDelegate);
				}

				if (mEventWindowChangeCoord == null)
					ExportWindowEvent_AdviseWindowChangeCoord(Native, true);
				mEventWindowChangeCoord += value;
			}
			remove
			{
				mEventWindowChangeCoord -= value;
				if (mEventWindowChangeCoord == null)
					ExportWindowEvent_AdviseWindowChangeCoord(Native, false);
			}
		}

		private struct ExportEventWindowChangeCoord
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWindowEvent_DelegateWindowChangeCoord(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportWindowChangeCoord(
			IntPtr _sender)
		{
			Window sender = (Window)BaseWidget.GetByNative(_sender);

			if (sender.mEventWindowChangeCoord != null)
				sender.mEventWindowChangeCoord(
					sender);
		}

		#endregion
		#region Event WindowButtonPressed

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindowEvent_AdviseWindowButtonPressed(IntPtr _native, bool _advise);

		public delegate void HandleWindowButtonPressed(
			Window _sender,
			string _name);
			
		private HandleWindowButtonPressed mEventWindowButtonPressed;
		public event HandleWindowButtonPressed EventWindowButtonPressed
		{
			add
			{
				if (ExportEventWindowButtonPressed.mDelegate == null)
				{
					ExportEventWindowButtonPressed.mDelegate = new ExportEventWindowButtonPressed.ExportHandle(OnExportWindowButtonPressed);
					ExportEventWindowButtonPressed.ExportWindowEvent_DelegateWindowButtonPressed(ExportEventWindowButtonPressed.mDelegate);
				}

				if (mEventWindowButtonPressed == null)
					ExportWindowEvent_AdviseWindowButtonPressed(Native, true);
				mEventWindowButtonPressed += value;
			}
			remove
			{
				mEventWindowButtonPressed -= value;
				if (mEventWindowButtonPressed == null)
					ExportWindowEvent_AdviseWindowButtonPressed(Native, false);
			}
		}

		private struct ExportEventWindowButtonPressed
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWindowEvent_DelegateWindowButtonPressed(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[MarshalAs(UnmanagedType.LPStr)] string _name);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportWindowButtonPressed(
			IntPtr _sender,
			string _name)
		{
			Window sender = (Window)BaseWidget.GetByNative(_sender);

			if (sender.mEventWindowButtonPressed != null)
				sender.mEventWindowButtonPressed(
					sender ,
					_name);
		}

		#endregion
		#region Method SetMaxSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize__width__height(IntPtr _native,
			int _width,
			int _height);

		public void SetMaxSize(
			int _width,
			int _height)
		{
			ExportWindow_SetMaxSize__width__height(Native,
				_width,
				_height);
		}

		#endregion
		#region Method SetMinSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize__width__height(IntPtr _native,
			int _width,
			int _height);

		public void SetMinSize(
			int _width,
			int _height)
		{
			ExportWindow_SetMinSize__width__height(Native,
				_width,
				_height);
		}

		#endregion
		#region Method SetVisibleSmooth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetVisibleSmooth__value(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _value);

		public void SetVisibleSmooth(
			bool _value)
		{
			ExportWindow_SetVisibleSmooth__value(Native,
				_value);
		}

		#endregion
		#region Property Movable

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWindow_GetMovable(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMovable(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool Movable
		{
			get { return ExportWindow_GetMovable(Native); }
			set { ExportWindow_SetMovable(Native, value); }
		}

		#endregion
		#region Property ActionScale

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetActionScale(IntPtr _native);

		public IntCoord ActionScale
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportWindow_GetActionScale(Native), typeof(IntCoord)); }
		}

		#endregion
		#region Property Snap

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWindow_GetSnap(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetSnap(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool Snap
		{
			get { return ExportWindow_GetSnap(Native); }
			set { ExportWindow_SetSnap(Native, value); }
		}

		#endregion
		#region Property MaxSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMaxSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize(IntPtr _widget, [In] ref IntSize _value);

		public IntSize MaxSize
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportWindow_GetMaxSize(Native), typeof(IntSize)); }
			set { ExportWindow_SetMaxSize(Native, ref value); }
		}

		#endregion
		#region Property MinSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMinSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize(IntPtr _widget, [In] ref IntSize _value);

		public IntSize MinSize
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportWindow_GetMinSize(Native), typeof(IntSize)); }
			set { ExportWindow_SetMinSize(Native, ref value); }
		}

		#endregion
		#region Property CaptionWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetCaptionWidget(IntPtr _native);

		public TextBox CaptionWidget
		{
			get { return (TextBox)BaseWidget.GetByNative(ExportWindow_GetCaptionWidget(Native)); }
		}

		#endregion
		#region Property AutoAlpha

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWindow_GetAutoAlpha(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetAutoAlpha(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool AutoAlpha
		{
			get { return ExportWindow_GetAutoAlpha(Native); }
			set { ExportWindow_SetAutoAlpha(Native, value); }
		}

		#endregion
		
    }
}
