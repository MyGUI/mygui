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

    public partial class Widget
    {

		//InsertPoint
		#region Property Height

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetHeight( IntPtr _native );

		public int Height
		{
			get { return  ExportWidget_GetHeight( mNative )  ; }
		}

		#endregion
		#region Property Width

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetWidth( IntPtr _native );

		public int Width
		{
			get { return  ExportWidget_GetWidth( mNative )  ; }
		}

		#endregion
		#region Property Bottom

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetBottom( IntPtr _native );

		public int Bottom
		{
			get { return  ExportWidget_GetBottom( mNative )  ; }
		}

		#endregion
		#region Property Top

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetTop( IntPtr _native );

		public int Top
		{
			get { return  ExportWidget_GetTop( mNative )  ; }
		}

		#endregion
		#region Property Right

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetRight( IntPtr _native );

		public int Right
		{
			get { return  ExportWidget_GetRight( mNative )  ; }
		}

		#endregion
		#region Property Left

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetLeft( IntPtr _native );

		public int Left
		{
			get { return  ExportWidget_GetLeft( mNative )  ; }
		}

		#endregion
		#region Property AbsoluteTop

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetAbsoluteTop( IntPtr _native );

		public int AbsoluteTop
		{
			get { return  ExportWidget_GetAbsoluteTop( mNative )  ; }
		}

		#endregion
		#region Property AbsoluteLeft

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern int ExportWidget_GetAbsoluteLeft( IntPtr _native );

		public int AbsoluteLeft
		{
			get { return  ExportWidget_GetAbsoluteLeft( mNative )  ; }
		}

		#endregion
		#region Property AbsoluteCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteCoord( IntPtr _native );

		public IntCoord AbsoluteCoord
		{
			get { return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetAbsoluteCoord( mNative )  , typeof(IntCoord) )  ; }
		}

		#endregion
		#region Property AbsoluteRect

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteRect( IntPtr _native );

		public IntRect AbsoluteRect
		{
			get { return  (IntRect)Marshal.PtrToStructure(  ExportWidget_GetAbsoluteRect( mNative )  , typeof(IntRect) )  ; }
		}

		#endregion
		#region Property AbsolutePosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetAbsolutePosition( IntPtr _native );

		public IntPoint AbsolutePosition
		{
			get { return  (IntPoint)Marshal.PtrToStructure(  ExportWidget_GetAbsolutePosition( mNative )  , typeof(IntPoint) )  ; }
		}

		#endregion
		#region Property Coord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetCoord( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
		private static extern void ExportWidget_SetCoord( IntPtr _widget, [In] ref IntCoord _value );

		public IntCoord Coord
		{
			get { return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetCoord( mNative )  , typeof(IntCoord) )  ; }
			set { ExportWidget_SetCoord( mNative, ref value ); }
		}

		#endregion
		#region Property Size

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetSize( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
		private static extern void ExportWidget_SetSize( IntPtr _widget, [In] ref IntSize _value );

		public IntSize Size
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWidget_GetSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWidget_SetSize( mNative, ref value ); }
		}

		#endregion
		#region Property Position

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        
		private static extern IntPtr ExportWidget_GetPosition( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
		private static extern void ExportWidget_SetPosition( IntPtr _widget, [In] ref IntPoint _value );

		public IntPoint Position
		{
			get { return  (IntPoint)Marshal.PtrToStructure(  ExportWidget_GetPosition( mNative )  , typeof(IntPoint) )  ; }
			set { ExportWidget_SetPosition( mNative, ref value ); }
		}

		#endregion
		
    }

}
