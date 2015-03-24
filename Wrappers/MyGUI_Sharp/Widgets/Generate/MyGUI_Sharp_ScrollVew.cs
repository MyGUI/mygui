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
    public  class ScrollView :
		Widget
    {
        #region ScrollView

        protected override string GetWidgetType() { return "ScrollView"; }

        internal static BaseWidget RequestWrapScrollView(BaseWidget _parent, IntPtr _widget)
        {
			ScrollView widget = new ScrollView();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateScrollView(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ScrollView widget = new ScrollView();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetCanvasSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasSize__width__height(IntPtr _native,
			int _width,
			int _height);

		public void SetCanvasSize(
			int _width,
			int _height)
		{
			ExportScrollView_SetCanvasSize__width__height(Native,
				_width,
				_height);
		}

		#endregion
		#region Property ViewOffset

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetViewOffset(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetViewOffset(IntPtr _widget, [In] ref IntPoint _value);

		public IntPoint ViewOffset
		{
			get { return (IntPoint)Marshal.PtrToStructure(ExportScrollView_GetViewOffset(Native), typeof(IntPoint)); }
			set { ExportScrollView_SetViewOffset(Native, ref value); }
		}

		#endregion
		#region Property ViewCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetViewCoord(IntPtr _native);

		public IntCoord ViewCoord
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportScrollView_GetViewCoord(Native), typeof(IntCoord)); }
		}

		#endregion
		#region Property CanvasSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetCanvasSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasSize(IntPtr _widget, [In] ref IntSize _value);

		public IntSize CanvasSize
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportScrollView_GetCanvasSize(Native), typeof(IntSize)); }
			set { ExportScrollView_SetCanvasSize(Native, ref value); }
		}

		#endregion
		#region Property CanvasAlign

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern Align ExportScrollView_GetCanvasAlign(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasAlign(IntPtr _widget, [MarshalAs(UnmanagedType.I4)] Align _value);

		public Align CanvasAlign
		{
			get { return ExportScrollView_GetCanvasAlign(Native); }
			set { ExportScrollView_SetCanvasAlign(Native, value); }
		}

		#endregion
		#region Property IsVisibleHScroll

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportScrollView_IsVisibleHScroll(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleHScroll(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool IsVisibleHScroll
		{
			get { return ExportScrollView_IsVisibleHScroll(Native); }
			set { ExportScrollView_SetVisibleHScroll(Native, value); }
		}

		#endregion
		#region Property IsVisibleVScroll

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportScrollView_IsVisibleVScroll(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleVScroll(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool IsVisibleVScroll
		{
			get { return ExportScrollView_IsVisibleVScroll(Native); }
			set { ExportScrollView_SetVisibleVScroll(Native, value); }
		}

		#endregion
		
    }
}
