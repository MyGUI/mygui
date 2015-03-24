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
    public  class ProgressBar :
		Widget
    {
        #region ProgressBar

        protected override string GetWidgetType() { return "ProgressBar"; }

        internal static BaseWidget RequestWrapProgressBar(BaseWidget _parent, IntPtr _widget)
        {
			ProgressBar widget = new ProgressBar();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateProgressBar(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ProgressBar widget = new ProgressBar();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Property FlowDirection

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern FlowDirection ExportProgressBar_GetFlowDirection(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetFlowDirection(IntPtr _widget, [MarshalAs(UnmanagedType.I4)] FlowDirection _value);

		public FlowDirection FlowDirection
		{
			get { return ExportProgressBar_GetFlowDirection(Native); }
			set { ExportProgressBar_SetFlowDirection(Native, value); }
		}

		#endregion
		#region Property ProgressAutoTrack

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportProgressBar_GetProgressAutoTrack(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressAutoTrack(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool ProgressAutoTrack
		{
			get { return ExportProgressBar_GetProgressAutoTrack(Native); }
			set { ExportProgressBar_SetProgressAutoTrack(Native, value); }
		}

		#endregion
		#region Property ProgressPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressPosition(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressPosition(IntPtr _widget, uint _value);

		public uint ProgressPosition
		{
			get { return ExportProgressBar_GetProgressPosition(Native); }
			set { ExportProgressBar_SetProgressPosition(Native, value); }
		}

		#endregion
		#region Property ProgressRange

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressRange(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressRange(IntPtr _widget, uint _value);

		public uint ProgressRange
		{
			get { return ExportProgressBar_GetProgressRange(Native); }
			set { ExportProgressBar_SetProgressRange(Native, value); }
		}

		#endregion
		
    }
}
