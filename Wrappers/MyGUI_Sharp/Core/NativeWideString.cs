using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	internal static class NativeWideString
	{
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMarshaling_FreeWideString(IntPtr value);

		// Only exported getter results are owned; callback arguments remain borrowed.
		internal static string CopyAndFree(IntPtr value)
		{
			try
			{
				return Marshal.PtrToStringUni(value);
			}
			finally
			{
				ExportMarshaling_FreeWideString(value);
			}
		}
	}
}
