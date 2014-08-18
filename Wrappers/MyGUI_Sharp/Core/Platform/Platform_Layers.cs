using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{
		#region Export

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportLayers_GetLayer(
			[MarshalAs(UnmanagedType.U4)] uint _index,
			[Out, MarshalAs(UnmanagedType.U4)] out uint _layerId,
			[Out] out IntPtr _name);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportLayers_SetWidgetDepth(
			IntPtr _widget,
			[MarshalAs(UnmanagedType.R4)] float _depth);

		#endregion

		public static bool GetLayer(uint _index, out uint _layerId, out string _name)
		{
			_name = "";
			IntPtr name = IntPtr.Zero;

			bool result = ExportLayers_GetLayer(_index, out _layerId, out name);

			if (name != IntPtr.Zero)
			{
				_name = Marshal.PtrToStringAnsi(name);
			}

			return result;
		}

		public static void SetWidgetDepth(Widget _widget, float _depth)
		{
			ExportLayers_SetWidgetDepth(_widget.Native, _depth);
		}
	}
}
