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

    public  class ProgressBar : Widget
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
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   


   


   


   


   		#region Property FlowDirection

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern FlowDirection ExportProgressBar_GetFlowDirection( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetFlowDirection( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  FlowDirection _value );

		public FlowDirection FlowDirection
		{
			get { return  ExportProgressBar_GetFlowDirection( mNative )  ; }
			set { ExportProgressBar_SetFlowDirection( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressAutoTrack

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportProgressBar_GetProgressAutoTrack( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressAutoTrack( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ProgressAutoTrack
		{
			get { return  ExportProgressBar_GetProgressAutoTrack( mNative )  ; }
			set { ExportProgressBar_SetProgressAutoTrack( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressPosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressPosition( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressPosition( IntPtr _widget,   uint _value );

		public uint ProgressPosition
		{
			get { return  ExportProgressBar_GetProgressPosition( mNative )  ; }
			set { ExportProgressBar_SetProgressPosition( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressRange

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressRange( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressRange( IntPtr _widget,   uint _value );

		public uint ProgressRange
		{
			get { return  ExportProgressBar_GetProgressRange( mNative )  ; }
			set { ExportProgressBar_SetProgressRange( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}
