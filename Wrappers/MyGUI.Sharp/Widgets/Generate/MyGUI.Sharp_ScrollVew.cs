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
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Property ViewOffset

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetViewOffset( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetViewOffset( IntPtr _widget, [In] ref IntPoint _value );

		public IntPoint ViewOffset
		{
			get { return  (IntPoint)Marshal.PtrToStructure(  ExportScrollView_GetViewOffset( mNative )  , typeof(IntPoint) )  ; }
			set { ExportScrollView_SetViewOffset( mNative, ref value ); }
		}

		#endregion



   		#region Method GetViewCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetViewCoord( IntPtr _native );

		public IntCoord GetViewCoord( )
		{
			return  (IntCoord)Marshal.PtrToStructure(  ExportScrollView_GetViewCoord( mNative )  , typeof(IntCoord) )  ;
		}

		#endregion



   		#region Method SetCanvasSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportScrollView_GetCanvasSize( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasSize( IntPtr _widget,  ref IntSize _value );

		public IntSize CanvasSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportScrollView_GetCanvasSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportScrollView_SetCanvasSize( mNative, ref value ); }
		}

		#endregion



   		#region Property CanvasAlign

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportScrollView_GetCanvasAlign( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetCanvasAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align CanvasAlign
		{
			get { return  ExportScrollView_GetCanvasAlign( mNative )  ; }
			set { ExportScrollView_SetCanvasAlign( mNative,  value ); }
		}

		#endregion



   		#region Property VisibleHScroll

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollView_IsVisibleHScroll( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleHScroll( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VisibleHScroll
		{
			get { return  ExportScrollView_IsVisibleHScroll( mNative )  ; }
			set { ExportScrollView_SetVisibleHScroll( mNative,  value ); }
		}

		#endregion



   		#region Property VisibleVScroll

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollView_IsVisibleVScroll( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollView_SetVisibleVScroll( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VisibleVScroll
		{
			get { return  ExportScrollView_IsVisibleVScroll( mNative )  ; }
			set { ExportScrollView_SetVisibleVScroll( mNative,  value ); }
		}

		#endregion



   


   


   


   


   


   


   


   


   

		
    }

}
