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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetHeight(IntPtr _native);

		public int Height
		{
			get { return ExportWidget_GetHeight(Native); }
		}

		#endregion
		#region Property Width

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetWidth(IntPtr _native);

		public int Width
		{
			get { return ExportWidget_GetWidth(Native); }
		}

		#endregion
		#region Property Bottom

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetBottom(IntPtr _native);

		public int Bottom
		{
			get { return ExportWidget_GetBottom(Native); }
		}

		#endregion
		#region Property Top

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetTop(IntPtr _native);

		public int Top
		{
			get { return ExportWidget_GetTop(Native); }
		}

		#endregion
		#region Property Right

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetRight(IntPtr _native);

		public int Right
		{
			get { return ExportWidget_GetRight(Native); }
		}

		#endregion
		#region Property Left

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetLeft(IntPtr _native);

		public int Left
		{
			get { return ExportWidget_GetLeft(Native); }
		}

		#endregion
		#region Property AbsoluteTop

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteTop(IntPtr _native);

		public int AbsoluteTop
		{
			get { return ExportWidget_GetAbsoluteTop(Native); }
		}

		#endregion
		#region Property AbsoluteLeft

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteLeft(IntPtr _native);

		public int AbsoluteLeft
		{
			get { return ExportWidget_GetAbsoluteLeft(Native); }
		}

		#endregion
		#region Property AbsoluteCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteCoord(IntPtr _native);

		public IntCoord AbsoluteCoord
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetAbsoluteCoord(Native), typeof(IntCoord)); }
		}

		#endregion
		#region Property AbsoluteRect

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteRect(IntPtr _native);

		public IntRect AbsoluteRect
		{
			get { return (IntRect)Marshal.PtrToStructure(ExportWidget_GetAbsoluteRect(Native), typeof(IntRect)); }
		}

		#endregion
		#region Property AbsolutePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsolutePosition(IntPtr _native);

		public IntPoint AbsolutePosition
		{
			get { return (IntPoint)Marshal.PtrToStructure(ExportWidget_GetAbsolutePosition(Native), typeof(IntPoint)); }
		}

		#endregion
		#region Property Coord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetCoord(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord(IntPtr _widget, [In] ref IntCoord _value);

		public IntCoord Coord
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetCoord(Native), typeof(IntCoord)); }
			set { ExportWidget_SetCoord(Native, ref value); }
		}

		#endregion
		#region Property Size

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetSize(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize(IntPtr _widget, [In] ref IntSize _value);

		public IntSize Size
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportWidget_GetSize(Native), typeof(IntSize)); }
			set { ExportWidget_SetSize(Native, ref value); }
		}

		#endregion
		#region Property Position

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetPosition(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition(IntPtr _widget, [In] ref IntPoint _value);

		public IntPoint Position
		{
			get { return (IntPoint)Marshal.PtrToStructure(ExportWidget_GetPosition(Native), typeof(IntPoint)); }
			set { ExportWidget_SetPosition(Native, ref value); }
		}

		#endregion
		
    }
}
