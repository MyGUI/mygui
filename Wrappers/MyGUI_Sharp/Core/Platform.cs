using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public class Platform
	{

		#region Platform

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_CreatePlatform([MarshalAs(UnmanagedType.LPStr)] string _logFileName);
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_DestroyPlatform();

		public static void CreatePlatform(string _logFileName)
		{
			ExportGui_CreatePlatform(_logFileName);
		}

		public static void DestroyPlatform()
		{
			ExportGui_DestroyPlatform();
		}

		#endregion

		#region Gui

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_CreateGui([MarshalAs(UnmanagedType.LPStr)] string _coreFileName);
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_DestroyGui();

		public static void CreateGui(string _coreFileName)
		{
			ExportGui_CreateGui(_coreFileName);
		}

		public static void DestroyGui()
		{
			ExportGui_DestroyGui();
		}

		#endregion

	}
}
