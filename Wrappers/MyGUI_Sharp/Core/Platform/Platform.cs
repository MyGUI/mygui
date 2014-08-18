using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{
		#region Platform

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportPlatform_Create([MarshalAs(UnmanagedType.LPStr)] string _logFileName);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportPlatform_Destroy();

		public static void CreatePlatform(string _logFileName)
		{
			ExportPlatform_Create(_logFileName);
		}

		public static void DestroyPlatform()
		{
			ExportPlatform_Destroy();
		}

		#endregion

		#region Platform Log

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportPlatform_Log(
			[MarshalAs(UnmanagedType.I4)] LogLevel _level,
			[MarshalAs(UnmanagedType.LPStr)] string _message
			);

		public static void Log(LogLevel _level, string _message)
		{
			ExportPlatform_Log(_level, _message);
		}

		#endregion

		#region Gui

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_Create([MarshalAs(UnmanagedType.LPStr)] string _coreFileName);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_Destroy();

		public static void CreateGui(string _coreFileName)
		{
			ExportGui_Create(_coreFileName);
		}

		public static void DestroyGui()
		{
			ExportGui_Destroy();
		}

		#endregion

		#region DataManager

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportDataManager_AddResourceLocation([MarshalAs(UnmanagedType.LPStr)] string _logFileName, [MarshalAs(UnmanagedType.U1)] bool _recursive);

		public static void AddResourceLocation(string _path, bool _recursive)
		{
			ExportDataManager_AddResourceLocation(_path, _recursive);
		}

		#endregion
	}
}
