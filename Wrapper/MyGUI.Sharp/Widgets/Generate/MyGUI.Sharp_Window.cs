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

    public  class Window : Widget
    {

        #region Window

        public Window()
            : base()
        {
        }

        internal Window(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal Window(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Window"; }

        internal static BaseWidget RequestWrapWindow(BaseWidget _parent, IntPtr _widget)
        {
            return new Window(_parent, _widget);
        }

        internal static BaseWidget RequestCreateWindow(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new Window(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint





   		#region Method SetProperty

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetProperty_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetProperty(
			string _key ,
			string _value )
		{
			ExportWindow_SetProperty_key_value( mNative , 
				 _key ,
				 _value );
		}

		#endregion



   		#region Property Snap

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWindow_GetSnap( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetSnap( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Snap
		{
			get { return  ExportWindow_GetSnap( mNative )  ; }
			set { ExportWindow_SetSnap( mNative,  value ); }
		}

		#endregion



   


   


   


   


   


   


   		#region Method SetMaxSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetMaxSize(
			int _width ,
			int _height )
		{
			ExportWindow_SetMaxSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Property MaxSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMaxSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize( IntPtr _widget,  ref IntSize _value );

		public IntSize MaxSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWindow_GetMaxSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWindow_SetMaxSize( mNative, ref value ); }
		}

		#endregion



   		#region Method SetMinSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetMinSize(
			int _width ,
			int _height )
		{
			ExportWindow_SetMinSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Property MinSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMinSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize( IntPtr _widget,  ref IntSize _value );

		public IntSize MinSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWindow_GetMinSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWindow_SetMinSize( mNative, ref value ); }
		}

		#endregion



   


   		#region Property AutoAlpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWindow_GetAutoAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetAutoAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool AutoAlpha
		{
			get { return  ExportWindow_GetAutoAlpha( mNative )  ; }
			set { ExportWindow_SetAutoAlpha( mNative,  value ); }
		}

		#endregion



   


   		#region Method SetVisibleSmooth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetVisibleSmooth_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetVisibleSmooth(
			bool _value )
		{
			ExportWindow_SetVisibleSmooth_value( mNative , 
				 _value );
		}

		#endregion



   


   


   


   

		
    }

}
