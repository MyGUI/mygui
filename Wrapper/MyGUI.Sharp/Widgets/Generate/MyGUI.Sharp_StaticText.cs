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

    public  class StaticText : Widget
    {

        #region StaticText

        public StaticText()
            : base()
        {
        }

        internal StaticText(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal StaticText(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "StaticText"; }

        internal static BaseWidget RequestWrapStaticText(BaseWidget _parent, IntPtr _widget)
        {
            return new StaticText(_parent, _widget);
        }

        internal static BaseWidget RequestCreateStaticText(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new StaticText(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   		#region Property TextColour

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportStaticText_GetTextColour( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticText_SetTextColour( IntPtr _widget, [In] ref Colour _value );

		public Colour TextColour
		{
			get { return  (Colour)Marshal.PtrToStructure(  ExportStaticText_GetTextColour( mNative )  , typeof(Colour) )  ; }
			set { ExportStaticText_SetTextColour( mNative, ref value ); }
		}

		#endregion



   		#region Property TextAlign

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportStaticText_GetTextAlign( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticText_SetTextAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align TextAlign
		{
			get { return  ExportStaticText_GetTextAlign( mNative )  ; }
			set { ExportStaticText_SetTextAlign( mNative,  value ); }
		}

		#endregion



   		#region Property FontHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportStaticText_GetFontHeight( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticText_SetFontHeight( IntPtr _widget,   int _value );

		public int FontHeight
		{
			get { return  ExportStaticText_GetFontHeight( mNative )  ; }
			set { ExportStaticText_SetFontHeight( mNative,  value ); }
		}

		#endregion



   		#region Property FontName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportStaticText_GetFontName( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticText_SetFontName( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string FontName
		{
			get { return  Marshal.PtrToStringAnsi(  ExportStaticText_GetFontName( mNative )  )  ; }
			set { ExportStaticText_SetFontName( mNative,  value ); }
		}

		#endregion



   		#region Method GetTextSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportStaticText_GetTextSize( IntPtr _native );

		public IntSize GetTextSize( )
		{
			return  (IntSize)Marshal.PtrToStructure(  ExportStaticText_GetTextSize( mNative )  , typeof(IntSize) )  ;
		}

		#endregion



   		#region Method GetTextRegion

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportStaticText_GetTextRegion( IntPtr _native );

		public IntCoord GetTextRegion( )
		{
			return  (IntCoord)Marshal.PtrToStructure(  ExportStaticText_GetTextRegion( mNative )  , typeof(IntCoord) )  ;
		}

		#endregion



   


   


   

		
    }

}
