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
		#region Method ClearUserStrings

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ClearUserStrings(IntPtr _native);

		public void ClearUserStrings( )
		{
			ExportWidget_ClearUserStrings(Native);
		}

		#endregion
		#region Method IsUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_IsUserString__key(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _key);

		public bool IsUserString(
			string _key)
		{
			return ExportWidget_IsUserString__key(Native,
				_key);
		}

		#endregion
		#region Method ClearUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_ClearUserString__key(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _key);

		public bool ClearUserString(
			string _key)
		{
			return ExportWidget_ClearUserString__key(Native,
				_key);
		}

		#endregion
		#region Method GetUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetUserString__key(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _key);

		public string GetUserString(
			string _key)
		{
			return Marshal.PtrToStringAnsi(ExportWidget_GetUserString__key(Native,
				_key));
		}

		#endregion
		#region Method SetUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetUserString__key__value(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _key,
			[MarshalAs(UnmanagedType.LPStr)] string _value);

		public void SetUserString(
			string _key,
			string _value)
		{
			ExportWidget_SetUserString__key__value(Native,
				_key,
				_value);
		}

		#endregion
		
    }
}
