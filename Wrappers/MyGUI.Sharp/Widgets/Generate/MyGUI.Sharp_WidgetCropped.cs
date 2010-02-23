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

   		#region Method GetHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetHeight( IntPtr _native );

		public int GetHeight( )
		{
			return  ExportWidget_GetHeight( mNative )  ;
		}

		#endregion



   		#region Method GetWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetWidth( IntPtr _native );

		public int GetWidth( )
		{
			return  ExportWidget_GetWidth( mNative )  ;
		}

		#endregion



   		#region Method GetBottom

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetBottom( IntPtr _native );

		public int GetBottom( )
		{
			return  ExportWidget_GetBottom( mNative )  ;
		}

		#endregion



   		#region Method GetTop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetTop( IntPtr _native );

		public int GetTop( )
		{
			return  ExportWidget_GetTop( mNative )  ;
		}

		#endregion



   		#region Method GetRight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetRight( IntPtr _native );

		public int GetRight( )
		{
			return  ExportWidget_GetRight( mNative )  ;
		}

		#endregion



   		#region Method GetLeft

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetLeft( IntPtr _native );

		public int GetLeft( )
		{
			return  ExportWidget_GetLeft( mNative )  ;
		}

		#endregion



   		#region Property Align

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportWidget_GetAlign( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align Align
		{
			get { return  ExportWidget_GetAlign( mNative )  ; }
			set { ExportWidget_SetAlign( mNative,  value ); }
		}

		#endregion



   		#region Method GetAbsoluteTop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteTop( IntPtr _native );

		public int GetAbsoluteTop( )
		{
			return  ExportWidget_GetAbsoluteTop( mNative )  ;
		}

		#endregion



   		#region Method GetAbsoluteLeft

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteLeft( IntPtr _native );

		public int GetAbsoluteLeft( )
		{
			return  ExportWidget_GetAbsoluteLeft( mNative )  ;
		}

		#endregion



   		#region Method GetAbsoluteCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteCoord( IntPtr _native );

		public IntCoord GetAbsoluteCoord( )
		{
			return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetAbsoluteCoord( mNative )  , typeof(IntCoord) )  ;
		}

		#endregion



   		#region Method GetAbsoluteRect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteRect( IntPtr _native );

		public IntRect GetAbsoluteRect( )
		{
			return  (IntRect)Marshal.PtrToStructure(  ExportWidget_GetAbsoluteRect( mNative )  , typeof(IntRect) )  ;
		}

		#endregion



   		#region Method GetAbsolutePosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsolutePosition( IntPtr _native );

		public IntPoint GetAbsolutePosition( )
		{
			return  (IntPoint)Marshal.PtrToStructure(  ExportWidget_GetAbsolutePosition( mNative )  , typeof(IntPoint) )  ;
		}

		#endregion



   		#region Property Visible

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsVisible( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Visible
		{
			get { return  ExportWidget_IsVisible( mNative )  ; }
			set { ExportWidget_SetVisible( mNative,  value ); }
		}

		#endregion



   		#region Property Size

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize( IntPtr _widget,  ref IntSize _value );

		public IntSize Size
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWidget_GetSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWidget_SetSize( mNative, ref value ); }
		}

		#endregion



   		#region Property Position

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetPosition( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition( IntPtr _widget, [In] ref IntPoint _value );

		public IntPoint Position
		{
			get { return  (IntPoint)Marshal.PtrToStructure(  ExportWidget_GetPosition( mNative )  , typeof(IntPoint) )  ; }
			set { ExportWidget_SetPosition( mNative, ref value ); }
		}

		#endregion



   		#region Property Coord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetCoord( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord( IntPtr _widget, [In] ref IntCoord _value );

		public IntCoord Coord
		{
			get { return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetCoord( mNative )  , typeof(IntCoord) )  ; }
			set { ExportWidget_SetCoord( mNative, ref value ); }
		}

		#endregion



   

		
    }

}
