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

    public  class TextBox : Widget
    {

        #region TextBox

        protected override string GetWidgetType() { return "TextBox"; }

        internal static BaseWidget RequestWrapTextBox(BaseWidget _parent, IntPtr _widget)
        {
			TextBox widget = new TextBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateTextBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			TextBox widget = new TextBox();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetCaptionWithReplacing

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetCaptionWithReplacing_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetCaptionWithReplacing(
			string _value )
		{
			ExportTextBox_SetCaptionWithReplacing_value( mNative , 
				 _value );
		}

		#endregion
		#region Property TextShadow

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportTextBox_GetTextShadow( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetTextShadow( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool TextShadow
		{
			get { return  ExportTextBox_GetTextShadow( mNative )  ; }
			set { ExportTextBox_SetTextShadow( mNative,  value ); }
		}

		#endregion
		#region Property TextShadowColour

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetTextShadowColour( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetTextShadowColour( IntPtr _widget, [In] ref Colour _value );

		public Colour TextShadowColour
		{
			get { return  (Colour)Marshal.PtrToStructure(  ExportTextBox_GetTextShadowColour( mNative )  , typeof(Colour) )  ; }
			set { ExportTextBox_SetTextShadowColour( mNative, ref value ); }
		}

		#endregion
		#region Property TextColour

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetTextColour( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetTextColour( IntPtr _widget, [In] ref Colour _value );

		public Colour TextColour
		{
			get { return  (Colour)Marshal.PtrToStructure(  ExportTextBox_GetTextColour( mNative )  , typeof(Colour) )  ; }
			set { ExportTextBox_SetTextColour( mNative, ref value ); }
		}

		#endregion
		#region Property TextAlign

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportTextBox_GetTextAlign( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetTextAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align TextAlign
		{
			get { return  ExportTextBox_GetTextAlign( mNative )  ; }
			set { ExportTextBox_SetTextAlign( mNative,  value ); }
		}

		#endregion
		#region Property FontHeight

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTextBox_GetFontHeight( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetFontHeight( IntPtr _widget,   int _value );

		public int FontHeight
		{
			get { return  ExportTextBox_GetFontHeight( mNative )  ; }
			set { ExportTextBox_SetFontHeight( mNative,  value ); }
		}

		#endregion
		#region Property FontName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetFontName( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetFontName( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string FontName
		{
			get { return  Marshal.PtrToStringAnsi(  ExportTextBox_GetFontName( mNative )  )  ; }
			set { ExportTextBox_SetFontName( mNative,  value ); }
		}

		#endregion
		#region Property Caption

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetCaption( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTextBox_SetCaption( IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public string Caption
		{
			get { return  Marshal.PtrToStringUni(  ExportTextBox_GetCaption( mNative )  )  ; }
			set { ExportTextBox_SetCaption( mNative,  value ); }
		}

		#endregion
		#region Property TextSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetTextSize( IntPtr _native );

		public IntSize TextSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportTextBox_GetTextSize( mNative )  , typeof(IntSize) )  ; }
		}

		#endregion
		#region Property TextRegion

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportTextBox_GetTextRegion( IntPtr _native );

		public IntCoord TextRegion
		{
			get { return  (IntCoord)Marshal.PtrToStructure(  ExportTextBox_GetTextRegion( mNative )  , typeof(IntCoord) )  ; }
		}

		#endregion
		
    }

}
