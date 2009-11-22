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

    public  class ScrollView : Widget
    {

        #region ScrollView

        public ScrollView()
            : base()
        {
        }

        internal ScrollView(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal ScrollView(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "ScrollView"; }

        internal static BaseWidget RequestWrapScrollView(BaseWidget _parent, IntPtr _widget)
        {
            return new ScrollView(_parent, _widget);
        }

        internal static BaseWidget RequestCreateScrollView(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new ScrollView(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   		#region Method SetCanvasSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetCanvasSize(
			int _width ,
			int _height )
		{
			ExportScrollView_SetCanvasSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Property CanvasSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetCanvasSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasSize( IntPtr _widget,  ref IntSize _value );

		public IntSize CanvasSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportScrollView_GetCanvasSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportScrollView_SetCanvasSize( mNative, ref value ); }
		}

		#endregion



   		#region Property CanvasAlign

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportScrollView_GetCanvasAlign( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align CanvasAlign
		{
			get { return  ExportScrollView_GetCanvasAlign( mNative )  ; }
			set { ExportScrollView_SetCanvasAlign( mNative,  value ); }
		}

		#endregion



   		#region Property VisibleHScroll

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollView_IsVisibleHScroll( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleHScroll( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VisibleHScroll
		{
			get { return  ExportScrollView_IsVisibleHScroll( mNative )  ; }
			set { ExportScrollView_SetVisibleHScroll( mNative,  value ); }
		}

		#endregion



   		#region Property VisibleVScroll

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollView_IsVisibleVScroll( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleVScroll( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VisibleVScroll
		{
			get { return  ExportScrollView_IsVisibleVScroll( mNative )  ; }
			set { ExportScrollView_SetVisibleVScroll( mNative,  value ); }
		}

		#endregion



   


   


   


   


   


   


   


   


   

		
    }

}
