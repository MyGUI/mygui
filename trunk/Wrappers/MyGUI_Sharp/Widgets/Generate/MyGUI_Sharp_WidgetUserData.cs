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
		private static extern void ExportWidget_ClearUserStrings( IntPtr _native );

		public void ClearUserStrings( )
		{
			ExportWidget_ClearUserStrings( Native );
		}

		#endregion
		#region Method IsUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public bool IsUserString(
			string _key )
		{
			return  ExportWidget_IsUserString_key( Native , 
				 _key  )  ;
		}

		#endregion
		#region Method ClearUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_ClearUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public bool ClearUserString(
			string _key )
		{
			return  ExportWidget_ClearUserString_key( Native , 
				 _key  )  ;
		}

		#endregion
		#region Method GetUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public string GetUserString(
			string _key )
		{
			return  Marshal.PtrToStringAnsi(  ExportWidget_GetUserString_key( Native , 
				 _key  )  )  ;
		}

		#endregion
		#region Method SetUserString

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetUserString_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetUserString(
			string _key ,
			string _value )
		{
			ExportWidget_SetUserString_key_value( Native , 
				 _key ,
				 _value  );
		}

		#endregion
		
    }

}
