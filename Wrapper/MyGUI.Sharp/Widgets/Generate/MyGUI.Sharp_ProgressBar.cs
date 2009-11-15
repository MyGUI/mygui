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

        public ProgressBar()
            : base()
        {
        }

        internal ProgressBar(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal ProgressBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Progress"; }

        internal static BaseWidget RequestWrapProgressBar(BaseWidget _parent, IntPtr _widget)
        {
            return new ProgressBar(_parent, _widget);
        }

        internal static BaseWidget RequestCreateProgressBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new ProgressBar(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Method SetProperty

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProperty_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetProperty(
			string _key ,
			string _value )
		{
			ExportProgressBar_SetProperty_key_value( mNative , 
				 _key ,
				 _value );
		}

		#endregion



   


   


   


   


   


   


   		#region Property ProgressStartPoint

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportProgressBar_GetProgressStartPoint( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressStartPoint( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align ProgressStartPoint
		{
			get { return  ExportProgressBar_GetProgressStartPoint( mNative )  ; }
			set { ExportProgressBar_SetProgressStartPoint( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressAutoTrack

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportProgressBar_GetProgressAutoTrack( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressAutoTrack( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ProgressAutoTrack
		{
			get { return  ExportProgressBar_GetProgressAutoTrack( mNative )  ; }
			set { ExportProgressBar_SetProgressAutoTrack( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressPosition( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressPosition( IntPtr _widget,   uint _value );

		public uint ProgressPosition
		{
			get { return  ExportProgressBar_GetProgressPosition( mNative )  ; }
			set { ExportProgressBar_SetProgressPosition( mNative,  value ); }
		}

		#endregion



   		#region Property ProgressRange

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportProgressBar_GetProgressRange( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportProgressBar_SetProgressRange( IntPtr _widget,   uint _value );

		public uint ProgressRange
		{
			get { return  ExportProgressBar_GetProgressRange( mNative )  ; }
			set { ExportProgressBar_SetProgressRange( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}
